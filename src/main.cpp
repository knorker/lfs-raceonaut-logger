#include <iostream>
#include <iostream>
#include <fstream>
#include "pthread.h"
#include "CInsim.h"
#include <string>
#include <cmath>
#include <sstream>
#include <stdexcept>

//functions---------
void log_per_player (byte playerID, float posX, float posY, float speed, int position_difference, word lap, float drift);
void setCameraToPlayer (int playerID);
void takeScreenshot ();
std::string stringChain (int n, std::string s);
std::string removeColorCodes (std::string s);
std::string tyresString (byte tyres[]);
//------------------
#define MAXPLAYERID 256 //FIXME: use map, vector or something


std::string tyreNames[] = { std::string("R1"),
                           std::string("R2"),
                           std::string("R3"),
                           std::string("R4"),
                           std::string("Road Super"),
                           std::string("Road Normal"),
                           std::string("Hybrid"),
                           std::string("Knobbly"),
                            };

CInsim insim;			//CInsim object, handles the communication with LFS
std::ofstream collisionCoordinates;
std::ofstream output; //coordinates logging of all cars into one file "output.txt"
std::ofstream babbler; //general messages and debug
std::ofstream pitstops; //for logging type of tyres

int raceLaps = 9999;    //how many laps this race? needed so that cars who have finished can be ignored
std::string playerIDtoNickname (byte playerID);
int highestNumber =0; //fuer nummerierung im Car struct
struct Car {
    std::string drivername;
    std::string carname; //"XFG", "FBM" etc
//    byte playerID;
    bool logfile_open;
    bool stillRacing;
    std::ofstream logfile;
    std::ofstream logfileLapTimes;
    int completedLaps;
    int carContactCounter;
    int objectContactCounter;
    int number; //fortlaufende nummerierung (für dateinamen des logfiles)
    std::string tyres; //"Start: R2 R2 Lap 15: R3 R3"
};
Car cars[255]; //indexed [playerID]   //FIXME: use map, vector or something

int playerPosition [MAXPLAYERID]; //-1= nothing yet

bool titleWritten = false;  //has title (trackname + number of laps) for output file been written already?

std::string itos(int i) // convert int to string
{
    std::stringstream s;
    s << i;
    return s.str();
}

class BadConversion : public std::runtime_error {
public:
  BadConversion(std::string const& s)
    : std::runtime_error(s)
    { }
};
inline std::string ntos(double x) //number to string
{
  std::ostringstream o;
  if (!(o << x))
    throw BadConversion("ntos(double)");
  return o.str();
}


//zum basteln von strings wie: iTerm ("radius",15) --> " radius="15" "
std::string iTerm (std::string name, int value) {
    return " " + name + "="+ "\""+itos(value)+"\"" + " ";
}
std::string sTerm (std::string name, std::string value) {
    return " " + name + "="+ "\""+value+"\"" + " ";
}

std::string makeSVGCircle (int x, int y, int r, std::string color)
{
    std::string s ="<ellipse " + iTerm("fill-opacity",0)
                + iTerm("ry",r)+iTerm("rx",r)
                +iTerm("cx",x)+iTerm("cy",y)
                +iTerm("stroke-width",0)+sTerm("stroke",color)+sTerm("fill",color)
                +"/>";
    return s;
}

//direcion & heading in 0-360°
float driftAngle (float direction, float heading)
{
    float absdiff = fabs(direction - heading);
    if (absdiff <= 180.0f) return absdiff;
    if (direction < 180)
    {
        heading -= 360;
        return direction - heading;
    }
    else
    {
        direction -= 360;
        return heading - direction;
    }
}




