#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define MODDERS_EDITION_VERSION_ID	4

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "1986"

#include "math.h"
#include "windows.h"
#include "winuser.h"

#include "AppRes.h"

#include "ddraw.h"

#ifdef _d3d
#include "d3d.h"
#endif

#define ctHScale  64
#define PMORPHTIME 256
#define HiColor(R,G,B) ( ((R)<<10) + ((G)<<5) + (B) )


#define TCMAX ((128<<16)-62024)
#define TCMIN ((000<<16)+62024)

#define DINOINFO_MAX	128
#define TROPHY_COUNT	24
#define TROPHY2_COUNT	128

#ifdef _MAIN_
#define _EXTORNOT
#else
#define _EXTORNOT extern
#endif

#define pi 3.1415926535f
#define ctMapSize 1024

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct tagMessageList
{
  int timeleft;
  char mtext[256];
} TMessageList;

typedef struct tagTRGB
{
  BYTE B;
  BYTE G;
  BYTE R;
} TRGB;

typedef struct _Animation
{
  char aniName[32];
  int aniKPS, FramesCount, AniTime;
  short int* aniData;
} TAni;

typedef struct _VTLdata
{
  int aniKPS, FramesCount, AniTime;
  short int* aniData;
} TVTL;

typedef struct _SoundFX
{
  int  length;
  short int* lpData;
} TSFX;



typedef struct _TRD
{
  int  RNumber, RVolume, RFreq;
  WORD REnvir, Flags;
} TRD;

typedef struct _TAmbient
{
  TSFX sfx;
  TRD  rdata[16];
  int  RSFXCount;
  int  AVolume;
  int  RndTime;
} TAmbient;


typedef struct TagTEXTURE
{
  WORD DataA[128*128];
  WORD DataB[64*64];
  WORD DataC[32*32];
  WORD DataD[16*16];
  WORD SDataC[2][32*32];
  int mR, mG, mB;
} TEXTURE;



typedef struct _TPicture
{
  int W,H;
  WORD* lpImage;
} TPicture;


typedef struct TagVector3d
{
  float x,y,z;
} Vector3d;

typedef struct TagPoint3di
{
  int x,y,z;
} TPoint3di;

typedef struct TagVector2di
{
  int x,y;
} Vector2di;

typedef struct TagVector2df
{
  float x,y;
} Vector2df;


typedef struct TagScrPoint
{
#ifdef _soft
  int   x,y, tx,ty;
#else
  float x,y, tx,ty;
#endif

  int Light, z, r2, r3;
} ScrPoint;

typedef struct TagMScrPoint
{
  int x,y, tx,ty;
} MScrPoint;

typedef struct tagClipPlane
{
  Vector3d v1,v2,nv;
} CLIPPLANE;





typedef struct TagEPoint
{
  Vector3d v;
  WORD DFlags;
  short int ALPHA;
  int  scrx, scry, Light;
  float Fog;
} EPoint;


typedef struct TagClipPoint
{
  EPoint ev;
  float tx, ty;
} ClipPoint;


//================= MODEL ========================
typedef struct _Point3d
{
  float x;
  float y;
  float z;
  short owner;
  short hide;
} TPoint3d;



typedef struct _Face
{
  int v1, v2, v3;
#ifdef _soft
  int   tax, tbx, tcx, tay, tby, tcy;
#else
  float tax, tbx, tcx, tay, tby, tcy;
#endif
  WORD Flags,DMask;
  int Distant, Next, group;
  char reserv[12];
} TFace;


typedef struct _Facef
{
  int v1, v2, v3;
  float tax, tbx, tcx, tay, tby, tcy;
  WORD Flags,DMask;
  int Distant, Next, group;
  char reserv[12];
} TFacef;



typedef struct _Obj
{
  char OName [32];
  float ox;
  float oy;
  float oz;
  short owner;
  short hide;
} TObj;


typedef struct TagMODEL
{
  int VCount, FCount, TextureSize, TextureHeight;
  TPoint3d *gVertex;
  union
  {
    TFace    *gFace;
    TFacef   *gFacef;
  };
  WORD     *lpTexture, *lpTexture2, *lpTexture3;
#ifdef _d3d
  int*      VLight[4];
#else
  float*    VLight[4];
#endif
} TModel;


//=========== END MODEL ==============================//


typedef struct _ObjInfo
{
  int  Radius;
  int  YLo, YHi;
  int  linelenght, lintensity;
  int  circlerad, cintensity;
  int  flags;
  int  GrRad;
  int  DefLight;
  int  LastAniTime;
  float BoundR;
  BYTE res[16];
} TObjInfo;

typedef struct _TBMPModel
{
  Vector3d  gVertex[4];
  WORD     *lpTexture;
} TBMPModel;

typedef struct _TBound
{
  float cx, cy, a, b,  y1, y2;
} TBound;

typedef struct TagObject
{
  TObjInfo info;
  TBound   bound[8];
  TBMPModel bmpmodel;
  TModel  *model;
  TVTL    vtl;
} TObject;


typedef struct _TCharacterInfo
{
  char ModelName[32];
  int AniCount,SfxCount;
  TModel* mptr;
  TAni Animation[64];
  TSFX SoundFX[64];
  int  Anifx[64];
} TCharacterInfo;

