#include <iostream>
#include <fstream>
#include "pthread.h"
#include "CInsim.h"
#include <string>
#include <sstream>
#include <stdexcept>

//functions---------
void log_per_player (byte playerID, float posX, float posY, float speed, int position_difference, word lap);
std::string removeColorCodes (std::string s);
//------------------
#define MAXPLAYERID 256 //FIXME: use map, vector or something

CInsim insim;			//CInsim object, handles the communication with LFS
std::ofstream collisionCoordinates;
std::ofstream output; //coordinates logging of all cars into one file "output.txt"
std::ofstream babbler; //general messages and debug

int raceLaps = 9999;    //how many laps this race? needed so that cars who have finished can be ignored
std::string playerIDtoNickname (byte playerID);
int highestNumber =0; //fuer nummerierung im Car struct
struct Car {
    std::string drivername;
    std::string carname;
//    byte playerID;
    bool logfile_open;
    bool stillRacing;
    std::ofstream logfile;
    int carContactCounter;
    int number; //fortlaufende nummerierung (für dateinamen des logfiles)
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
        //std::cout<<"["<< i << "] playerID:" << playerid << " speed: " << speed << std::endl;
        //output << makeSVGCircle (posX, posY, 1, "#000000") << std::endl;
        if (cars[playerid].stillRacing) //(lap <= raceLaps)
            {
            output << posX << " " << posY << " " << speed<< " " << position_difference << " " << lap << std::endl;
            log_per_player (playerid,  posX,  posY,  speed,  position_difference, lap);
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
    collisionCoordinates<<  collisionPacket->A.X  / 16<< " " << collisionPacket->A.Y / 16 << "\n" << collisionPacket->B.X  / 16<< " " << collisionPacket->B.Y  / 16<< std::endl;
    //output << makeSVGCircle (-collisionPacket->A.X / 16, collisionPacket->A.Y / 16, 3, "#ff0000") << std::endl;
    //output << makeSVGCircle (-collisionPacket->B.X / 16, collisionPacket->B.Y / 16, 3, "#ff00ff") << std::endl;
    cars[collisionPacket->A.PLID].carContactCounter ++;
    cars[collisionPacket->B.PLID].carContactCounter ++;
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
    std::string carname = ccar;
    std::string snickname = nickname;
    std::cout << "(NPL) player joined race:" << snickname << "playerID:" << (int)playerID << std::endl;
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
    babbler << "Car contacts: " << playerIDtoNickname (playerID) << " "<< cars[playerID].carContactCounter <<std::endl;
}

void ncn_message ()
{
    struct IS_NCN* ncnPacket = (struct IS_NCN*)insim.get_packet(); //packet abholen
    char *nickname;
    nickname = ncnPacket->PName;
    std::cout << "(NCN) player joined race:" << nickname << std::endl;
}


void log_per_player (byte playerID, float posX, float posY, float speed, int position_difference, word lap)
{
    cars[playerID].logfile << posX << " " << posY << " " << speed<< " " << position_difference << " " << lap << std::endl;
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
              ISF_MCI+ISF_CON + ISF_OBH,		//what kind of packets to recieve?
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
            std::cout<<"*"<<std::flush;
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

        if (packetType == ISP_FIN) //racer finished race
        {
            fin_message();
        }

    }
    return 0;
}