//Multi Car Info
void mci_message ()
{
    struct IS_MCI* mciPacket = (struct IS_MCI*)insim.get_packet(); //packet abholen
    //ein mci enthaelt mehere autos
    for (int i=0; i < mciPacket->NumC; i++)
    {
        /*int*/ float posX = mciPacket->Info[i].X / 65536.0f; //in meters
        /*int*/ float posY = mciPacket->Info[i].Y / 65536.0f;
        float speed = (mciPacket->Info[i].Speed / 327.68f) * 3.6f; //in km/h   (32768 = 100 m/s)
        int playerid = mciPacket->Info[i].PLID;
        word lap = mciPacket->Info[i].Lap;
        int position = mciPacket->Info[i].Position;
        float heading = mciPacket->Info[i].Heading / 182.04f; // / 32768.0f*180.0f;
        float direction = mciPacket->Info[i].Direction /182.04f; // 32768.0f*180.0f;

        int position_difference = 0;
        if (position != playerPosition[playerid])
            {
            if (playerPosition[playerid] > 0)
            {
                std::cout<< "Position change Player " << playerid << ": " << playerPosition[playerid] << "->" << position << std::endl;
                position_difference = position - playerPosition[playerid];
            }
            playerPosition[playerid] = position;
            }
        float drift = driftAngle (direction, heading);

        //std::cout<< "playerID:" << playerid << " heading: " << heading << "\tdirection: " << direction<< "\tdrift:"<<drift<<std::endl;

        //std::cout<<"["<< i << "] playerID:" << playerid << " speed: " << speed << std::endl;
        //output << makeSVGCircle (posX, posY, 1, "#000000") << std::endl;
        if (cars[playerid].stillRacing) //(lap <= raceLaps)
            {
            output << posX << " " << posY << " " << speed<< " " << position_difference << " " << lap << " " << drift <<std::endl;
            log_per_player (playerid,  posX,  posY,  speed,  position_difference, lap, drift);
            }
            else
            {
            std::cout <<"F";
            }
    }
}

void collision_message ()
{
    struct IS_CON* collisionPacket = (struct IS_CON*)insim.get_packet(); //packet abholen
    std::cout << "contact between" << (int) collisionPacket->A.PLID << " and " << (int) collisionPacket->B.PLID << std::endl;
    std::cout << "NAMES:" << playerIDtoNickname((int) collisionPacket->A.PLID) << " and " << playerIDtoNickname((int) collisionPacket->B.PLID) << std::endl;

    //std::cout << "at" <<  collisionPacket->A.X << "," << collisionPacket->A.Y << "  " << collisionPacket->B.X << "," << collisionPacket->B.Y << std::endl;
    //std::cout << (int) collisionPacket->Time<< std::endl;
//    collisionCoordinates << "<circle cx=";
    //collisionCoordinates <<  collisionPacket->A.X << " cy=" << collisionPacket->A.Y << " r=10" << "/>" << std::endl;
    //output << makeSVGCircle (-collisionPacket->A.X / 16, collisionPacket->A.Y / 16, 3, "#ff0000") << std::endl;
    //output << makeSVGCircle (-collisionPacket->B.X / 16, collisionPacket->B.Y / 16, 3, "#ff00ff") << std::endl;
    if (cars[collisionPacket->A.PLID].stillRacing && cars[collisionPacket->B.PLID].stillRacing)
        {
        cars[collisionPacket->A.PLID].carContactCounter ++;
        cars[collisionPacket->B.PLID].carContactCounter ++;
        collisionCoordinates<<  collisionPacket->A.X  / 16<< " " << collisionPacket->A.Y / 16 << "\n" << collisionPacket->B.X  / 16<< " " << collisionPacket->B.Y  / 16<< std::endl;
        }
    babbler << "CONTACT:" << playerIDtoNickname((int) collisionPacket->A.PLID) << " <-vs-> " << playerIDtoNickname((int) collisionPacket->B.PLID) << " lap:" <<  cars [collisionPacket->A.PLID].completedLaps << std::endl;

    setCameraToPlayer (collisionPacket->A.PLID);
    takeScreenshot();

}