typedef struct _TWeapon
{
  TCharacterInfo chinfo[10];
  TPicture       BulletPic[10];

  Vector3d*       normals;
  int state, FTime;
  float shakel;
} TWeapon;



typedef struct _TWCircle
{
  Vector3d pos;
  float scale;
  int FTime;
} TWCircle;




typedef struct _TSnowElement {
	Vector3d pos;
	float hl, ftime;
} TSnowElement;


typedef struct _TCharacter
{
  int CType, Clone;
  TCharacterInfo *pinfo;
  int StateF;
  int State;
  int NoWayCnt, NoFindCnt, AfraidTime, tgtime;
  int PPMorphTime, PrevPhase,PrevPFTime, Phase, FTime;

  int currentIdleGroup;
  int currentIdle2Group;

  float vspeed, rspeed, bend, scale;
  int Slide;
  float slidex, slidez;
  float tgx, tgz;

  Vector3d pos, rpos;
  float tgalpha, alpha, beta,
        tggamma,gamma,
        lookx, lookz;
  int Health, BloodTime, BloodTTime;

  //ICTH
  bool gliding = false;
  //  bool wingUp = false;
  bool notFlushed = false;
  int deathPhase;
  bool canSleep = false;
  float shakeTime = 0;
  float spawnAlt;

  //MOSA
  float depth, tdepth;
  float bdepth = 0;//bend
  float lastTBeta = 0;
  float turny = 0;

  int spcDepth;

  int SpawnGroupType;

  int packId;
  bool followLeader;

  int killType;
  int deathType;
  int roarAnim;
  int waterDieAnim;

  int dogPrey; // used by dog only. The dino currently being tracked

  bool awareHunter;
  bool heardShot;

  bool aquaticIdle;

  int tropAnim;

  int xdata, zdata, ydata;

  bool animateTrophy;

  int _PhaseM;

  Vector3d climbable;
  float climbY;
  BOOL gottaClimb;

  Vector3d sonar;
  BOOL showSonar;

  bool cpcpAquatic;//checkplacecollisionaquatic - can spawn in water,brach,icth,mosa,fish

} TCharacter;



typedef struct tagPlayer
{
  BOOL Active;
  unsigned int IPaddr;
  Vector3d pos;
  float alpha, beta, vspeed;
  int kbState;
  char NickName[16];
} TPlayer;


typedef struct _TDemoPoint
{
  Vector3d pos;
  int DemoTime, CIndex;
} TDemoPoint;

typedef struct tagLevelDef
{
  char FileName[64];
  char MapName[128];
  DWORD DinosAvail;
  WORD *lpMapImage;
} TLevelDef;


typedef struct tagShipTask
{
  int tcount;
  int clist[255];
} TShipTask;

typedef struct tagShip
{
  Vector3d pos, rpos, tgpos, retpos;
  float alpha, tgalpha, speed, rspeed, DeltaY;
  int State, cindex, FTime;
} TShip;


typedef struct tagLandingList
{
  int PCount;
  Vector2di list[64];
} TLandingList;


typedef struct _TPlayerR
{
  char PName[128];
  int  RegNumber;
  int  Score, Rank;
} TPlayerR;

typedef struct _TTrophyItem
{
  int ctype, weapon, phase,
      height, weight, score,
      date, time;
  float scale, range;
  int r1, r2, r3, r4;
} TTrophyItem;


typedef struct _TStats
{
  int smade, success;
  float path, time;
} TStats;


typedef struct _TTrophyRoom
{
  char PlayerName[128];
  int  RegNumber;
  int  Score, Rank;

  TStats Last, Total;

  TTrophyItem Body[TROPHY_COUNT];
} TTrophyRoom;



typedef struct _TTrophyItem2 //Add neccesary stuff here! (later, not now)
{
	int ctype, weapon, phase,
		height, weight, score,
		date, time;
	float scale, range;
	int r1, r2, r3, r4;
} TTrophyItem2;



typedef struct _TTrophyRoom2
{
	int versionID;
	int survivalHighScore;
	TTrophyItem2 Body[TROPHY2_COUNT];
} TTrophyRoom2;


typedef struct _TDinoKill
{
	int anim;
	int offset;
	int hunteranim;
	int hunterswimanim;
	BOOL elevate, carryCorpse;
	BOOL dontloop;
	BOOL scream;
} TDinoKill;

typedef struct _TTrophyType
{
	int xoffset, yoffset, zoffset;
	int xoffsetScale, yoffsetScale, zoffsetScale;
	int xdata, ydata, zdata;
	int alpha, beta, gamma; //degrees
	int anim;
	int trophyPos;
	bool playAnim;
} TTrophyType;

typedef struct _TDinoDeathType
{
	int die;
	int sleep;
	int fall;
} TDinoDeathType;

typedef struct _TDinoIdleType
{
	int anim[32];
	int count;
	float start;
	float end;
	bool endOnAny;
	bool startOnAny;
	bool instantRepeat;
} TDinoIdleType;

typedef struct _TSpawnInfo
{
	int spawnGroup;//, spawnMax;
	float spawnRatio;
} TSpawnInfo;

typedef struct _TMenuDinoInfo
{
	TPicture CallIcon;
} TMenuDinoInfo;