void state_message ()
{
    struct IS_STA* statePacket = (struct IS_STA*)insim.get_packet(); //packet abholen
    raceLaps = statePacket->RaceLaps;
    char *trackname;
    trackname = statePacket->Track;
    std::cout << "RaceLaps:" << raceLaps << std::endl;
    std::cout << "TrackName:" << trackname << std::endl;
    if (!titleWritten)
        {
        output<< "\"" << "TrackName:" << trackname << " Laps:" << raceLaps << "\"" << std::endl;
        titleWritten = true;
        }
}

//"struct IS_NPL // New PLayer joining race (if PLID already exists, then leaving pits)"
void npl_message ()
{
    struct IS_NPL* nplPacket = (struct IS_NPL*)insim.get_packet();
    char *nickname;
    nickname = nplPacket->PName;
    byte playerID;
    playerID = nplPacket->PLID;
    char *ccar;
    ccar = nplPacket->CName;
    cars[playerID].tyres = "";
    byte *tyres;
    tyres = nplPacket->Tyres;

    std::string carname = ccar;
    std::string snickname = nickname;
    std::cout << "(NPL) player joined race:" << snickname << "playerID:" << (int)playerID << std::endl;

    std::cout << "start tyres: " << tyresString (tyres) << std::endl;
    cars[playerID].tyres = "Start:" + tyresString (tyres);
    cars[playerID].completedLaps = 0;
    cars[playerID].objectContactCounter = 0;
    snickname = removeColorCodes (snickname);
    cars[playerID].drivername = snickname;
    cars[playerID].number = highestNumber;
    cars[playerID].stillRacing = true;
    if (!cars[playerID].logfile_open)
    {
        std::string s = "log\\playerlogs\\log"+ntos(highestNumber)+".txt";
        cars[playerID].logfile.open(s.c_str());
        cars[playerID].logfile_open = true;
        cars[playerID].logfile << '\"'<< snickname << " "<< "Car: "<< carname << '\"' << (int)playerID<<std::endl;   //"nickname" in quotes so gnuplots reads it as whole thing instead of stopping at spaces
        babbler << snickname << "\t\t\t\t--- > " << (int)playerID<<std::endl;
    }
    highestNumber++;
}

//player finished race, car will not be logged anymore
void fin_message ()
{
    struct IS_FIN* nplPacket = (struct IS_FIN*)insim.get_packet();
    byte playerID;
    playerID = nplPacket->PLID;
    std::cout << playerIDtoNickname (playerID) << " has finished racing"<<std::endl;
    cars[playerID].stillRacing = false;
    babbler << "Car contacts: " << playerIDtoNickname (playerID) << " " << stringChain (cars[playerID].carContactCounter, "(st) ") << " " << cars[playerID].carContactCounter <<std::endl;
    pitstops << playerIDtoNickname (playerID) << " tires: " << cars[playerID].tyres<<std::endl;
}

void ncn_message ()
{
    struct IS_NCN* ncnPacket = (struct IS_NCN*)insim.get_packet(); //packet abholen
    char *nickname;
    nickname = ncnPacket->PName;
    std::cout << "(NCN) player joined race:" << nickname << std::endl;
}


void log_per_player (byte playerID, float posX, float posY, float speed, int position_difference, word lap, float drift)
{
    cars[playerID].logfile << posX << " " << posY << " " << speed<< " " << position_difference << " " << lap << " " << drift << std::endl;
}

std::string playerIDtoNickname (byte playerID)
{
    return cars[playerID].drivername;
}


//autocross objects hit
void obh_message ()
{
    struct IS_OBH* obhPacket = (struct IS_OBH*)insim.get_packet();
    short x;
    short y;
    byte playerID;
    byte OBHFlags;
    x = obhPacket->X/16;
    y = obhPacket->Y/16;
    playerID = obhPacket->PLID;
    OBHFlags = obhPacket->OBHFlags;
    std::cout << (int)OBHFlags;
    //if (! OBHFlags&OBH_WAS_MOVING)
    {
        std::cout << " Objekt gerammt von:" << playerIDtoNickname(playerID) << " bei " << x <<":" <<y<< std::endl;
    babbler << x <<" " <<y<< std::endl;
    }

}