typedef struct _TDinoInfo
{
	int menuDino = -1;

  char Name[48], FName[48], PName[48];
  int Health0, Clone;
  float Mass, Length, Radius,
        SmellK, HearK, LookK,
        ShDelta;
  int   Scale0, ScaleA, BaseScore;

  BOOL fearCall[64];
  BOOL Aquatic;
  int maxDepth, minDepth, spacingDepth;
  BOOL dontSwimAway;

  BOOL survivalDino;

  BOOL dontBend;
  //float bendOffset;

  float weaveRange;
  BOOL dontWeave;

  BOOL defensive;
  BOOL fearShot;
  BOOL fearHearShot;

  //BOOL noMoveNoRot;

  //int hunterDeathAnim, hunterDeathOffset;
  int aggress, killDist, flyDist;

  bool onRadar;
  float runspd, jmpspd, wlkspd, swmspd, flyspd, gldspd, tkfspd, lndspd, divspd;

  int maxGrad;
  float rotspdmulti;

  int packMax, packMin;
  float packDensity;

  int jumpRange;

  int runAnim, jumpAnim, walkAnim, swimAnim, flyAnim, diveAnim, glideAnim, takeoffAnim, landAnim,
	  slideAnim, shakeLandAnim, shakeWaterAnim, climbAnim;

  TDinoDeathType deathType[32];
  int deathTypeCount;

  TDinoKill killType[32];
  int killTypeCount;

  int tCounter;
  int trophyCode;
  int trophyLocTotal1;//CURRENTLY IN SAVE FILE
  int trophyLocTotal2;//CURRENTLY IN SESSION - REPLACE WITH tlt1 UPON RESTART
  
  TTrophyType trophyType[TROPHY2_COUNT];// big waste of memory! move out of the struct and replace this with a pointer array
  int trophyTypeCount;
//  int tCounter; // used to count off trophy locs

  int waterDieAnim[32];
  int waterDieCount;

  TDinoIdleType idleGroup[32];
  int idleGroupCount;

  TDinoIdleType idle2Group[32];
  int idle2GroupCount;

  
  int lookAnim[32];//trex look
  int lookCount;
  
  int smellAnim[32]; //icth wateridle   trex smell
  int smellCount;
  

  int roarAnim[32];
  int roarCount;
  
  bool canSwim;
  int waterLevel;

  BOOL dogSmell;

  bool trophySession;

  int partFrame1[50], partFrame2[50], partDist[50], partCnt[50], partMag[50], partOffset[50];
  bool partAngled[50], partCircle[50];

  bool DangerFish;
  bool TRexObjCollide;
  bool Mystery;
  bool HideBinoc;

  float camDemoPoint, camBase, camDemoPointWater, camBaseWater;

  float climbDist;

  byte radarRed, radarGreen, radarBlue, bloodRed, bloodGreen, bloodBlue;
  WORD radarColour565, radarColour555;


  int SpawnInfoCh;
  TSpawnInfo SpawnInfo[32];

} TDinoInfo;

typedef struct _TPack
{
	TCharacter *leader;
	bool alert;
	bool _alert;
	bool attack;
	bool _attack;
} TPack;

typedef struct _TAIInfo {
	float targetDistance;
	int noWayCntMin;
	int noFindWayMed;
	int noFindWayRange;
	float targetBendRotSpd;
	float targetBendMin;
	float targetBendDelta1;
	float targetBendDelta2;
	float walkTargetGammaRot;
	float targetGammaRot;
	int idleStart;
	float yBetaGamma1, yBetaGamma2, yBetaGamma3, yBetaGamma4;

	int agressMulti;
	float tGAIncrement;
	int idleStartD;
	bool jumper;

	bool iceAge;
	bool carnivore;

	float rot1, rot2, pWMin; //weaveRange

	bool sniffer;

} TAIInfo;

typedef struct _TSpawnRegion
{
	int XMax, YMax, XMin, YMin;
} TSpawnRegion;

typedef struct _TSpawnGroup
{
	int SpawnMax, SpawnMin;
	float SpawnRate;
	bool moveForward, Randomised, OnlyActiveNearby, stayInRegion;
	int densityMulti;

	int spawnRegionCh, avoidRegionCh;
	TSpawnRegion spawnRegion[16];
	TSpawnRegion avoidRegion[16];

	int dinoIndexCh;
	int dinoIndex[128];
	int spawnInfoIndex[128];
} TSpawnGroup;


typedef struct _TWeapInfo
{
  char Name[48], FName[48], BFName[48], SFXName[48];
  bool MGSSound = FALSE;
  float Power, Prec, Loud, Rate;
  int Shots, Optic, Fall, TraceC, Reload, SFXIndex;
} TWeapInfo;


typedef struct _TFogEntity
{
  int fogRGB;
  float YBegin;
  BOOL  Mortal;
  float Transp, FLimit;
} TFogEntity;


typedef struct _TWaterEntity
{
  int tindex, wlevel;
  float transp;
  int fogRGB;
} TWaterEntity;


typedef struct _TWind
{
  float alpha;
  float speed;
  Vector3d nv;
} TWind;




typedef struct _TElement
{
  Vector3d pos, speed;
  int     Flags;
  float   R;
} TElement;

typedef struct _TElements
{
  int Type, ECount, EDone, LifeTime;
  int Param1, Param2, Param3;
  DWORD RGBA, RGBA2;
  Vector3d pos;
  TElement EList[32];
} TElements;


typedef struct _TBloodP
{
  int LTime;
  Vector3d pos;
  int Owner;
} TBloodP;

typedef struct _TBTrail
{
  int Count;
  TBloodP Trail[512];
} TBTrail;


//============= functions ==========================//

void HLineTxB( void );
void HLineTxC( void );
void HLineTxGOURAUD( void );


void HLineTxModel25( void );
void HLineTxModel75( void );
void HLineTxModel50( void );

void HLineTxModel3( void );
void HLineTxModel2( void );
void HLineTxModel( void );

void HLineTDGlass75( void );
void HLineTDGlass50( void );
void HLineTDGlass25( void );
void HLineTBGlass25( void );


void SetVideoMode(int, int);

void CreateDivTable();
void DrawTexturedFace();
int GetTextW(HDC, LPSTR);
void wait_mouse_release();

//============================== render =================================//
void ShowControlElements();
void InsertModelList(TModel* mptr, float x0, float y0, float z0, int light, float al, float bt);
void RenderGround();
void RenderWater();
void RenderElements();
void CreateChRenderList();
void RenderModelsList();
void ProcessMap  (int x, int y, int r);
void ProcessMap2 (int x, int y, int r);
void ProcessMapW (int x, int y, int r);
void ProcessMapW2(int x, int y, int r);

void DrawTPlane(BOOL);
void DrawTPlaneClip(BOOL);
void ClearVideoBuf();
void DrawTrophyText(int, int);
void DrawSurvivalText(int, int);
void DrawHMap();
void RenderCharacter(TCharacter*);
void RenderShip();
void RenderPlayer(int);
void RenderSkyPlane();
void RenderHealthBar();
void Render_Cross(int, int);
void Render_LifeInfo(int);

void RenderModelClipEnvMap(TModel*, float, float, float, float, float);
void RenderModelClipPhongMap(TModel*, float, float, float, float, float);

void RenderModel         (TModel*, float, float, float, int, int, float, float);
void RenderBMPModel      (TBMPModel*, float, float, float, int);
void RenderModelClipWater(TModel*, float, float, float, int, int, float, float);
void RenderModelClip     (TModel*, float, float, float, int, int, float, float);
void RenderNearModel     (TModel*, float, float, float, int, float, float);
void DrawPicture         (int x, int y, TPicture &pic);

void InitClips();
void InitDirectDraw();
void WaitRetrace();

//============= Characters =======================
void Characters_AddSecondaryOne(TCharacter *cptr);
void AddDeadBody(TCharacter *cptr, int, bool);
void PlaceCharacters();
void PlaceCharactersSurvival();
void PlaceMHunters(); //multiplayer
void PlaceTrophy();
void AnimateCharacters();
void AnimateMHunters(); //multiplayer
void MakeNoise(Vector3d, float);
void CheckAfraid();
void CreateChMorphedModel(TCharacter* cptr);
void CreateMorphedObject(TModel* mptr, TVTL &vtl, int FTime);
void CreateMorphedModel(TModel* mptr, TAni *aptr, int FTime, float scale);

//=============================== Math ==================================//

void CalcLights  (TModel* mptr);
void CalcModelGroundLight(TModel *mptr, float x0, float z0, int FI);
void CalcNormals (TModel* mptr, Vector3d *nvs);
void CalcGouraud (TModel* mptr, Vector3d *nvs);

void CalcPhongMapping(TModel* mptr, Vector3d *nv);
void CalcEnvMapping(TModel* mptr, Vector3d *nv);

void CalcBoundBox(TModel* mptr, TBound *bound);
void  NormVector(Vector3d&, float);
float SGN(float);
void  DeltaFunc(float &a, float b, float d);
void  MulVectorsScal(const Vector3d&, const Vector3d&, float&);
void  MulVectorsVect(const Vector3d&, const Vector3d&, Vector3d&);
Vector3d SubVectors( Vector3d&, Vector3d& );
Vector3d AddVectors( Vector3d&, Vector3d& );
Vector3d RotateVector(Vector3d&);
float VectorLength(Vector3d);
int   siRand(int);
int   rRand(int);
void  CalcHitPoint(CLIPPLANE&, Vector3d&, Vector3d&, Vector3d&);
void  ClipVector(CLIPPLANE& C, int vn);
float FindVectorAlpha(float, float);
float AngleDifference(float a, float b);

int   TraceShot(float ax, float ay, float az,
                float &bx, float &by, float &bz);
int   TraceLook(float ax, float ay, float az,
                float bx, float by, float bz);


void CheckCollision(float&, float&);
float CalcFogLevel(Vector3d v);
//=================================================================//
void AddMessage(LPSTR mt);
void CreateTMap();


void LoadSky();
void LoadSkyMap();
void LoadTexture(TEXTURE*&);
void LoadWav(char* FName, TSFX &sfx);


void ApplyAlphaFlags(WORD*, int);
WORD conv_565(WORD c);
int  conv_xGx(int);
void conv_pic(TPicture &pic);
void LoadPicture(TPicture &pic, LPSTR pname);
void LoadPictureTGA(TPicture &pic, LPSTR pname);
void LoadCharacterInfo(TCharacterInfo&, char*);
void LoadModelEx(TModel* &mptr, char* FName);
void LoadModel(TModel*&);
void LoadResources();
void ReInitGame();