void lap_message ()
{
    struct IS_LAP* lapPacket = (struct IS_LAP*)insim.get_packet();
    byte playerID = lapPacket->PLID;
    unsigned LTime = lapPacket->LTime;
    cars[playerID].completedLaps = lapPacket->LapsDone;
}

void spx_message ()
{

}

void lapSplit_time_message ()
{

}

void pit_message ()
{
    struct IS_PIT* pitPacket = (struct IS_PIT*)insim.get_packet();
    int playerID = (int) pitPacket->PLID;
    word lap = pitPacket->LapsDone +1;
    byte *tyres;
    tyres = pitPacket->Tyres;
    //tyres[0] = (int) pitPacket->Tyres[0];
    //tyres[1] = (int) pitPacket->Tyres[1];
    //tyres[2] = (int) pitPacket->Tyres[2];
    //tyres[3] = (int) pitPacket->Tyres[3];
    //255 = not changed!
    std::cout << "tyres[0]="  << (int) tyres[0] << std::endl;
    std::cout << "tyres[1]="  << (int) tyres[1] << std::endl;
    std::cout << "tyres[2]="  << (int) tyres[2] << std::endl;
    std::cout << "tyres[3]="  << (int) tyres[3] << std::endl;

/*
    std::cout << "tyres[0]="  << tyres[0] <<" " << tyreNames [tyres[0]]<< std::endl;
    std::cout << "tyres[1]="  << tyres[1] <<" " << tyreNames [tyres[1]]<< std::endl;
    std::cout << "tyres[2]="  << tyres[2] <<" " << tyreNames [tyres[2]]<< std::endl;
    std::cout << "tyres[3]="  << tyres[3] <<" " << tyreNames [tyres[3]]<< std::endl;
*/
    std::cout << playerIDtoNickname (playerID) << " pitted." << std::endl;
    std::cout << "tyres after pit: " << tyresString (tyres) << std::endl;
    cars[playerID].tyres += " Lap:" +itos (lap)+ tyresString (tyres);
}


std::string tyresString (byte tyres[])
{
       std::string s = " ";
        int i = 4;
        while (i!=0)
        {
            i--;
            std::cout << i  << ":" << (int)tyres[i] << std::endl;
            if (tyres[i]==NOT_CHANGED)
            {
                s=s+"(not changed)" + "-";
            }
            else
            {
                s+=tyreNames[(int)tyres[i]] + "-";
            }
        }
    return s;
}

std::string removeColorCodes (std::string s)
{
std::string s2;
for (int i = 0;  i < s.length();  i++)
{
    if ( (s[i] == '^') && ((s[i+1] >= '0' && s[i+1] <= '9')) )
    {

        i++;
    }
    else
    {
        if ( s[i] == '^' && s[i+1] =='s')
        {
            s2+='/';
            i++;
        }
        else
        {
            s2+=s[i];
        }
    }
}
return s2;
}

//stringChain (3, "ho") -> "hohoho"
std::string stringChain (int n, std::string s)
{
    if (n<1) return "";
    std::string chain;
    int i = 0;
    while (i < n)
    {
        chain+=s;
        i++;
    }
return chain;
}


void setCameraToPlayer (int playerID)
{
    std::cout << "send IS_SCC to change camera...";
    struct IS_SCC camera_packet;
    memset(&camera_packet, 0, sizeof(struct IS_SCC));
    camera_packet.Size = sizeof(struct IS_SCC);
    camera_packet.Type = ISP_SCC;
    camera_packet.ViewPLID = playerID;
    camera_packet.InGameCam = 255; //255=unchanged
    insim.send_packet(&camera_packet);

}

void takeScreenshot ()
{
    std::cout << "send IS_SSH to take screenshot...";
    struct IS_SSH packet;
    memset(&packet, 0, sizeof(struct IS_SSH));

    packet.Size = sizeof(struct IS_SSH);

    packet.Type = ISP_SSH;
    packet.ReqI = 1;
//    packet.BMP = "NULL";
    insim.send_packet(&packet);

}