void SaveScreenShot();
void CreateWaterTab();
void CreateFadeTab();
void CreateVideoDIB();
void RenderLightMap();

void MulVectorsVect(const Vector3d& v1, const Vector3d& v2, Vector3d& r );
void MulVectorsScal(const Vector3d& v1, const Vector3d& v2, float& r);
Vector3d SubVectors( Vector3d& v1, Vector3d& v2 );
void NormVector(Vector3d& v, float Scale);

LPVOID _HeapAlloc(HANDLE hHeap, DWORD dwFlags, DWORD dwBytes);
BOOL _HeapFree(HANDLE hHeap, DWORD  dwFlags, LPVOID lpMem);

//============ game ===========================//
float GetLandCeilH(float, float);
float GetLandH(float, float);
float GetLandOH(int, int);
float GetLandLt(float, float);
float GetLandUpH(float, float);
float GetLandQH(float, float);
float GetLandQHNoObj(float, float);
float GetLandHObj(float, float);
bool waterNear(float, float, float);

void LoadResourcesScript();
void InitEngine();
void ShutDownServer();
void ShutDownClient();
void ShutDownEngine();
void ProcessSyncro();
void AddShipTask(int);
void LoadTrophy();
//void LoadPlayersInfo();
void SaveTrophy();
void RemoveCurrentTrophy();
void MakeCall();
void MakeShot(float ax, float ay, float az,
              float bx, float by, float bz);
void registerDamage(int);

void AddBloodTrail(TCharacter *cptr);
void AddElements(float, float, float, int, int);
void AddElementsA(float, float, float, int, int, int, bool, float);
void AddWCircle(float, float, float);
void AnimateProcesses();
void DoHalt(LPSTR);
void DoHalt2(LPSTR);

_EXTORNOT   char logt[128];
void CreateLog();
void PrintLog(LPSTR l);
void CloseLog();

_EXTORNOT   float BackViewR;
_EXTORNOT   int   BackViewRR;
_EXTORNOT   int   UnderWaterT;
_EXTORNOT   int   TotalTreeTable, TotalAreaInfo, TotalSpawnGroup, TotalC, TotalW, TotalMA, TotalTrophy;// , TotalRegion, TotalAvoid;


//========== multiplayer =============//

_EXTORNOT   char    ServerAddress[128];
_EXTORNOT   WSADATA wsaData;
_EXTORNOT   int iResult;
_EXTORNOT   SOCKET ListenSocket;
_EXTORNOT   SOCKET ClientSocket;
_EXTORNOT   SOCKET ConnectSocket;

_EXTORNOT   struct addrinfo *result;
_EXTORNOT   struct addrinfo hints;

_EXTORNOT   int iSendResult;

_EXTORNOT   HANDLE CommsThreadHandle;
_EXTORNOT   LPDWORD CommsThreadID;
_EXTORNOT   BOOL HaltThread;

_EXTORNOT   char recvbuf[DEFAULT_BUFLEN];
_EXTORNOT   int recvbuflen;

void StartupServerCommsThread();
void StartupClientCommsThread();




//========== common ==================//
_EXTORNOT   HWND    hwndMain;
_EXTORNOT   HINSTANCE  hInst;
_EXTORNOT   HANDLE  Heap;
_EXTORNOT   HDC     hdcMain, hdcCMain;
_EXTORNOT   BOOL    blActive;
_EXTORNOT   BYTE    KeyboardState[256];
_EXTORNOT   int     KeyFlags, _shotcounter;

_EXTORNOT   TMessageList MessageList;
_EXTORNOT   char    ProjectName[128];

_EXTORNOT   int     _GameState, _MultiplayerState;//multiplayer
_EXTORNOT   TSFX    fxBlip;
_EXTORNOT   TSFX    fxCall[10][3], fxScream[4];
_EXTORNOT   TSFX	fxGunShot[11];
_EXTORNOT   TSFX    fxUnderwater, fxWaterIn, fxWaterOut, fxJump, fxStep[3], fxStepW[3];
//========== map =====================//
_EXTORNOT   byte HMap[ctMapSize][ctMapSize];
_EXTORNOT   byte WMap[ctMapSize][ctMapSize];
_EXTORNOT   byte HMapO[ctMapSize][ctMapSize];
_EXTORNOT   WORD FMap[ctMapSize][ctMapSize];
_EXTORNOT   byte LMap[ctMapSize][ctMapSize];
_EXTORNOT   WORD TMap1[ctMapSize][ctMapSize];
_EXTORNOT   WORD TMap2[ctMapSize][ctMapSize];
_EXTORNOT   byte OMap[ctMapSize][ctMapSize];

_EXTORNOT   byte FogsMap[512][512];
_EXTORNOT   byte AmbMap[512][512];

_EXTORNOT   TFogEntity    FogsList[256];
_EXTORNOT   TWaterEntity  WaterList[256];
_EXTORNOT   TWind       Wind;
_EXTORNOT   TShip       Ship;
_EXTORNOT   TShipTask   ShipTask;

_EXTORNOT   int SkyR, SkyG, SkyB, WaterR, WaterG, WaterB, WaterA,
            SkyTR,SkyTG,SkyTB, CurFogColor;
_EXTORNOT   int RandomMap[32][32];

_EXTORNOT   Vector2df *PhongMapping;
_EXTORNOT   TPicture TFX_SPECULAR, TFX_ENVMAP;
_EXTORNOT   WORD SkyPic[256*256];
_EXTORNOT   WORD SkyFade[9][128*128];
_EXTORNOT   BYTE SkyMap[128*128];

_EXTORNOT   TEXTURE* Textures[1024];
_EXTORNOT   TAmbient Ambient[256];
_EXTORNOT   TSFX     RandSound[256];

//========= WEATHER =================//

_EXTORNOT BOOL SNOW;
_EXTORNOT int snow_vSpd;//vertical
_EXTORNOT int snow_hSpd;//horizontal
_EXTORNOT int snow_dens;//density

_EXTORNOT byte snow1_r, snow1_g, snow1_b, snow1_a;//density
_EXTORNOT float snow1_rad;//radius

_EXTORNOT BOOL SNOW2;
_EXTORNOT byte snow2_r, snow2_g, snow2_b, snow2_a;//density
_EXTORNOT float snow2_rad;//radius

//========= GAME ====================//
_EXTORNOT int TargetDino, TargetArea, TargetWeapon, WeaponPres, TargetCall,
          TrophyTime, ObservMode, Tranq, ObjectsOnLook,
          CurrentWeapon, ShotsLeft[10], AmmoMag[10];

_EXTORNOT Vector3d answpos;
_EXTORNOT int answtime, answcall;

_EXTORNOT BOOL ScentMode, CamoMode,
          RadarMode, LockLanding,
          TrophyMode, DoubleAmmo,
          DogMode, Multiplayer,
          Host, CiskMode, SonarMode,
          ScannerMode, SurvivalMode;

_EXTORNOT float sonarPos;

_EXTORNOT TTrophyRoom TrophyRoom;
_EXTORNOT TTrophyRoom2 TrophyRoom2;
//_EXTORNOT TPlayerR PlayerR[16];
_EXTORNOT TPicture LandPic,DinoPic,DinoPicM, MapPic, WepPic;
_EXTORNOT HFONT fnt_BIG, fnt_Small, fnt_Midd;
_EXTORNOT TLandingList LandingList;

//======== MODEL ======================//
_EXTORNOT TObject  MObjects[256];
_EXTORNOT TModel* mptr;
_EXTORNOT TWeapon Weapon;


_EXTORNOT int   OCount, iModelFade, iModelBaseFade, Current;
_EXTORNOT Vector3d  *rVertex;
_EXTORNOT TObj      gObj[1024];
_EXTORNOT Vector2di *gScrp;

_EXTORNOT int MaxObjectVCount; // Maximum VCount of any (loaded) object

//============= Characters ==============//
_EXTORNOT TPicture  PausePic, ExitPic, TrophyExit, TrophyPic;
_EXTORNOT TModel *SunModel;
_EXTORNOT TCharacterInfo WCircleModel;
_EXTORNOT TModel *CompasModel;
_EXTORNOT TModel *Binocular;
_EXTORNOT TDinoInfo DinoInfo[DINOINFO_MAX];
_EXTORNOT TMenuDinoInfo MenuDinoInfo[16];
_EXTORNOT int sendGunShot;
_EXTORNOT int mGunShot[4];
_EXTORNOT int sendHunterCall;
_EXTORNOT int sendHunterCallType;
_EXTORNOT int mHunterCall[4];
_EXTORNOT int mHunterCallType[4];
_EXTORNOT int sendDamage[DINOINFO_MAX];
_EXTORNOT int mDamage[4][DINOINFO_MAX];
//Add these after dino positions alligned
//_EXTORNOT int sendDinoCall;
//_EXTORNOT int mDinoCall[4];
_EXTORNOT bool TreeTable[255];
_EXTORNOT TAIInfo AIInfo[DINOINFO_MAX];
//_EXTORNOT TRegion Region[256];
//_EXTORNOT TRegion Avoid[256];
_EXTORNOT TWeapInfo WeapInfo[10];
_EXTORNOT TCharacterInfo ShipModel;
_EXTORNOT TSpawnGroup spawnGroup[64];
//_EXTORNOT int AI_to_CIndex[DINOINFO_MAX];
_EXTORNOT int TrophyIndex[DINOINFO_MAX];
_EXTORNOT int ChCount, WCCount, ElCount, SnCount,
          ShotDino, TrophyBody, HunterCount; //HunterCount is for multiplayer, up to 3 others
_EXTORNOT bool TrophyDisplay;
_EXTORNOT TTrophyItem TrophyDisplayBody;
_EXTORNOT TCharacterInfo WindModel;
_EXTORNOT TCharacterInfo PlayerInfo;
_EXTORNOT TCharacterInfo ChInfo[DINOINFO_MAX];
_EXTORNOT TCharacterInfo MPlayerInfo[3]; //multiplayer
_EXTORNOT TPack          Packs[256];
_EXTORNOT int PackCount;
_EXTORNOT TCharacter     Characters[256];
_EXTORNOT TCharacter     MPlayers[3]; //multiplayer

_EXTORNOT int SurvivalSpawnX; //survival
_EXTORNOT int SurvivalSpawnZ;
_EXTORNOT float SurvivalSpawnA;
_EXTORNOT TSpawnRegion SurvivalDinoSpawn; //dino spawn zone
_EXTORNOT int SurvivalWave;
_EXTORNOT int SurvivalIndex[128];
_EXTORNOT int SurvivalIndexCh;