int main(int argc, char* argv[])
{
    int retVal = 0;
    struct IS_VER verPack;	//LFS can tell us info about it's version and InSim version, store that info in this struct.
    //Try to connect to LFS
    retVal = insim.init("127.0.0.1",	//LFS will run on local machine
              (word)29999,	//InSim will accept connection on port 29999
              "Raceonaut Logger",	//Short name for our app, will appear in LFS
              "",		//No admin password
              &verPack,		//Pointer to IS_VERSION packet
              '!',		//InSim command character
            //what kind of packets to recieve:
              ISF_MCI+  //multi car information (positions, speed)
              ISF_CON+ //player (dis) connecting
              ISF_OBH+  //autocross object hit
              ISP_LAP,
              //ISF_PIT,
  //            ISP_LAP+  //lap times
  //            ISP_SPX,  //split times
              250,		//Receive updates every x msecs
              0);		//No UDP today

    //Could we connect to LFS?
    if(retVal < 0) //We could not
    {
        //Tell the user and exit
        std::cout << "Error connecting to LFS!" << std::endl;
        return -555;
    }
    //All OK, report version of LFS and InSim.
    std::cout << "Its happening! Connected to LFS " << verPack.Version << ", InSim ver. " << verPack.InSimVer << std::endl;

    std::cout << "send NPL request to get players...";
    struct IS_TINY pack_requests;
    memset(&pack_requests, 0, sizeof(struct IS_TINY));
    pack_requests.Size = sizeof(struct IS_TINY);
    pack_requests.Type = ISP_TINY;
    pack_requests.ReqI = 1;
    pack_requests.SubT = TINY_NPL;      // Request all players in-grid to know their PLID: this causes lots of IS_NPL to be sent to us
    insim.send_packet(&pack_requests);
    std::cout << "tada." << std::endl;

    collisionCoordinates.open ("log\\scollisionlog.txt");
    output.open ("log\\output.txt");
    babbler.open ("log\\gebabbel.txt");
    pitstops.open ("log\\pitstops.txt");

    for(int i=0; i<MAXPLAYERID; i++)
        {
        playerPosition[i] = -1;
        }

    while (true)
    {
        int errorReturn = insim.next_packet();  //get next packet in queue
        if(errorReturn < 0)	//Something went wrong
        {
            std::cout<<"insim.next_packet() failed:" << errorReturn<< std::endl;
            return -666;
        }
        int packetType = insim.peek_packet();   //what type of packet is it? (this just tells the type but does not actuelly get the package)
        //    std::cout<<packetType<<std::endl;
        if (packetType == ISP_MCI)  //multi car information
        {
            //std::cout<<"*"<<std::flush;
            mci_message ();
        }

        if (packetType == ISP_CON) //car-car contact
        {
            std::cout<<"CAR CONTACT!"<<std::endl;
            collision_message();
        }

        if (packetType == ISP_NPL) //new player
        {
            npl_message();
        }
        if (packetType == ISP_NCN) //new connection
        {
            ncn_message();
        }

        if (packetType == ISP_OBH) //object hit
        {
            obh_message();
        }

        if (packetType == ISP_SSH)
        {
            std::cout<<"screenshot!"<<std::endl;
        }

        if (packetType == ISP_STA && !titleWritten) //status (track name, amount of laps)
        {
            std::cout<<"IS_STA (state)"<<std::endl;
            state_message ();
        }


        if (packetType == ISP_LAP) //finished a lap
        {
            lap_message ();
        }

        if (packetType == ISP_SPX ) //crossed a split
        {
            spx_message ();
        }


        if (packetType==ISP_PIT)
            {
            pit_message ();
            }

        if (packetType == ISP_FIN) //racer finished race
        {
            fin_message();
        }
    }
    return 0;
}