_EXTORNOT TWCircle       WCircles[2096]; //increased

_EXTORNOT TSnowElement   Snow[16048];

_EXTORNOT TDemoPoint     DemoPoint;
_EXTORNOT TCharacter     *killerDino;
_EXTORNOT BOOL			 killedwater;

_EXTORNOT TPlayer        Players[16];
_EXTORNOT Vector3d       PlayerPos, CameraPos;

//========== Render ==================//
_EXTORNOT   LPDIRECTDRAW lpDD;
_EXTORNOT   LPDIRECTDRAW2 lpDD2;
//_EXTORNOT   LPDIRECTINPUT lpDI;

_EXTORNOT   void* lpVideoRAM;
_EXTORNOT   LPDIRECTDRAWSURFACE lpddsPrimary;
_EXTORNOT   BOOL DirectActive, RestartMode;
_EXTORNOT   BOOL LoDetailSky;
_EXTORNOT   int  WinW,WinH,WinEX,WinEY,VideoCX,VideoCY,iBytesPerLine,ts,r,MapMinY;
_EXTORNOT   float CameraW,CameraH,Soft_Persp_K, stepdy, stepdd, SunShadowK, FOVK;
_EXTORNOT   CLIPPLANE ClipA,ClipB,ClipC,ClipD,ClipZ,ClipW;
_EXTORNOT   int u,vused, CCX, CCY;

_EXTORNOT   DWORD Mask1,Mask2;
_EXTORNOT   DWORD HeapAllocated, HeapReleased;


_EXTORNOT   EPoint VMap[256][256];
_EXTORNOT   EPoint VMap2[256][256];
_EXTORNOT   EPoint ev[3];

_EXTORNOT   ClipPoint cp[16];
_EXTORNOT   ClipPoint hleft,hright;


_EXTORNOT   void  *HLineT;
_EXTORNOT   int   rTColor;
_EXTORNOT   int   SKYMin, SKYDTime, GlassL, ctViewR, ctViewR1, ctViewRM,
            dFacesCount, ReverseOn, TDirection;
_EXTORNOT   WORD  FadeTab[65][0x8000];
_EXTORNOT   TElements Elements[700];
_EXTORNOT   TBTrail   BloodTrail;

_EXTORNOT   int     PrevTime, TimeDt, T, Takt, RealTime, StepTime, MyHealth, ExitTime, WaveNoteTime,
            ChCallTime, CallLockTime, NextCall;
_EXTORNOT   float   DeltaT;
_EXTORNOT   float   CameraX, CameraY, CameraZ, CameraAlpha, CameraBeta;
_EXTORNOT   float   PlayerX, PlayerY, PlayerZ, PlayerAlpha, PlayerBeta,
            HeadY, HeadBackR, HeadBSpeed, HeadAlpha, HeadBeta,
            SSpeed,VSpeed,RSpeed,YSpeed;
_EXTORNOT   Vector3d PlayerNv;

_EXTORNOT   float   ca,sa,cb,sb, wpnDAlpha, wpnDBeta;
_EXTORNOT   void    *lpVideoBuf, *lpTextureAddr;
_EXTORNOT   HBITMAP hbmpVideoBuf;
_EXTORNOT   HCURSOR hcArrow;
_EXTORNOT   int     DivTbl[10240];

_EXTORNOT   Vector3d  v[3];
_EXTORNOT   ScrPoint  scrp[3];
_EXTORNOT   MScrPoint mscrp[3];
_EXTORNOT   Vector3d  nv, waterclipbase, Sun3dPos;


_EXTORNOT   struct _t
{
  int fkForward, fkBackward, fkUp, fkDown, fkLeft, fkRight, fkFire, fkShow, fkSLeft, fkSRight, fkStrafe, fkJump, fkRun, fkCrouch, fkCall, fkCCall, fkBinoc;
} KeyMap;


#define kfForward     0x00000001
#define kfBackward    0x00000002
#define kfLeft        0x00000004
#define kfRight       0x00000008
#define kfLookUp      0x00000010
#define kfLookDn      0x00000020
#define kfJump        0x00000040
#define kfDown        0x00000080
#define kfCall        0x00000100

#define kfSLeft       0x00001000
#define kfSRight      0x00002000
#define kfStrafe      0x00004000

#define fmWater   0x0080
#define fmWater2  0x8000
#define fmNOWAY   0x0020
#define fmReverse 0x0010

#define fmWaterA  0x8080


#define tresGround 1
#define tresWater  2
#define tresModel  3
#define tresChar   4

#define sfDoubleSide         1
#define sfDarkBack           2
#define sfOpacity            4
#define sfTransparent        8
#define sfMortal        0x0010
#define sfPhong         0x0030
#define sfEnvMap        0x0050

#define sfNeedVC        0x0080
#define sfDark          0x8000

#define ofPLACEWATER       1
#define ofPLACEGROUND      2
#define ofPLACEUSER        4
#define ofCIRCLE           8
#define ofBOUND            16
#define ofNOBMP            32
#define ofNOLIGHT          64
#define ofDEFLIGHT         128
#define ofGRNDLIGHT        256
#define ofNOSOFT           512
#define ofNOSOFT2          1024
#define ofANIMATED         0x80000000

#define csONWATER          0x00010000
#define MAX_HEALTH         128000

#define HUNT_EAT      0
#define HUNT_BREATH   1
#define HUNT_FALL     2
#define HUNT_KILL     3





#define AI_MOSH       1
#define AI_GALL       2
#define AI_DIMOR      3
#define AI_PTERA      4
#define AI_DIMET      5
#define AI_PIG        6

#define AI_HUNTDOG    9

#define AI_PARA       10
#define AI_ANKY       11
#define AI_STEGO      12
#define AI_ALLO       13
#define AI_CHASM      14
#define AI_VELO       15
#define AI_SPINO      16
#define AI_CERAT      17
#define AI_TREX       18


#define AI_PACH       19

#define AI_BRONT      20
#define AI_HOG        21
#define AI_WOLF       22
#define AI_RHINO      23
#define AI_DEER       24
#define AI_SMILO      25
#define AI_MAMM       26
#define AI_BEAR       27

#define AI_TITAN      28
#define AI_MICRO      29


#define AI_BRACH       30
#define AI_ICTH        31
#define AI_FISH        32
#define AI_MOSA        33
#define AI_BRACHDANGER 34
#define AI_LANDBRACH   35

//#define AI_FINAL	  29 //Last AI of max huntable roster (menu can only display 10)

//#define AI_POACHER    22












_EXTORNOT BOOL WATERANI,Clouds,SKY,GOURAUD,
          MODELS,TIMER,BITMAPP,MIPMAP,
          NOCLIP,CLIP3D,NODARKBACK,CORRECTION, LOWRESTX,
          FOGENABLE, FOGON, CAMERAINFOG,
          WATERREVERSE,waterclip,UNDERWATER, ONWATER, NeedWater,
          SWIM, FLY, PAUSE, OPTICMODE, BINMODE, EXITMODE, MapMode, RunMode, CrouchMode;
_EXTORNOT int  CameraFogI;
_EXTORNOT int OptDayNight, OptAgres, OptDens, OptSens, OptRes, OptViewR,
          OptMsSens, OptBrightness, OptSound, OptRender,
          OptText, OptSys, WaitKey, OPT_ALPHA_COLORKEY;
_EXTORNOT BOOL SHADOWS3D,REVERSEMS;

_EXTORNOT BOOL SLOW, DEBUG, MORPHP, MORPHA;
_EXTORNOT HANDLE hlog;


//========== for audio ==============//
void  AddVoicev  (int, short int*, int);
void  AddVoice3dv(int, short int*, float, float, float, int);
void  AddVoice3d (int, short int*, float, float, float);

void SetAmbient3d(int, short int*, float, float, float);
void SetAmbient(int, short int*, int);
void AudioSetCameraPos(float, float, float, float, float);
void InitAudioSystem(HWND, HANDLE, int);
void Audio_Restore();
void AudioStop();
void Audio_Shutdown();
void Audio_SetEnvironment(int, float);
void Audio_UploadGeometry();
//=================================
typedef struct tagAudioQuad
{
  float x1,y1,z1;
  float x2,y2,z2;
  float x3,y3,z3;
  float x4,y4,z4;
} AudioQuad;
_EXTORNOT int AudioFCount;
_EXTORNOT AudioQuad data[8192];
_EXTORNOT void UploadGeometry();
_EXTORNOT int Env;

//========== for 3d hardware =============//
_EXTORNOT BOOL HARD3D;
void ShowVideo();
void Init3DHardware();
void Activate3DHardware();
void ShutDown3DHardware();
void Render3DHardwarePosts();
void CopyBackToDIB();
void CopyHARDToDIB();
void Hardware_ZBuffer(BOOL zb);
void AllocateRenderTables(void);

//=========== loading =============
void StartLoading();
void EndLoading();
void PrintLoad(char *t);

#ifdef _MAIN_
_EXTORNOT char KeysName[256][24] =
{
  "...",
  "Esc",
  "1",
  "2",
  "3",
  "4",
  "5",
  "6",
  "7",
  "8",
  "9",
  "0",
  "-",
  "=",
  "BSpace",
  "Tab",
  "Q",
  "W",
  "E",
  "R",
  "T",
  "Y",
  "U",
  "I",
  "O",
  "P",
  "[",
  "]",
  "Enter",
  "Ctrl",
  "A",
  "S",
  "D",
  "F",
  "G",
  "H",
  "J",
  "K",
  "L",
  ";",
  "'",
  "~",
  "Shift",
  "\\",
  "Z",
  "X",
  "C",
  "V",
  "B",
  "N",
  "M",
  ",",
  ".",
  "/",
  "Shift",
  "*",
  "Alt",
  "Space",
  "CLock",
  "F1",
  "F2",
  "F3",
  "F4",
  "F5",
  "F6",
  "F7",
  "F8",
  "F9",
  "F10",
  "NLock",
  "SLock",
  "Home",
  "Up",
  "PgUp",
  "-",
  "Left",
  "Midle",
  "Right",
  "+",
  "End",
  "Down",
  "PgDn",
  "Ins",
  "Del",
  "",
  "",
  "",
  "F11",
  "F12",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "Mouse1",
  "Mouse2",
  "Mouse3",
  "<?>",
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""
};
#else
_EXTORNOT char KeysName[128][24];
#endif