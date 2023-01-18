#include "Hunt.h"


#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <timeapi.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "1986"




/*typedef struct tagAudioQuad
{
  float x1,y1,z1;
  float x2,y2,z2;
  float x3,y3,z3;
  float x4,y4,z4;
} AudioQuad;

AudioQuad data[8192];

  HMap[1024][1024];
*/


const int bufSizeMax = 189;
const int bufSizeHostInit = 189;
const int bufSizeHost = 153;
const int bufSizeClient = 31;

bool ShowFaces = true;

void UploadGeometry()
{
  int x,y,xx,yy;
  byte temp;

  AudioFCount = 0;

  int MaxView = 18;
  int HalfView = (int)(MaxView/2)+1;

  for (x = 0; x < MaxView; x++)
    for (y = 0; y < MaxView; y++)
    {
      xx = (x - HalfView)*2;
      yy = (y - HalfView)*2;
      data[AudioFCount].x1 = (CCX+xx) * 256 - CameraX;
      data[AudioFCount].y1 = HMap[CCY+yy][CCX+xx] * ctHScale - CameraY;
      data[AudioFCount].z1 = (CCY+yy) * 256 - CameraZ;

      xx = ((x+1) - HalfView)*2;
      yy = (y - HalfView)*2;
      data[AudioFCount].x2 = (CCX+xx) * 256 - CameraX;
      data[AudioFCount].y2 = HMap[CCY+yy][CCX+xx] * ctHScale - CameraY;
      data[AudioFCount].z2 = (CCY+yy) * 256 - CameraZ;

      xx = ((x+1) - HalfView)*2;
      yy = ((y+1) - HalfView)*2;
      data[AudioFCount].x3 = (CCX+xx) * 256 - CameraX;
      data[AudioFCount].y3 = HMap[CCY+yy][CCX+xx] * ctHScale - CameraY;
      data[AudioFCount].z3 = (CCY+yy) * 256 - CameraZ;

      xx = (x - HalfView)*2;
      yy = ((y+1) - HalfView)*2;
      data[AudioFCount].x4 = (CCX+xx) * 256 - CameraX;
      data[AudioFCount].y4 = HMap[CCY+yy][CCX+xx] * ctHScale - CameraY;
      data[AudioFCount].z4 = (CCY+yy) * 256 - CameraZ;

      AudioFCount++;
    }

//     MessageBeep(-1);

  if (ShowFaces)
  {
    wsprintf(logt,"Audio_UpdateGeometry: %i faces uploaded\n", AudioFCount);
    PrintLog(logt);

    ShowFaces = false;
  }
}

void SetupRes()
{
  if (!HARD3D)
    if (OptRes>5) OptRes=5;
  if (OptRes==0)
  {
    WinW = 320;
    WinH=240;
  }
  if (OptRes==1)
  {
    WinW = 400;
    WinH=300;
  }
  if (OptRes==2)
  {
    WinW = 512;
    WinH=384;
  }
  if (OptRes==3)
  {
    WinW = 640;
    WinH=480;
  }
  if (OptRes==4)
  {
    WinW = 800;
    WinH=600;
  }
  if (OptRes==5)
  {
    WinW =1024;
    WinH=768;
  }
  if (OptRes==6)
  {
    WinW =1280;
    WinH=1024;
  }
  if (OptRes==7)
  {
    WinW =1600;
    WinH=1200;
  }
}


float GetLandOH(int x, int y)
{
  return (float)(HMapO[y][x]) * ctHScale;
}


float GetLandOUH(int x, int y)
{
  if (FMap[y][x] & fmReverse)
    return (float)((int)(HMap[y][x+1]+HMap[y+1][x])/2.f)*ctHScale;
  else
    return (float)((int)(HMap[y][x]+HMap[y+1][x+1])/2.f)*ctHScale;
}



float GetLandUpH(float x, float y)
{

  int CX = (int)x / 256;
  int CY = (int)y / 256;

  if (!(FMap[CY][CX] & fmWaterA)) return GetLandH(x,y);

  return (float)(WaterList[ WMap[CY][CX] ].wlevel * ctHScale);

}

bool waterNear(float x, float y, float maxDist)
{
	for (int varX = -maxDist; varX <= maxDist; varX += maxDist) {
		for (int varY = -maxDist; varY <= maxDist; varY += maxDist) {
			if ((FMap[varY / 256][varX / 256] & fmWaterA)) return true;

		}
	}
	//if ((FMap[(int)y / 256][(int)x / 256] & fmWaterA)) return true;
	return false;
}


float GetLandH(float x, float y)
{
  int CX = (int)x / 256;
  int CY = (int)y / 256;

  int dx = (int)x % 256;
  int dy = (int)y % 256;

  int h1 = HMap[CY][CX];
  int h2 = HMap[CY][CX+1];
  int h3 = HMap[CY+1][CX+1];
  int h4 = HMap[CY+1][CX];


  if (FMap[CY][CX] & fmReverse)
  {
    if (256-dx>dy) h3 = h2+h4-h1;
    else h1 = h2+h4-h3;
  }
  else
  {
    if (dx>dy) h4 = h1+h3-h2;
    else h2 = h1+h3-h4;
  }

  float h = (float)
            (h1   * (256-dx) + h2 * dx) * (256-dy) +
            (h4   * (256-dx) + h3 * dx) * dy;

  return  (h / 256.f / 256.f) * ctHScale;
}



float GetLandLt(float x, float y)
{
  int CX = (int)x / 256;
  int CY = (int)y / 256;

  int dx = (int)x % 256;
  int dy = (int)y % 256;

  int h1 = LMap[CY][CX];
  int h2 = LMap[CY][CX+1];
  int h3 = LMap[CY+1][CX+1];
  int h4 = LMap[CY+1][CX];

  float h = (float)
            (h1   * (256-dx) + h2 * dx) * (256-dy) +
            (h4   * (256-dx) + h3 * dx) * dy;

  return  (h / 256.f / 256.f);
}



float GetLandLt2(float x, float y)
{
  int CX = ((int)x / 512)*2 - CCX;
  int CY = ((int)y / 512)*2 - CCY;

  int dx = (int)x % 512;
  int dy = (int)y % 512;

  int h1 = VMap[CY+128][CX+128].Light;
  int h2 = VMap[CY+128][CX+2+128].Light;
  int h3 = VMap[CY+2+128][CX+2+128].Light;
  int h4 = VMap[CY+2+128][CX+128].Light;

  float h = (float)
            (h1   * (512-dx) + h2 * dx) * (512-dy) +
            (h4   * (512-dx) + h3 * dx) * dy;

  return  (h / 512.f / 512.f);
}



void CalcModelGroundLight(TModel *mptr, float x0, float z0, int FI)
{
  float ca = cos(FI * pi / 2);
  float sa = sin(FI * pi / 2);
  for (int v=0; v<mptr->VCount; v++)
  {
    float x = mptr->gVertex[v].x * ca + mptr->gVertex[v].z * sa + x0;
    float z = mptr->gVertex[v].z * ca - mptr->gVertex[v].x * sa + z0;
    mptr->VLight[0][v] = GetLandLt2(x, z) - 128;
  }
}


BOOL PointOnBound(float &H, float px, float py, float cx, float cy, float oy, TBound *bound, int angle)
{
  px-=cx;
  py-=cy;

  float ca = (float) cos(angle*pi / 2.f);
  float sa = (float) sin(angle*pi / 2.f);

  BOOL _on = FALSE;
  H=-1000;

  for (int o=0; o<8; o++)
  {

    if (bound[o].a<0) continue;
    if (bound[o].y2 + oy > PlayerY + 128) continue;

    float a,b;
    float ccx = bound[o].cx*ca + bound[o].cy*sa;
    float ccy = bound[o].cy*ca - bound[o].cx*sa;

    if (angle & 1)
    {
      a = bound[o].b;
      b = bound[o].a;
    }
    else
    {
      a = bound[o].a;
      b = bound[o].b;
    }

    if ( ( fabs(px - ccx) < a) &&  (fabs(py - ccy) < b) )
    {
      _on=TRUE;
      if (H < bound[o].y2) H = bound[o].y2;
    }
  }

  return _on;
}



BOOL PointUnBound(float &H, float px, float py, float cx, float cy, float oy, TBound *bound, int angle)
{
  px-=cx;
  py-=cy;

  float ca = (float) cos(angle*pi / 2.f);
  float sa = (float) sin(angle*pi / 2.f);

  BOOL _on = FALSE;
  H=+1000;

  for (int o=0; o<8; o++)
  {

    if (bound[o].a<0) continue;
    if (bound[o].y1 + oy < PlayerY + 128) continue;

    float a,b;
    float ccx = bound[o].cx*ca + bound[o].cy*sa;
    float ccy = bound[o].cy*ca - bound[o].cx*sa;

    if (angle & 1)
    {
      a = bound[o].b;
      b = bound[o].a;
    }
    else
    {
      a = bound[o].a;
      b = bound[o].b;
    }

    if ( ( fabs(px - ccx) < a) &&  (fabs(py - ccy) < b) )
    {
      _on=TRUE;
      if (H > bound[o].y1) H = bound[o].y1;
    }
  }

  return _on;
}





float GetLandCeilH(float CameraX, float CameraZ)
{
  float h,hh;

  h = GetLandH(CameraX, CameraZ) + 20480;

  int ccx = (int)CameraX / 256;
  int ccz = (int)CameraZ / 256;

  for (int z=-4; z<=4; z++)
    for (int x=-4; x<=4; x++)
      if (OMap[ccz+z][ccx+x]!=255)
      {
        int ob = OMap[ccz+z][ccx+x];

        float CR = (float)MObjects[ob].info.Radius - 1.f;

        float oz = (ccz+z) * 256.f + 128.f;
        float ox = (ccx+x) * 256.f + 128.f;

        float LandY = GetLandOH(ccx+x, ccz+z);

        if (!(MObjects[ob].info.flags & ofBOUND))
        {
          if (MObjects[ob].info.YLo + LandY > h) continue;
          if (MObjects[ob].info.YLo + LandY < PlayerY+100) continue;
        }

        float r = CR+1;

        if (MObjects[ob].info.flags & ofBOUND)
        {
          float hh;
          if (PointUnBound(hh, CameraX, CameraZ, ox, oz, LandY, MObjects[ob].bound, ((FMap[ccz+z][ccx+x] >> 2) & 3)  ) )
            if (h > LandY + hh) h = LandY + hh;
        }
        else
        {
          if (MObjects[ob].info.flags & ofCIRCLE)
            r = (float) sqrt( (ox-CameraX)*(ox-CameraX) + (oz-CameraZ)*(oz-CameraZ) );
          else
            r = (float) MAX( fabs(ox-CameraX), fabs(oz-CameraZ) );

          if (r<CR) h = MObjects[ob].info.YLo + LandY;
        }

      }
  return h;
}



float GetLandQH(float CameraX, float CameraZ)
{
  float h,hh;

  h = GetLandH(CameraX, CameraZ);
  hh = GetLandH(CameraX-90.f, CameraZ-90.f);
  if (hh>h) h=hh;
  hh = GetLandH(CameraX+90.f, CameraZ-90.f);
  if (hh>h) h=hh;
  hh = GetLandH(CameraX-90.f, CameraZ+90.f);
  if (hh>h) h=hh;
  hh = GetLandH(CameraX+90.f, CameraZ+90.f);
  if (hh>h) h=hh;

  hh = GetLandH(CameraX+128.f, CameraZ);
  if (hh>h) h=hh;
  hh = GetLandH(CameraX-128.f, CameraZ);
  if (hh>h) h=hh;
  hh = GetLandH(CameraX, CameraZ+128.f);
  if (hh>h) h=hh;
  hh = GetLandH(CameraX, CameraZ-128.f);
  if (hh>h) h=hh;

  int ccx = (int)CameraX / 256;
  int ccz = (int)CameraZ / 256;

  for (int z=-4; z<=4; z++)
    for (int x=-4; x<=4; x++)
      if (OMap[ccz+z][ccx+x]!=255)
      {
        int ob = OMap[ccz+z][ccx+x];

        float CR = (float)MObjects[ob].info.Radius - 1.f;

        float oz = (ccz+z) * 256.f + 128.f;
        float ox = (ccx+x) * 256.f + 128.f;

        float LandY = GetLandOH(ccx+x, ccz+z);

        if (!(MObjects[ob].info.flags & ofBOUND))
        {
          if (MObjects[ob].info.YHi + LandY < h) continue;
          if (MObjects[ob].info.YHi + LandY > PlayerY+128) continue;
          //if (MObjects[ob].info.YLo + LandY > PlayerY+256) continue;
        }

        float r = CR+1;

        if (MObjects[ob].info.flags & ofBOUND)
        {
          float hh;
          if (PointOnBound(hh, CameraX, CameraZ, ox, oz, LandY, MObjects[ob].bound, ((FMap[ccz+z][ccx+x] >> 2) & 3)  ) )
            if (h < LandY + hh) h = LandY + hh;
        }
        else
        {
          if (MObjects[ob].info.flags & ofCIRCLE)
            r = (float) sqrt( (ox-CameraX)*(ox-CameraX) + (oz-CameraZ)*(oz-CameraZ) );
          else
            r = (float) MAX( fabs(ox-CameraX), fabs(oz-CameraZ) );

          if (r<CR) h = MObjects[ob].info.YHi + LandY;
        }

      }
  return h;
}


float GetLandHObj(float CameraX, float CameraZ)
{
  float h;

  h = 0;

  int ccx = (int)CameraX / 256;
  int ccz = (int)CameraZ / 256;

  for (int z=-3; z<=3; z++)
    for (int x=-3; x<=3; x++)
      if (OMap[ccz+z][ccx+x]!=255)
      {
        int ob = OMap[ccz+z][ccx+x];
        float CR = (float)MObjects[ob].info.Radius - 1.f;

        float oz = (ccz+z) * 256.f + 128.f;
        float ox = (ccx+x) * 256.f + 128.f;

        if (MObjects[ob].info.YHi + GetLandOH(ccx+x, ccz+z) < h) continue;
        if (MObjects[ob].info.YLo + GetLandOH(ccx+x, ccz+z) > PlayerY+256) continue;
        float r;
        if (MObjects[ob].info.flags & ofCIRCLE)
          r = (float) sqrt( (ox-CameraX)*(ox-CameraX) + (oz-CameraZ)*(oz-CameraZ) );
        else
          r = (float) MAX( fabs(ox-CameraX), fabs(oz-CameraZ) );

        if (r<CR)
          h = MObjects[ob].info.YHi + GetLandOH(ccx+x, ccz+z);
      }

  return h;
}


float GetLandQHNoObj(float CameraX, float CameraZ)
{
  float h,hh;

  h = GetLandH(CameraX, CameraZ);
  hh = GetLandH(CameraX-90.f, CameraZ-90.f);
  if (hh>h) h=hh;
  hh = GetLandH(CameraX+90.f, CameraZ-90.f);
  if (hh>h) h=hh;
  hh = GetLandH(CameraX-90.f, CameraZ+90.f);
  if (hh>h) h=hh;
  hh = GetLandH(CameraX+90.f, CameraZ+90.f);
  if (hh>h) h=hh;

  hh = GetLandH(CameraX+128.f, CameraZ);
  if (hh>h) h=hh;
  hh = GetLandH(CameraX-128.f, CameraZ);
  if (hh>h) h=hh;
  hh = GetLandH(CameraX, CameraZ+128.f);
  if (hh>h) h=hh;
  hh = GetLandH(CameraX, CameraZ-128.f);
  if (hh>h) h=hh;

  return h;
}


void ProcessCommandLine()
{

  for (int a=0; a<__argc; a++)
  {
    LPSTR s = __argv[a];
    if (strstr(s,"x="))
    {
      PlayerX = (float)atof(&s[2])*256.f;
      LockLanding = TRUE;
    }
    if (strstr(s,"y="))
    {
      PlayerZ = (float)atof(&s[2])*256.f;
      LockLanding = TRUE;
    }

    if (strstr(s,"reg=")) TrophyRoom.RegNumber = atoi(&s[4]);
    if (strstr(s,"prj=")) strcpy(ProjectName, (s+4));
    if (strstr(s,"din=")) TargetDino = (atoi(&s[4])*1024);
    if (strstr(s,"wep=")) WeaponPres = atoi(&s[4]);
    if (strstr(s,"dtm=")) OptDayNight  = atoi(&s[4]);
	if (strstr(s, "server=")) strcpy(ServerAddress, (s + 7));

    if (strstr(s,"-debug"))   DEBUG = TRUE;
    if (strstr(s,"-double"))  DoubleAmmo = TRUE;
	if (strstr(s, "-huntdog"))  DogMode = TRUE;
    if (strstr(s,"-radar"))   RadarMode = TRUE;
	if (strstr(s, "-sonar"))   SonarMode = TRUE;
	if (strstr(s, "-scanner"))   ScannerMode = TRUE;
	if (strstr(s, "-scent"))   ScentMode = TRUE;
	if (strstr(s, "-camo"))   CamoMode = TRUE;
	if (strstr(s, "-multiplayer"))   Multiplayer = TRUE;
	if (strstr(s, "-host"))   Host = TRUE;
	if (strstr(s, "-cisk"))   CiskMode = TRUE;
    if (strstr(s,"-tranq")) Tranq = TRUE;
    if (strstr(s,"-observ")) ObservMode = TRUE;

  }
}



void AddWCircle(float x, float z, float scale)
{
  WCircles[WCCount].pos.x = x;
  WCircles[WCCount].pos.z = z;
  WCircles[WCCount].pos.y = GetLandUpH(x, z);
  WCircles[WCCount].FTime = 0;
  WCircles[WCCount].scale = scale;
  WCCount++;
}


void AddShipTask(int cindex)
{

  TCharacter *cptr = &Characters[cindex];

  BOOL TROPHYON  = (GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z) < 100) &&
                   (!Tranq);

  if (TROPHYON)
  {
    ShipTask.clist[ShipTask.tcount] = cindex;
    ShipTask.tcount++;
    AddVoicev(ShipModel.SoundFX[3].length,
              ShipModel.SoundFX[3].lpData, 256);
  }

  //===== trophy =======//
  SYSTEMTIME st;
  GetLocalTime(&st);
  int t=0;
  for (t=0; t< TROPHY_COUNT-1; t++)
    if (!TrophyRoom.Body[t].ctype) break;

  //t += DinoInfo[Characters[cindex].CType].trophyFirstSlot;

  float score = (float)DinoInfo[Characters[cindex].CType].BaseScore;

  if (TrophyRoom.Last.success>1)
    score*=(1.f + TrophyRoom.Last.success / 10.f);

  if (!(TargetDino & (1<<Characters[cindex].AI)) ) score/=2.f;

  if (Tranq    ) score *= 1.25f;
  if (RadarMode) score *= 0.70f;
  if (ScentMode) score *= 0.80f;
  if (CamoMode ) score *= 0.85f;
  TrophyRoom.Score+=(int)score;


  if (!Tranq)
  {

	  TrophyDisplayBody.ctype = Characters[cindex].CType; //0 is blank trophy
	  TrophyDisplayBody.scale = Characters[cindex].scale;
	  TrophyDisplayBody.weapon = CurrentWeapon;
	  TrophyDisplayBody.score = (int)score;
	  TrophyDisplayBody.phase = (RealTime & 3);
	  TrophyDisplayBody.time = (st.wHour << 10) + st.wMinute;
	  TrophyDisplayBody.date = (st.wYear << 20) + (st.wMonth << 10) + st.wDay;
	  TrophyDisplayBody.range = VectorLength(SubVectors(Characters[cindex].pos, PlayerPos)) / 64.f;
	  TrophyDisplay = true;

	  TrophyTime = 20 * 1000;
	  if (DinoInfo[Characters[cindex].CType].trophyLocTotal2 < DinoInfo[Characters[cindex].CType].trophyTypeCount) {
		  DinoInfo[Characters[cindex].CType].trophyLocTotal2++;
		  TrophyBody = t;
		  TrophyRoom.Body[t].ctype = DinoInfo[Characters[cindex].CType].trophyCode; //0 is blank trophy
		  TrophyRoom.Body[t].scale = Characters[cindex].scale;
		  TrophyRoom.Body[t].weapon = CurrentWeapon;
		  TrophyRoom.Body[t].score = (int)score;
		  TrophyRoom.Body[t].phase = (RealTime & 3);
		  TrophyRoom.Body[t].time = (st.wHour << 10) + st.wMinute;
		  TrophyRoom.Body[t].date = (st.wYear << 20) + (st.wMonth << 10) + st.wDay;
		  TrophyRoom.Body[t].range = VectorLength(SubVectors(Characters[cindex].pos, PlayerPos)) / 64.f;
		  PrintLog("Trophy added: ");
		  PrintLog(DinoInfo[Characters[cindex].CType].Name);
		  PrintLog("\n");
	  }
  }
}



void InitShip(int cindex)
{
  TCharacter *cptr = &Characters[cindex];

  Ship.DeltaY = 2048.f + DinoInfo[cptr->CType].ShDelta * cptr->scale;

  Ship.pos.x = PlayerX - 90*256;
  if (Ship.pos.x < 256) Ship.pos.x = PlayerX + 90*256;
  Ship.pos.z = PlayerZ - 90*256;
  if (Ship.pos.z < 256) Ship.pos.z = PlayerZ + 90*256;
  Ship.pos.y = GetLandUpH(Ship.pos.x, Ship.pos.z)  + Ship.DeltaY + 1024;

  Ship.tgpos.x = cptr->pos.x;
  Ship.tgpos.z = cptr->pos.z;
  Ship.tgpos.y = GetLandUpH(Ship.tgpos.x, Ship.tgpos.z)  + Ship.DeltaY;
  Ship.State = 0;

  Ship.retpos = Ship.pos;
  Ship.cindex = cindex;
  Ship.FTime = 0;
}



void HideWeapon()
{
  TWeapon *wptr = &Weapon;
  if (UNDERWATER && !wptr->state) return;
  if (ObservMode || TrophyMode) return;

  if (wptr->state == 0)
  {
    if (!ShotsLeft[CurrentWeapon]) return;
    if (WeapInfo[CurrentWeapon].Optic) OPTICMODE = TRUE;
    AddVoicev(wptr->chinfo[CurrentWeapon].SoundFX[0].length,
              wptr->chinfo[CurrentWeapon].SoundFX[0].lpData, 256);
    wptr->FTime = 0;
    wptr->state = 1;
    BINMODE = FALSE;
    MapMode = FALSE;
    wptr->shakel = 0.2f;
    return;
  }

  if (wptr->state!=2 || wptr->FTime!=0) return;
  AddVoicev(wptr->chinfo[CurrentWeapon].SoundFX[2].length,
            wptr->chinfo[CurrentWeapon].SoundFX[2].lpData, 256);
  wptr->state = 3;
  wptr->FTime = 0;
  OPTICMODE = FALSE;
  return ;
}








void InitGameInfo()
{
  for (int c=0; c< DINOINFO_MAX; c++)
  {
    DinoInfo[c].Scale0 = 800;
    DinoInfo[c].ScaleA = 600;
    DinoInfo[c].ShDelta = 0;
  }
  /*
      WeapInfo[0].Name = "Shotgun";
  	WeapInfo[0].Power = 1.5f;
  	WeapInfo[0].Prec  = 1.1f;
  	WeapInfo[0].Loud  = 0.3f;
  	WeapInfo[0].Rate  = 1.6f;
  	WeapInfo[0].Shots = 6;

  	WeapInfo[1].Name = "X-Bow";
  	WeapInfo[1].Power = 1.1f;
  	WeapInfo[1].Prec  = 0.7f;
  	WeapInfo[1].Loud  = 1.9f;
  	WeapInfo[1].Rate  = 1.2f;
  	WeapInfo[1].Shots = 8;

      WeapInfo[2].Name = "Sniper Rifle";
  	WeapInfo[2].Power = 1.0f;
  	WeapInfo[2].Prec  = 1.8f;
  	WeapInfo[2].Loud  = 0.6f;
  	WeapInfo[2].Rate  = 1.0f;
  	WeapInfo[2].Shots = 6;




  	DinoInfo[ 0].Name = "Moschops";
  	DinoInfo[ 0].Health0 = 2;
  	DinoInfo[ 0].Mass = 0.15f;

      DinoInfo[ 1].Name = "Galimimus";
  	DinoInfo[ 1].Health0 = 2;
  	DinoInfo[ 1].Mass = 0.1f;

  	DinoInfo[ 2].Name = "Dimorphodon";
      DinoInfo[ 2].Health0 = 1;
  	DinoInfo[ 2].Mass = 0.05f;

  	DinoInfo[ 3].Name = "Dimetrodon";
      DinoInfo[ 3].Health0 = 2;
  	DinoInfo[ 3].Mass = 0.22f;


  	DinoInfo[ 5].Name = "Parasaurolophus";
  	DinoInfo[ 5].Mass = 1.5f;
  	DinoInfo[ 5].Length = 5.8f;
  	DinoInfo[ 5].Radius = 320.f;
  	DinoInfo[ 5].Health0 = 5;
  	DinoInfo[ 5].BaseScore = 6;
  	DinoInfo[ 5].SmellK = 0.8f; DinoInfo[ 4].HearK = 1.f; DinoInfo[ 4].LookK = 0.4f;
  	DinoInfo[ 5].ShDelta = 48;

  	DinoInfo[ 6].Name = "Pachycephalosaurus";
  	DinoInfo[ 6].Mass = 0.8f;
  	DinoInfo[ 6].Length = 4.5f;
  	DinoInfo[ 6].Radius = 280.f;
  	DinoInfo[ 6].Health0 = 4;
  	DinoInfo[ 6].BaseScore = 8;
  	DinoInfo[ 6].SmellK = 0.4f; DinoInfo[ 5].HearK = 0.8f; DinoInfo[ 5].LookK = 0.6f;
  	DinoInfo[ 6].ShDelta = 36;

  	DinoInfo[ 7].Name = "Stegosaurus";
      DinoInfo[ 7].Mass = 7.f;
  	DinoInfo[ 7].Length = 7.f;
  	DinoInfo[ 7].Radius = 480.f;
  	DinoInfo[ 7].Health0 = 5;
  	DinoInfo[ 7].BaseScore = 7;
  	DinoInfo[ 7].SmellK = 0.4f; DinoInfo[ 6].HearK = 0.8f; DinoInfo[ 6].LookK = 0.6f;
  	DinoInfo[ 7].ShDelta = 128;

  	DinoInfo[ 8].Name = "Allosaurus";
  	DinoInfo[ 8].Mass = 0.5;
  	DinoInfo[ 8].Length = 4.2f;
  	DinoInfo[ 8].Radius = 256.f;
  	DinoInfo[ 8].Health0 = 3;
  	DinoInfo[ 8].BaseScore = 12;
  	DinoInfo[ 8].Scale0 = 1000;
  	DinoInfo[ 8].ScaleA = 600;
  	DinoInfo[ 8].SmellK = 1.0f; DinoInfo[ 7].HearK = 0.3f; DinoInfo[ 7].LookK = 0.5f;
  	DinoInfo[ 8].ShDelta = 32;
  	DinoInfo[ 8].DangerCall = TRUE;

  	DinoInfo[ 9].Name = "Chasmosaurus";
  	DinoInfo[ 9].Mass = 3.f;
  	DinoInfo[ 9].Length = 5.0f;
  	DinoInfo[ 9].Radius = 400.f;
  	DinoInfo[ 9].Health0 = 8;
  	DinoInfo[ 9].BaseScore = 9;
  	DinoInfo[ 9].SmellK = 0.6f; DinoInfo[ 8].HearK = 0.5f; DinoInfo[ 8].LookK = 0.4f;
  	//DinoInfo[ 8].ShDelta = 148;
  	DinoInfo[ 9].ShDelta = 108;

  	DinoInfo[10].Name = "Velociraptor";
  	DinoInfo[10].Mass = 0.3f;
  	DinoInfo[10].Length = 4.0f;
  	DinoInfo[10].Radius = 256.f;
  	DinoInfo[10].Health0 = 3;
  	DinoInfo[10].BaseScore = 16;
  	DinoInfo[10].ScaleA = 400;
  	DinoInfo[10].SmellK = 1.0f; DinoInfo[ 9].HearK = 0.5f; DinoInfo[ 9].LookK = 0.4f;
  	DinoInfo[10].ShDelta =-24;
  	DinoInfo[10].DangerCall = TRUE;

  	DinoInfo[11].Name = "T-Rex";
      DinoInfo[11].Mass = 6.f;
  	DinoInfo[11].Length = 12.f;
  	DinoInfo[11].Radius = 400.f;
  	DinoInfo[11].Health0 = 1024;
  	DinoInfo[11].BaseScore = 20;
  	DinoInfo[11].SmellK = 0.85f; DinoInfo[10].HearK = 0.8f; DinoInfo[10].LookK = 0.8f;
  	DinoInfo[11].ShDelta = 168;
  	DinoInfo[11].DangerCall = TRUE;

  	DinoInfo[ 4].Name = "Brahiosaurus";
      DinoInfo[ 4].Mass = 9.f;
  	DinoInfo[ 4].Length = 12.f;
  	DinoInfo[ 4].Radius = 400.f;
  	DinoInfo[ 4].Health0 = 1024;
  	DinoInfo[ 4].BaseScore = 0;
  	DinoInfo[ 4].SmellK = 0.85f; DinoInfo[16].HearK = 0.8f; DinoInfo[16].LookK = 0.8f;
  	DinoInfo[ 4].ShDelta = 168;
  	DinoInfo[ 4].DangerCall = FALSE;
  */
  LoadResourcesScript();
}


// MULTIPLAYER ===================================================

void putInt(byte data[], int *pos, long in) {
	data[*pos] = (int)((in & 0XFF));
	*pos += 1;
}

void putInt2(byte data[], int *pos, long in) {
	data[*pos] = (int)((in >> 8) & 0XFF);
	data[*pos + 1] = (int)((in & 0XFF));
	*pos += 2;
}

void putFloat(byte data[], int *pos, long in) {
	data[*pos]   = (int)((in >> 24) & 0xFF);
	data[*pos+1] = (int)((in >> 16) & 0xFF);
	data[*pos+2] = (int)((in >> 8) & 0XFF);
	data[*pos+3] = (int)((in & 0XFF));
	*pos += 4;
}

int readInt(const byte data[], int *pos) {
	int pos2 = *pos;
	*pos += 1;
	return (int)((data[pos2]));
}

int readInt2(const byte data[], int *pos) {
	int pos2 = *pos;
	*pos += 2;
	return (int)((data[pos2] << 8)
		+ (data[pos2 + 1]));
}

float readFloat(const byte data[], int *pos) {
	int pos2 = *pos;
	*pos += 4;
	return (float)((data[pos2] << 24)
		+ (data[pos2 + 1] << 16)
		+ (data[pos2 + 2] << 8)
		+ (data[pos2 + 3]));
}

bool RecvPacket(SOCKET *socket, int bufSize, bool init){
	iResult = recv(*socket, recvbuf, bufSize, 0);
	if (iResult > 0) {

		const byte *tdata2 = reinterpret_cast<const byte*>(recvbuf);
		int pos = 0;

		Vector3d *posTemp = new Vector3d;
		posTemp->x = readFloat(tdata2, &pos) / 10000.f;
		posTemp->y = readFloat(tdata2, &pos) / 10000.f;
		posTemp->z = readFloat(tdata2, &pos) / 10000.f;
		MPlayers[0].pos = *posTemp;

		MPlayers[0].alpha = readFloat(tdata2, &pos) / 10000.f;

		MPlayers[0].alpha += 1.5 * pi;
		if (MPlayers[0].alpha > pi * 2) MPlayers[0].alpha -= 2 * pi;

		int t = readInt(tdata2, &pos) - 1;
		if (t >= 0) mGunShot[0] = t;

		t = readInt(tdata2, &pos) - 1;
		if (t >= 0) mHunterCall[0] = t;

		t = readInt(tdata2, &pos) - 1;
		if (t >= 0) mHunterCallType[0] = t;

		if (!Host) {

			Wind.alpha = readFloat(tdata2, &pos) / 10000.f;
			Wind.speed = readFloat(tdata2, &pos) / 10000.f;

			for (int c = 0; c < 6; c++) {
				Characters[c].pos.x = readFloat(tdata2, &pos) / 10000.f;
				Characters[c].pos.z = readFloat(tdata2, &pos) / 10000.f;
				if (Characters[c].Clone == AI_DIMOR || Characters[c].Clone == AI_PTERA) Characters[c].pos.y = readFloat(tdata2, &pos) / 10000.f; //potentially all 6 ambs
				Characters[c].alpha = readFloat(tdata2, &pos) / 10000.f;
				Characters[c].bend = readFloat(tdata2, &pos) / 10000.f;
				Characters[c].Phase = readInt(tdata2, &pos);
				if (init) {
					Characters[c].scale = readFloat(tdata2, &pos) / 10000.f;
					Characters[c].deathType = readInt(tdata2, &pos);
					Characters[c].waterDieAnim = readInt(tdata2, &pos);
				}
			}
		} else {
			for (int c = 0; c < 6; c++) {
				mDamage[0][c] += readInt2(tdata2, &pos);
			}
		}

		return TRUE;
	}
	else return FALSE;
}

void SendPacket(SOCKET *socket, const int bufSize, bool init) {

	int pos = 0;
	byte tdata[bufSizeMax];

	long px = PlayerX * 10000.f;
	long py = PlayerY * 10000.f;
	long pz = PlayerZ * 10000.f;
	long pa = PlayerAlpha * 10000.f;
	putFloat(tdata, &pos, px);
	putFloat(tdata, &pos, py);
	putFloat(tdata, &pos, pz);
	putFloat(tdata, &pos, pa);

	long ps = sendGunShot + 1;
	sendGunShot = -1;
	putInt(tdata, &pos, ps);

	long pc = sendHunterCall + 1;
	sendHunterCall = -1;
	putInt(tdata, &pos, pc);

	long pct = sendHunterCallType + 1;
	sendHunterCallType = -1;
	putInt(tdata, &pos, pct);

	if (Host) {
		long wa = Wind.alpha * 10000.;
		long ws = Wind.speed * 10000.;
		putFloat(tdata, &pos, wa);
		putFloat(tdata, &pos, ws);

		for (int c = 0; c < 6; c++) {
			long charX = Characters[c].pos.x * 10000.f;
			putFloat(tdata, &pos, charX);
			long charZ = Characters[c].pos.z * 10000.f;
			putFloat(tdata, &pos, charZ);
			if (Characters[c].Clone == AI_DIMOR || Characters[c].Clone == AI_PTERA) { //potentially all 6 ambs
				long charY = Characters[c].pos.y * 10000.f;
				putFloat(tdata, &pos, charY);
			}
			long charA = Characters[c].alpha * 10000.f;
			putFloat(tdata, &pos, charA);
			long charB = Characters[c].bend * 10000.f;
			putFloat(tdata, &pos, charB);
			long charPh = Characters[c].Phase;
			putInt(tdata, &pos, charPh);
			if (init) {
				long charScale = Characters[c].scale * 10000.f;
				putFloat(tdata, &pos, charScale);
				//long charKill = Characters[c].killType;	// not for classic ambients
				//putInt(tdata, &pos, charKill);
				long charDeath = Characters[c].deathType;
				putInt(tdata, &pos, charDeath);
				long charWDeath = Characters[c].waterDieAnim;
				putInt(tdata, &pos, charWDeath);
				//long charRoar = Characters[c].roarAnim;	// not for classic ambients
				//putInt(tdata, &pos, charRoar);
			}
		}

	} else {
		for (int c = 0; c < 6; c++) {
			long Damage = sendDamage[c];
			sendDamage[c] = 0;
			putInt2(tdata, &pos, Damage);
		}
	}

	const char *sendbuf = reinterpret_cast<const char*>(tdata);

	iSendResult = send(*socket, sendbuf, bufSize, 0);
	if (iSendResult == SOCKET_ERROR) {
		PrintLog("send failed");
		closesocket(*socket);
		WSACleanup();
		DoHalt("Multiplayer: Send failed");
	}
}

DWORD WINAPI ServerCommsThread(LPVOID lpParameter)
{
	bool init = TRUE;

	while (HaltThread) {

		bool result = RecvPacket(&ClientSocket, bufSizeClient, FALSE);
		if (result) {

			if (init) {
				SendPacket(&ClientSocket, bufSizeHostInit, TRUE);
				init = FALSE;
				PrintLog("INIT_PACKET_SENT\n");//TEST
			} else SendPacket(&ClientSocket, bufSizeHost, FALSE);

		} else if (iResult != 0) {
			PrintLog("recv failed\n");
			closesocket(ClientSocket);
			WSACleanup();
			DoHalt("Multiplayer Host: recv failed");
		}
	}
	PrintLog("Server Comms Thread Shutdown Successful!\n");
	return 0;
}

DWORD WINAPI ClientCommsThread(LPVOID lpParameter)
{
	bool init = TRUE;

	while (HaltThread) {

		SendPacket(&ConnectSocket,bufSizeClient, FALSE);

		// Receive until the peer closes the connection
		bool responded = FALSE;
		do {

			if (init) {
				responded = RecvPacket(&ConnectSocket, bufSizeHostInit, TRUE);
			}
			else responded = RecvPacket(&ConnectSocket, bufSizeHost, FALSE);

			if (!responded && iResult != 0) {
				PrintLog("recv failed\n");
			}

		} while (!responded);

		if (init) PrintLog("INIT_PACKET_RECV\n");//TEST
		init = FALSE;

		//Sleep(10);//test
		// if laggy, add sleep statement
	}
	PrintLog("Client Comms Thread Shutdown Successful!\n");
	return 0;
}

void ShutDownServer() {

	//shutdown thread
	PrintLog("Server Comms Thread Shutting Down...\n");
	HaltThread = FALSE;
	WaitForSingleObject(CommsThreadHandle, INFINITE);
	CloseHandle(CommsThreadHandle);

	
	//tell clients to shut down

	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		PrintLog("shutdown failed\n");
		closesocket(ClientSocket);
		WSACleanup();
		DoHalt2("Multiplayer Host: shutdown failed\n");
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();

	//test
	PrintLog("ShutDown multiplayer\n");
	PrintLog("COMPLETE!\n");
}

void ShutDownClient() {

	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		PrintLog("shutdown failed");
		closesocket(ConnectSocket);
		WSACleanup();
		DoHalt2("Multiplayer Client: Shutdown failed");
	}

	//shutdown thread
	PrintLog("Client Comms Thread Shutting Down...\n");
	HaltThread = FALSE;
	WaitForSingleObject(CommsThreadHandle, INFINITE);
	CloseHandle(CommsThreadHandle);

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	//test
	PrintLog("Client connection closed!\n");
	PrintLog("COMPLETE!\n");
}

void _StartupServer() {
	//server

	PrintLog("Starting Server...\n");



	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		PrintLog("WSAStartup failed\n");
		DoHalt2("Multiplayer Host: WSAStartup failed");
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		PrintLog("getaddrinfo failed\n");
		WSACleanup();
		DoHalt2("Multiplayer Host: getaddrinfo failed");
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		PrintLog("socket failed\n");
		freeaddrinfo(result);
		WSACleanup();
		DoHalt2("Multiplayer Host: socket failed");
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		PrintLog("bind failed\n");
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		DoHalt2("Multiplayer Host: bind failed");
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		PrintLog("listen failed\n");
		closesocket(ListenSocket);
		WSACleanup();
		DoHalt2("Multiplayer Host: listen failed");
	}

	PrintLog("Waiting for client...\n");


	//CLIENT CONNECTION - TEMPORARY, check for new clients for x amount of time each iteration? Shutdown on exit, kick clients first


	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		PrintLog("accept failed\n");
		closesocket(ListenSocket);
		WSACleanup();
		DoHalt2("Multiplayer Host: accept failed");
	}
	else PrintLog("Client accepted!\n");

	// No longer need server socket
	closesocket(ListenSocket);

	/*
	// Receive until the peer shuts down the connection
	do {

		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			PrintLog("Bytes received: ");
			char bytesRec[25];
			_itoa(iResult, bytesRec, 10);
			PrintLog(bytesRec);
			PrintLog("\n");


			// Echo the buffer back to the sender
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				PrintLog("send failed\n");
				closesocket(ClientSocket);
				WSACleanup();
				DoHalt2("Multiplayer Host: send failed");
			}
			PrintLog("Bytes sent: ");
			char bytesSent[25];
			_itoa(iSendResult, bytesSent, 10);
			PrintLog(bytesSent);
			PrintLog("\n");
		}
		else if (iResult == 0) {}
		else {
			PrintLog("recv failed\n");
			closesocket(ClientSocket);
			WSACleanup();
			DoHalt2("Multiplayer Host: recv failed");
		}

	} while (iResult > 0);
	*/
}

void _StartupClient() {
	//CLIENT



	struct addrinfo
		//		  *result = NULL,
		*ptr = NULL
		//		  ,
		//		  hints
		;

	const char *sendbuf = "this is a test";

	/*
	// Validate the parameters
	if (argc != 2) {
		printf("usage: %s server-name\n", argv[0]);
		return 1;
	}
	*/

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		PrintLog("WSAStartup failed");
		DoHalt2("Multiplayer Client: WSAStartup failed");
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(static_cast<LPCTSTR>(ServerAddress), DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		PrintLog("getaddrinfo failed");
		WSACleanup();
		DoHalt2("Multiplayer Client: getaddrinfo failed");
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			PrintLog("socket failed\n");
			WSACleanup();
			DoHalt2("Multiplayer Client: Socket failed");
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		PrintLog("Unable to connect to server!\n");
		WSACleanup();
		DoHalt2("Multiplayer Client: Unable to connect to server!");
	}

	/*
	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		PrintLog("send failed");
		closesocket(ConnectSocket);
		WSACleanup();
		DoHalt2("Multiplayer Client: Send failed");
	}

	//PrintLog("Bytes Sent: %ld\n", iResult);
	PrintLog("Bytes sent: ");
	char bytesSent[25];
	_itoa(iResult, bytesSent, 10);
	PrintLog(bytesSent);
	PrintLog("\n");

	// Receive until the peer closes the connection
	bool responded = FALSE;
	do {
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			//PrintLog("Bytes received: %d\n", iResult);
			PrintLog("Bytes received: ");
			char bytesSent[25];
			_itoa(iResult, bytesSent, 10);
			PrintLog(bytesSent);
			PrintLog("\n");
			responded = TRUE;
		}
		else if (iResult == 0) {
			//PrintLog("Connection closed\n");
		}
		else {
			PrintLog("recv failed\n");
		}

	} while (!responded);
	*/

	//multiplayer test end
}

void StartupServerCommsThread() {
	PrintLog("Starting Server Comms...\n");
	_StartupServer();
	CommsThreadHandle = CreateThread(0, 0, ServerCommsThread, NULL, 0, CommsThreadID);
	PrintLog("Server Comms Thread Started\n");
}

void StartupClientCommsThread() {
	PrintLog("Starting Client Comms...\n");
	_StartupClient();
	CommsThreadHandle = CreateThread(0, 0, ClientCommsThread, NULL, 0, CommsThreadID);
	PrintLog("Client Comms Thread Started\n");
}




void InitEngine()
{
  DEBUG        = FALSE;

  WATERANI     = TRUE;
  NODARKBACK   = TRUE;
  LoDetailSky  = TRUE;
  CORRECTION   = TRUE;
  FOGON        = TRUE;
  FOGENABLE    = TRUE;
  Clouds       = TRUE;
  SKY          = TRUE;
  GOURAUD      = TRUE;
  MODELS       = TRUE;
  TIMER        = DEBUG;
  BITMAPP      = FALSE;
  MIPMAP       = TRUE;
  NOCLIP       = FALSE;
  CLIP3D       = TRUE;


  SLOW         = FALSE;
  LOWRESTX     = FALSE;
  MORPHP       = TRUE;
  MORPHA       = TRUE;

  _GameState = 0;
  _MultiplayerState = 0;

  RadarMode    = FALSE;

  //multiplayer
  Multiplayer = FALSE;
  HaltThread = TRUE;
  Host = FALSE;
  ListenSocket = INVALID_SOCKET;
  ClientSocket = INVALID_SOCKET;
  ConnectSocket = INVALID_SOCKET;
  recvbuflen = DEFAULT_BUFLEN;
  result = NULL;

  fnt_BIG = CreateFont(
              23, 10, 0, 0,
              600, 0,0,0,
#ifdef __rus
              RUSSIAN_CHARSET,
#else
              ANSI_CHARSET,
#endif
              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, NULL);




  fnt_Small = CreateFont(
                16, 7, 0, 0,
				100, 0,0,0,
	  
	  //14, 5, 0, 0,
	  //100, 0, 0, 0,
#ifdef __rus
                RUSSIAN_CHARSET,
#else
                ANSI_CHARSET,
#endif
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, NULL);


  fnt_Midd  = CreateFont(
			    16, 7, 0, 0,
	            550, 0, 0, 0,
#ifdef __rus
                RUSSIAN_CHARSET,
#else
                ANSI_CHARSET,
#endif
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, NULL);


  Heap = HeapCreate( 0, 60000000, 0 );
  if( Heap == NULL )
  {
    MessageBox(hwndMain,"Error creating heap.","Error",IDOK);
    return;
  }

  Textures[255] = (TEXTURE*) _HeapAlloc(Heap, 0, sizeof(TEXTURE));

  WaterR = 10;
  WaterG = 38;
  WaterB = 46;
  WaterA = 10;
  TargetDino = 1<<10;
  TargetCall = 10;
  WeaponPres = 1;
  MessageList.timeleft = 0;

  InitGameInfo();

  CreateVideoDIB();
  CreateFadeTab();
  CreateDivTable();
  InitClips();

  TrophyRoom.RegNumber=0;

  PlayerX = (ctMapSize / 3) * 256;
  PlayerZ = (ctMapSize / 3) * 256;

  ProcessCommandLine();


  switch (OptDayNight)
  {
  case 0:
    SunShadowK = 0.7;
    Sun3dPos.x = - 4048;
    Sun3dPos.y = + 2048;
    Sun3dPos.z = - 4048;
    break;
  case 1:
    SunShadowK = 0.5;
    Sun3dPos.x = - 2048;
    Sun3dPos.y = + 4048;
    Sun3dPos.z = - 2048;
    break;
  case 2:
    SunShadowK = -0.7;
    Sun3dPos.x = + 3048;
    Sun3dPos.y = + 3048;
    Sun3dPos.z = + 3048;
    break;
  }

  LoadTrophy();

  ProcessCommandLine();

  /*
  //Multiplayer
  if (Multiplayer) {
	  if (Host) {

		  

		  //test end


	  } else {




	  }
  }
  */



  //ctViewR  = 72;
  //ctViewR1 = 28;
  //ctViewRM = 24;

  
  ctViewR = 42 + (int)(OptViewR / 8) * 2;
  ctViewR1 = 28;
  ctViewRM = 24;
  

  /*
  //TEST VERSION - INTRODUCE QUAILTY SLIDER?
  ctViewR  = 42 + (int)(OptViewR / 3.1875);
  ctViewR1 = ctViewR - 10; // 28 + (int)(OptViewR / 1.175115207373272);
  ctViewRM = 24; //leave as 24 default
  if (ctViewR < 20) ctViewR = 20;
  if (ctViewR > 122) ctViewR = 122;
  //if (ctViewR1 < 12) ctViewR1 = 12;
  //if (ctViewR1 > ctViewR - 10) ctViewR1 = ctViewR - 10;
  */

  Soft_Persp_K = 1.5f;
  HeadY = 220;

  FogsList[0].fogRGB = 0x000000;
  FogsList[0].YBegin = 0;
  FogsList[0].Transp = 000;
  FogsList[0].FLimit = 000;

  FogsList[127].fogRGB = 0x00504000;
  FogsList[127].Mortal = FALSE;
  FogsList[127].Transp = 460;
  FogsList[127].FLimit = 200;

  FillMemory( FogsMap, sizeof(FogsMap), 0);
  PrintLog("Init Engine: Ok.\n");
}





void ShutDownEngine()
{
  ReleaseDC(hwndMain,hdcMain);
}



void ProcessSyncro()
{
  RealTime = timeGetTime();
  srand( (unsigned) RealTime );
  if (SLOW) RealTime/=4;
  TimeDt = RealTime - PrevTime;
  if (TimeDt<0) TimeDt = 10;
  if (TimeDt>10000) TimeDt = 10;
  if (TimeDt>1000) TimeDt = 1000;
  PrevTime = RealTime;
  Takt++;
  if (!PAUSE)
    if (MyHealth) MyHealth+=TimeDt*4;
  if (MyHealth>MAX_HEALTH) MyHealth = MAX_HEALTH;
}






void AddBloodTrail(TCharacter *cptr)
{
  if (BloodTrail.Count>508)
  {
    memcpy(&BloodTrail.Trail[0], &BloodTrail.Trail[1], 510*sizeof(TBloodP));
    BloodTrail.Count--;
  }
  BloodTrail.Trail[BloodTrail.Count].LTime = 210000;
  BloodTrail.Trail[BloodTrail.Count].pos = cptr->pos;
  BloodTrail.Trail[BloodTrail.Count].pos.x+=siRand(32);
  BloodTrail.Trail[BloodTrail.Count].pos.z+=siRand(32);
  BloodTrail.Trail[BloodTrail.Count].pos.y =
    GetLandH(BloodTrail.Trail[BloodTrail.Count].pos.x,
             BloodTrail.Trail[BloodTrail.Count].pos.z)+4;
  BloodTrail.Count++;
}





void MakeCall()
{
  if (!TargetDino) return;
  if (UNDERWATER) return;
  if (ObservMode || TrophyMode) return;
  if (CallLockTime) return;

  CallLockTime=1024*3;

  NextCall+=(RealTime % 2)+1;
  NextCall%=3;

  AddVoicev(fxCall[TargetCall-10][NextCall].length,
            fxCall[TargetCall-10][NextCall].lpData, 256);

  //multiplayer
  sendHunterCall = TargetCall - 10;
  sendHunterCallType = NextCall;

  float dmin = 512*256;
  int ai = -1;

  for (int c=0; c<ChCount; c++)
  {
    TCharacter *cptr = &Characters[c];

	if (DinoInfo[cptr->CType].fearCall[TargetCall]) {
		cptr->State = 2;
		cptr->AfraidTime = (10 + rRand(5)) * 1024;
	}

	/*
    if (DinoInfo[AI_to_CIndex[TargetCall] ].DangerCall)
      if (cptr->AI<10)
      {
        cptr->State=2;
        cptr->AfraidTime = (10 + rRand(5)) * 1024;
      }
	  */

    if (cptr->AI!=TargetCall) continue;
    if (cptr->AfraidTime) continue;
    if (cptr->State) continue;

    float d = VectorLength(SubVectors(PlayerPos, cptr->pos));
    if (d < ctViewR * 400)
    {
      if (rRand(128) > 32)
        if (d<dmin)
        {
          dmin = d;
          ai = c;
        }
      cptr->tgx = PlayerX + siRand(1800);
      cptr->tgz = PlayerZ + siRand(1800);
    }
  }

  if (ai!=-1)
  {
    answpos = SubVectors(Characters[ai].pos, PlayerPos);
    answpos.x/=-3.f;
    answpos.y/=-3.f;
    answpos.z/=-3.f;
    answpos = SubVectors(PlayerPos, answpos);
    answtime = 2000 + rRand(2000);
    answcall = TargetCall;
  }

}



DWORD ColorSum(DWORD C1, DWORD C2)
{
  DWORD R,G,B;
  R = MIN(255, ((C1>> 0) & 0xFF) + ((C2>> 0) & 0xFF));
  G = MIN(255, ((C1>> 8) & 0xFF) + ((C2>> 8) & 0xFF));
  B = MIN(255, ((C1>>16) & 0xFF) + ((C2>>16) & 0xFF));
  return R + (G<<8) + (B<<16);
}


#define partBlood   1
#define partWater   2
#define partGround  3
#define partBubble  4


void AddElements(float x, float y, float z, int etype, int cnt)
{
	AddElementsA(x,y,z,etype,cnt,cnt,false,0);
}

void AddElementsA(float x, float y, float z, int etype, int cnt, int mag, bool angled, float alph)
{
  if (ElCount > 698)
  {
    memcpy(&Elements[0], &Elements[1], (ElCount-1) * sizeof(TElements));
    ElCount--;
  }

  Elements[ElCount].EDone  = 0;
  Elements[ElCount].Type = etype;
  Elements[ElCount].ECount = MIN(30, cnt);
  int c;

  switch (etype)
  {
  case partBlood:
#ifdef _d3d
    Elements[ElCount].RGBA = 0xE0600000;
    Elements[ElCount].RGBA2= 0x20300000;
#else
    Elements[ElCount].RGBA = 0xE0000060;
    Elements[ElCount].RGBA2= 0x20000030;
#endif
    break;

  case partGround:
#ifdef _d3d
    Elements[ElCount].RGBA = 0xF0F09E55;
    Elements[ElCount].RGBA2= 0x10F09E55;
#else
    Elements[ElCount].RGBA = 0xF0559EF0;
    Elements[ElCount].RGBA2= 0x10559EF0;
#endif
    break;


  case partBubble:
    c = WaterList[ WMap[ (int)z / 256][ (int)x / 256] ].fogRGB;
#ifdef _d3d
    c = ColorSum( ((c & 0xFEFEFE)>>1), 0x152020);
#else
    c = ColorSum( ((c & 0xFEFEFE)>>1), 0x202015);
#endif
    Elements[ElCount].RGBA = 0x70000000 + (ColorSum(c, ColorSum(c,c)));
    Elements[ElCount].RGBA2= 0x40000000 + (ColorSum(c, c));
    break;

  case partWater:
    c = WaterList[ WMap[ (int)z / 256][ (int)x / 256] ].fogRGB;
#ifdef _d3d
    c = ColorSum( ((c & 0xFEFEFE)>>1), 0x152020);
#else
    c = ColorSum( ((c & 0xFEFEFE)>>1), 0x202015);
#endif
    Elements[ElCount].RGBA  = 0xB0000000 + ( ColorSum(c, ColorSum(c,c)) );
    Elements[ElCount].RGBA2 = 0x40000000 + (c);
    break;
  }

  Elements[ElCount].RGBA  = conv_xGx(Elements[ElCount].RGBA);
  Elements[ElCount].RGBA2 = conv_xGx(Elements[ElCount].RGBA2);

  float al = siRand(128) / 128.f * pi / 4.f;
  float ss = sin(al);
  float cc = cos(al);

  for (int e=0; e<Elements[ElCount].ECount; e++)
  {
    Elements[ElCount].EList[e].pos.x = x;
    Elements[ElCount].EList[e].pos.y = y;
    Elements[ElCount].EList[e].pos.z = z;
    Elements[ElCount].EList[e].R = 6 + rRand(5);
    Elements[ElCount].EList[e].Flags = 0;
    float v;

	float velo = mag * rRand(20)/20;

    switch (etype)
    {
    case partBlood:
      v = velo * 6 + rRand(96) + 220;
      Elements[ElCount].EList[e].speed.x =ss*ca*v + siRand(32);
      Elements[ElCount].EList[e].speed.y =cc * (v * 3);
      Elements[ElCount].EList[e].speed.z =ss*sa*v + siRand(32);
      break;
    case partGround:
      Elements[ElCount].EList[e].speed.x =siRand(52)-sa*64;
      Elements[ElCount].EList[e].speed.y =rRand(100) + 600 + velo * 20;
      Elements[ElCount].EList[e].speed.z =siRand(52)+ca*64;
      break;
    case partWater:
		Elements[ElCount].EList[e].speed.x = siRand(32);
		Elements[ElCount].EList[e].speed.z = siRand(32);
		Elements[ElCount].EList[e].speed.y =rRand(80) + 400 + velo * 40;
		if (angled) {
			Elements[ElCount].EList[e].speed.x = siRand(132) + ((float)cos(alph) * velo * 40);
			Elements[ElCount].EList[e].speed.z = siRand(132) + ((float)sin(alph) * velo * 40);
		}
      break;
    case partBubble:
      Elements[ElCount].EList[e].speed.x =siRand(40);
      Elements[ElCount].EList[e].speed.y =rRand(140) + 20;
      Elements[ElCount].EList[e].speed.z =siRand(40);
      break;
    }
  }

  ElCount++;
}


void MakeShot(float ax, float ay, float az,
              float bx, float by, float bz)
{
  int sres;
  if (!WeapInfo[CurrentWeapon].Fall)
    sres = TraceShot(ax, ay, az, bx, by, bz);
  else
  {
    Vector3d dl;
    float dy = 40.f * ctViewR / 36.f;
    dl.x = (bx-ax) / 3;
    dl.y = (by-ay) / 3;
    dl.z = (bz-az) / 3;
    bx = ax + dl.x;
    by = ay + dl.y - dy / 2;
    bz = az + dl.z;
    sres = TraceShot(ax, ay, az, bx, by, bz);
    if (sres!=-1) goto ENDTRACE;
    ax = bx;
    ay = by;
    az = bz;

    bx = ax + dl.x;
    by = ay + dl.y - dy * 3;
    bz = az + dl.z;
    sres = TraceShot(ax, ay, az, bx, by, bz);
    if (sres!=-1) goto ENDTRACE;
    ax = bx;
    ay = by;
    az = bz;

    bx = ax + dl.x;
    by = ay + dl.y - dy * 5;
    bz = az + dl.z;
    sres = TraceShot(ax, ay, az, bx, by, bz);
    if (sres!=-1) goto ENDTRACE;
    ax = bx;
    ay = by;
    az = bz;
  }

ENDTRACE:
  if (sres==-1) return;

  int mort = (sres & 0xFF00) && (Characters[ShotDino].Health);
  sres &= 0xFF;

  if (sres == tresGround)
    AddElements(bx, by, bz, partGround, 6 + WeapInfo[CurrentWeapon].Power*4);
  if (sres == tresModel)
    AddElements(bx, by, bz, partGround, 6 + WeapInfo[CurrentWeapon].Power*4);


  if (sres == tresWater)
  {
    AddElements(bx, by, bz, partWater, 4 + WeapInfo[CurrentWeapon].Power*3);
    //AddElements(bx, GetLandH(bx, bz), bz, partBubble);
    //AddWCircle(bx, bz, 1.2);
    AddWCircle(bx, bz, 1.2);
  }


  if (sres!=tresChar) return;
  AddElements(bx, by, bz, partBlood, 4 + WeapInfo[CurrentWeapon].Power*4);
  if (!Characters[ShotDino].Health) return;

//======= character damage =========//

  if (Multiplayer && !Host) {
	  if (mort) sendDamage[ShotDino] = Characters[ShotDino].Health;
	  else sendDamage[ShotDino] += WeapInfo[CurrentWeapon].Power;
  } else {
	  if (mort) Characters[ShotDino].Health = 0;
	  else Characters[ShotDino].Health -= WeapInfo[CurrentWeapon].Power;
	  if (Characters[ShotDino].Health < 0) Characters[ShotDino].Health = 0;
	  registerDamage(ShotDino);
  }
  
}


void registerDamage(int Dino) {

	if (!Characters[Dino].Health)
	{
		if (DinoInfo[Characters[Dino].CType].trophySession && !Multiplayer) //No trophies in multiplayer for now - update this at later date?
		{
			TrophyRoom.Last.success++;
			AddShipTask(Dino);
		}

		if (Characters[Dino].AI > 0 && Characters[Dino].AI < 6 && !Multiplayer) //No amb respawn in multiplayer for now - update this at later date?
			Characters_AddSecondaryOne(Characters[Dino].CType);

	}
	else
	{
		Characters[Dino].awareHunter = TRUE;
		Characters[Dino].AfraidTime = 60 * 1000;
		if (Characters[Dino].Clone != AI_TREX || Characters[Dino].State == 0)
			Characters[Dino].State = 2;

		//if (Characters[Dino].AI != AI_BRACH) // I removed this :)
		Characters[Dino].BloodTTime += 90000;

	}

	if (Characters[Dino].Clone == AI_TREX)
		if (Characters[Dino].State)
			Characters[Dino].State = 5;
		else
			Characters[Dino].State = 1;

}



void RemoveCharacter(int index)
{
  if (index==-1) return;
  memcpy( &Characters[index], &Characters[index+1], (255 - index) * sizeof(TCharacter) );
  ChCount--;

  if (DemoPoint.CIndex > index) DemoPoint.CIndex--;

  for (int c=0; c<ShipTask.tcount; c++)
    if (ShipTask.clist[c]>index) ShipTask.clist[c]--;
}


void AnimateShip()
{
  if (Ship.State==-1)
  {
    SetAmbient3d(0,0, 0,0,0);
    if (!ShipTask.tcount) return;
    InitShip(ShipTask.clist[0]);
    memcpy(&ShipTask.clist[0], &ShipTask.clist[1], 250*4);
    ShipTask.tcount--;
    return;
  }

  SetAmbient3d(ShipModel.SoundFX[0].length,
               ShipModel.SoundFX[0].lpData,
               Ship.pos.x, Ship.pos.y, Ship.pos.z);

  int _TimeDt = TimeDt;

//====== get up/down time acceleration ===========//
  if (Ship.FTime)
  {
    int am = ShipModel.Animation[0].AniTime;
    if (Ship.FTime < 500) _TimeDt = TimeDt * (Ship.FTime + 48) / 548;
    if (am-Ship.FTime < 500) _TimeDt = TimeDt * (am-Ship.FTime + 48) / 548;
    if (_TimeDt<2) _TimeDt=2;
  }
//===================================

  float L  = VectorLength( SubVectors(Ship.tgpos, Ship.pos) );
  float L2 = sqrt ( (Ship.tgpos.x - Ship.pos.x) * (Ship.tgpos.x - Ship.pos.x) +
                    (Ship.tgpos.x - Ship.pos.x) * (Ship.tgpos.x - Ship.pos.x) );

  Ship.pos.y+=0.3f*(float)cos(RealTime / 256.f);



  Ship.tgalpha    = FindVectorAlpha(Ship.tgpos.x - Ship.pos.x, Ship.tgpos.z - Ship.pos.z);
  float currspeed;
  float dalpha = (float)fabs(Ship.tgalpha - Ship.alpha);
  float drspd = dalpha;
  if (drspd>pi) drspd = 2*pi - drspd;


//====== fly more away if I near =============//
  if (Ship.State)
    if (Ship.speed>1)
      if (L<4000)
        if (VectorLength(SubVectors(PlayerPos, Ship.pos))<(ctViewR+2)*256)
        {
          Ship.tgpos.x += (float)cos(Ship.alpha) * 256*6.f;
          Ship.tgpos.z += (float)sin(Ship.alpha) * 256*6.f;
          Ship.tgpos.y = GetLandUpH(Ship.tgpos.x, Ship.tgpos.z) + Ship.DeltaY;
          Ship.tgpos.y = MAX(Ship.tgpos.y, GetLandUpH(Ship.pos.x, Ship.pos.z) + Ship.DeltaY);
        }
//==============================//



//========= animate down ==========//
  if (Ship.State==3)
  {
    Ship.FTime+=_TimeDt;
    if (Ship.FTime>=ShipModel.Animation[0].AniTime)
    {
      Ship.FTime=ShipModel.Animation[0].AniTime-1;
      Ship.State=2;
      AddVoicev(ShipModel.SoundFX[4].length,
                ShipModel.SoundFX[4].lpData, 256);
      AddVoice3d(ShipModel.SoundFX[1].length, ShipModel.SoundFX[1].lpData,
                 Ship.pos.x, Ship.pos.y, Ship.pos.z);
    }
    return;
  }


//========= get body on board ==========//
  if (Ship.State)
  {
    if (Ship.cindex!=-1)
    {
      DeltaFunc(Characters[Ship.cindex].pos.y, Ship.pos.y-650 - (Ship.DeltaY-2048), _TimeDt / 3.f);
      DeltaFunc(Characters[Ship.cindex].beta,  0, TimeDt / 4048.f);
      DeltaFunc(Characters[Ship.cindex].gamma, 0, TimeDt / 4048.f);
    }

    if (Ship.State==2)
    {
      Ship.FTime-=_TimeDt;
      if (Ship.FTime<0) Ship.FTime=0;

      if (Ship.FTime==0)
        if (fabs(Characters[Ship.cindex].pos.y - (Ship.pos.y-650 - (Ship.DeltaY-2048))) < 1.f)
        {
          Ship.State = 1;
          AddVoicev(ShipModel.SoundFX[5].length,
                    ShipModel.SoundFX[5].lpData, 256);
          AddVoice3d(ShipModel.SoundFX[2].length, ShipModel.SoundFX[2].lpData,
                     Ship.pos.x, Ship.pos.y, Ship.pos.z);
        }
      return;
    }
  }
//=====================================//


//====== speed ===============//
  float vspeed = 1.f + L / 128.f;
  if (vspeed > 24) vspeed = 24;
  if (Ship.State) vspeed = 24;
  if (fabs(dalpha) > 0.4) vspeed = 0.f;
  float _s = Ship.speed;
  if (vspeed>Ship.speed) DeltaFunc(Ship.speed, vspeed, TimeDt / 200.f);
  else Ship.speed = vspeed;

  if (Ship.speed>0 && _s==0)
    AddVoice3d(ShipModel.SoundFX[2].length, ShipModel.SoundFX[2].lpData,
               Ship.pos.x, Ship.pos.y, Ship.pos.z);

//====== fly ===========//
  float l = TimeDt * Ship.speed / 16.f;

  if (fabs(dalpha) < 0.4)
    if (l<L)
    {
      if (l>L2) l = L2 * 0.5f;
      if (L2<0.1) l = 0;
      Ship.pos.x += (float)cos(Ship.alpha)*l;
      Ship.pos.z += (float)sin(Ship.alpha)*l;
    }
    else
    {
      if (Ship.State)
      {
        Ship.State = -1;
        RemoveCharacter(Ship.cindex);
        return;
      }
      else
      {
        Ship.pos = Ship.tgpos;
        Ship.State = 3;
        Ship.FTime = 1;
        Ship.tgpos = Ship.retpos;
        Ship.tgpos.y = GetLandUpH(Ship.tgpos.x, Ship.tgpos.z) + Ship.DeltaY;
        Ship.tgpos.y = MAX(Ship.tgpos.y, GetLandUpH(Ship.pos.x, Ship.pos.z) + Ship.DeltaY);
        Characters[Ship.cindex].StateF = 0xFF;
        AddVoice3d(ShipModel.SoundFX[1].length, ShipModel.SoundFX[1].lpData,
                   Ship.pos.x, Ship.pos.y, Ship.pos.z);
      }
    }

//======= y movement ============//
  float h = GetLandUpH(Ship.pos.x, Ship.pos.z);
  DeltaFunc(Ship.pos.y, Ship.tgpos.y, TimeDt / 4.f);
  if (Ship.pos.y < h + 1024)
  {
    if (Ship.State)
      if (Ship.cindex!=-1)
        Characters[Ship.cindex].pos.y+= h + 1024 - Ship.pos.y;
    Ship.pos.y = h + 1024;
  }



//======= rotation ============//

  if (Ship.tgalpha > Ship.alpha) currspeed = 0.1f + (float)fabs(drspd)/2.f;
  else currspeed =-0.1f - (float)fabs(drspd)/2.f;

  if (fabs(dalpha) > pi) currspeed=-currspeed;


  DeltaFunc(Ship.rspeed, currspeed, (float)TimeDt / 420.f);

  float rspd=Ship.rspeed * TimeDt / 1024.f;
  if (fabs(drspd) < fabs(rspd))
  {
    Ship.alpha = Ship.tgalpha;
    Ship.rspeed/=2;
  }
  else
  {
    Ship.alpha+=rspd;
    if (Ship.State)
      if (Ship.cindex!=-1)
        Characters[Ship.cindex].alpha+=rspd;
  }

  if (Ship.alpha<0) Ship.alpha+=pi*2;
  if (Ship.alpha>pi*2) Ship.alpha-=pi*2;

//======== move body ===========//
  if (Ship.State)
  {
    if (Ship.cindex!=-1)
    {
      Characters[Ship.cindex].pos.x = Ship.pos.x;
      Characters[Ship.cindex].pos.z = Ship.pos.z;
    }
    if (L>1000) Ship.tgpos.y+=TimeDt / 12.f;
  }
  else
  {
    Ship.tgpos.x = Characters[Ship.cindex].pos.x;
    Ship.tgpos.z = Characters[Ship.cindex].pos.z;
    Ship.tgpos.y = GetLandUpH(Ship.tgpos.x, Ship.tgpos.z) + Ship.DeltaY;
    Ship.tgpos.y = MAX(Ship.tgpos.y, GetLandUpH(Ship.pos.x, Ship.pos.z) + Ship.DeltaY);
  }



}



void ProcessTrophy()
{
  TrophyBody = -1;

  for (int c=0; c<ChCount; c++)
  {
	  //Vector3d p = Characters[c].pos;
	  Vector3d p;
    //p.x+=Characters[c].lookx * 256*2.5f;
    //p.z+=Characters[c].lookz * 256*2.5f;
	  p.x = Characters[c].xdata;
	  p.z = Characters[c].zdata;
	  p.y = GetLandH(p.x, p.z) + Characters[c].ydata;

	//Characters[c].Phase = 1;

	if (VectorLength(SubVectors(p, PlayerPos)) < 148) {
      TrophyBody = c;
	  TrophyDisplayBody.ctype = TrophyIndex[TrophyRoom.Body[c].ctype];
	  TrophyDisplayBody.scale = TrophyRoom.Body[c].scale;
	  TrophyDisplayBody.weapon = TrophyRoom.Body[c].weapon;
	  TrophyDisplayBody.score = TrophyRoom.Body[c].score;
	  TrophyDisplayBody.phase = TrophyRoom.Body[c].phase;
 	  TrophyDisplayBody.time = TrophyRoom.Body[c].time;
	  TrophyDisplayBody.date = TrophyRoom.Body[c].date;
	  TrophyDisplayBody.range = TrophyRoom.Body[c].range;
	}
  }

  if (TrophyBody==-1) return;

  TrophyBody = Characters[TrophyBody].State;
}



void AnimateElements()
{
  for (int eg=0; eg<ElCount; eg++)
  {

    if  (Elements[eg].Type == partGround)
    {
      int a1 = Elements[eg].RGBA >> 24;
      a1-=TimeDt/4;
      if (a1<0) a1=0;
      Elements[eg].RGBA = (Elements[eg].RGBA  & 0x00FFFFFF) + (a1<<24);
      int a2 = Elements[eg].RGBA2>> 24;
      a2-=TimeDt/4;
      if (a2<0) a2=0;
      Elements[eg].RGBA2= (Elements[eg].RGBA2 & 0x00FFFFFF) + (a2<<24);
      if (a1 == 0 && a2==0) Elements[eg].ECount = 0;
    }

    if  (Elements[eg].Type == partWater)
      if (Elements[eg].EDone == Elements[eg].ECount)
        Elements[eg].ECount = 0;

    if  (Elements[eg].Type == partBubble)
      if (Elements[eg].EDone == Elements[eg].ECount)
        Elements[eg].ECount = 0;

    if  (Elements[eg].Type == partBlood)
      if ((Takt & 3)==0)
        if (Elements[eg].EDone == Elements[eg].ECount)
        {
          int a1 = Elements[eg].RGBA >> 24;
          a1--;
          if (a1<0) a1=0;
          Elements[eg].RGBA = (Elements[eg].RGBA  & 0x00FFFFFF) + (a1<<24);
          int a2 = Elements[eg].RGBA2>> 24;
          a2--;
          if (a2<0) a2=0;
          Elements[eg].RGBA2= (Elements[eg].RGBA2 & 0x00FFFFFF) + (a2<<24);
          if (a1 == 0 && a2==0) Elements[eg].ECount = 0;
        }



//====== remove finished process =========//
    if (!Elements[eg].ECount)
    {
      memcpy(&Elements[eg], &Elements[eg+1], (ElCount+1-eg) * sizeof(TElements));
      ElCount--;
      eg--;
      continue;
    }


    for (int e=0; e<Elements[eg].ECount; e++)
    {
      if (Elements[eg].EList[e].Flags) continue;
      Elements[eg].EList[e].pos.x+=Elements[eg].EList[e].speed.x * TimeDt / 1000.f;
      Elements[eg].EList[e].pos.y+=Elements[eg].EList[e].speed.y * TimeDt / 1000.f;
      Elements[eg].EList[e].pos.z+=Elements[eg].EList[e].speed.z * TimeDt / 1000.f;

      float h;
      h = GetLandUpH(Elements[eg].EList[e].pos.x, Elements[eg].EList[e].pos.z);
      BOOL OnWater = GetLandH(Elements[eg].EList[e].pos.x, Elements[eg].EList[e].pos.z) < h;

      switch (Elements[eg].Type)
      {
      case partBubble:
        Elements[eg].EList[e].speed.y += 2.0 * 256 * TimeDt / 1000.f;
        if (Elements[eg].EList[e].speed.y > 824) Elements[eg].EList[e].speed.y = 824;
        if (Elements[eg].EList[e].pos.y > h)
        {
          AddWCircle(Elements[eg].EList[e].pos.x, Elements[eg].EList[e].pos.z, 0.6);
          Elements[eg].EDone++;
          Elements[eg].EList[e].Flags = 1;
          if (OnWater) Elements[eg].EList[e].pos.y-= 10240;
        }
        break;

      default:
        Elements[eg].EList[e].speed.y -= 9.8 * 256 * TimeDt / 1000.f;
        if (Elements[eg].EList[e].pos.y < h)
        {
          if (OnWater) AddWCircle(Elements[eg].EList[e].pos.x, Elements[eg].EList[e].pos.z, 0.6);
          Elements[eg].EDone++;
          Elements[eg].EList[e].Flags = 1;
          if (OnWater) Elements[eg].EList[e].pos.y-= 10240;
          else Elements[eg].EList[e].pos.y = h + 4;
        }
        break;

      } //== switch ==//

    } // for(e) //
  } // for(eg) //

  for (int b=0; b<BloodTrail.Count; b++)
  {
    BloodTrail.Trail[b].LTime-=TimeDt;
    if (BloodTrail.Trail[b].LTime<=0)
    {
      memcpy(&BloodTrail.Trail[b], &BloodTrail.Trail[b+1], (511-b)*sizeof(TBloodP));
      BloodTrail.Count--;
      b--;
    }
  }
}


void AnimateProcesses()
{
  AnimateElements();

  if ((Takt & 63)==0)
  {
    float al2 = CameraAlpha + siRand(60) * pi / 180.f;
    float c2 = cos(al2);
    float s2 = sin(al2);
    float l = 1024 + rRand(3120);
    float xx = CameraX + s2 * l;
    float zz = CameraZ - c2 * l;
    if (GetLandUpH(xx,zz) > GetLandH(xx,zz)+256)
      AddElements(xx, GetLandH(xx,zz), zz, 4, 6 + rRand(6));
  }

  if (!Multiplayer || Host) {
	  if (Takt & 1)
	  {
		  Wind.alpha += siRand(16) / 4096.f;
		  Wind.speed += siRand(400) / 6400.f;
	  }

	  if (Wind.speed < 4.f) Wind.speed = 4.f;
	  if (Wind.speed > 18.f) Wind.speed = 18.f;
  }
  Wind.nv.x = (float)sin(Wind.alpha);
  Wind.nv.z = (float)-cos(Wind.alpha);
  Wind.nv.y = 0.f;

  if (answtime)
  {
    answtime-=TimeDt;
    if (answtime<=0)
    {
      answtime = 0;
      int r = rRand(128) % 3;
      AddVoice3d(fxCall[answcall-10][r].length,  fxCall[answcall-10][r].lpData,
                 answpos.x, answpos.y, answpos.z);
    }
  }



  if (CallLockTime)
  {
    CallLockTime-=TimeDt;
    if (CallLockTime<0) CallLockTime=0;
  }

  CheckAfraid();
  AnimateShip();
  if (TrophyMode)
    ProcessTrophy();

  for (int w=0; w<WCCount; w++)
  {
    if (WCircles[w].scale > 1)
      WCircles[w].FTime+=(int)(TimeDt*3 / WCircles[w].scale);
    else
      WCircles[w].FTime+=TimeDt*3;
    if (WCircles[w].FTime >= 2000)
    {
      memcpy(&WCircles[w], &WCircles[w+1], sizeof(TWCircle) * (WCCount+1-w) );
      w--;
      WCCount--;
    }
  }



  if (ExitTime)
  {
    ExitTime-=TimeDt;
    if (ExitTime<=0)
    {
      TrophyRoom.Total.time   +=TrophyRoom.Last.time;
      TrophyRoom.Total.smade  +=TrophyRoom.Last.smade;
      TrophyRoom.Total.success+=TrophyRoom.Last.success;
      TrophyRoom.Total.path   +=TrophyRoom.Last.path;

      if (MyHealth) SaveTrophy();
      else LoadTrophy();
      DoHalt("");
    }
  }
}



void RemoveCurrentTrophy()
{
  int p = 0;
  if (!TrophyMode) return;
  if (!TrophyRoom.Body[TrophyBody].ctype) return;

  PrintLog("Trophy removed: ");
  //PrintLog(DinoInfo[TrophyRoom.Body[TrophyBody].ctype].Name);
  PrintLog(DinoInfo[TrophyIndex[TrophyRoom.Body[TrophyBody].ctype]].Name);
  PrintLog("\n");

  for (int c=0; c<TrophyBody; c++)
    if (TrophyRoom.Body[c].ctype) p++;


  TrophyRoom.Body[TrophyBody].ctype = 0;

  if (TrophyMode)
  {
    memcpy(&Characters[p],
           &Characters[p+1],
           (250-p) * sizeof(TCharacter) );
    ChCount--;
  }

  TrophyTime = 0;
  TrophyBody = -1;
}


void LoadTrophy()
{
  int pr = TrophyRoom.RegNumber;
  FillMemory(&TrophyRoom, sizeof(TrophyRoom), 0);
  TrophyRoom.RegNumber = pr;
  DWORD l;
  char fname[128];
  int rn = TrophyRoom.RegNumber;
  wsprintf(fname, "trophy0%d.sav", TrophyRoom.RegNumber);
  HANDLE hfile = CreateFile(fname, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hfile==INVALID_HANDLE_VALUE)
  {
    PrintLog("===> Error loading trophy!\n");
    return;
  }
  ReadFile(hfile, &TrophyRoom, sizeof(TrophyRoom), &l, NULL);

  ReadFile(hfile, &OptAgres, 4, &l, NULL);
  ReadFile(hfile, &OptDens, 4, &l, NULL);
  ReadFile(hfile, &OptSens, 4, &l, NULL);

  if (Multiplayer) OptDens = 128;

  ReadFile(hfile, &OptRes, 4, &l, NULL);
  ReadFile(hfile, &FOGENABLE, 4, &l, NULL);
  ReadFile(hfile, &OptText, 4, &l, NULL);
  ReadFile(hfile, &OptViewR, 4, &l, NULL);
  ReadFile(hfile, &SHADOWS3D, 4, &l, NULL);
  ReadFile(hfile, &OptMsSens, 4, &l, NULL);
  ReadFile(hfile, &OptBrightness, 4, &l, NULL);


  ReadFile(hfile, &KeyMap, sizeof(KeyMap), &l, NULL);
  ReadFile(hfile, &REVERSEMS, 4, &l, NULL);
  //INGORE SAVEFILE SETTING FOR EQUIPMENT
  boolean temp;
  ReadFile(hfile, &temp, 4, &l, NULL);
  ReadFile(hfile, &temp, 4, &l, NULL);
  ReadFile(hfile, &temp, 4, &l, NULL);
  ReadFile(hfile, &temp, 4, &l, NULL);
  ReadFile(hfile, &OPT_ALPHA_COLORKEY, 4, &l, NULL);

  ReadFile(hfile, &OptSys, 4, &l, NULL);
  ReadFile(hfile, &OptSound, 4, &l, NULL);
  ReadFile(hfile, &OptRender, 4, &l, NULL);


  SetupRes();

  CloseHandle(hfile);
  TrophyRoom.RegNumber = rn;

  PrintLog("Trophy Loaded.\n");
//	TrophyRoom.Score = 299;
}




void SaveTrophy()
{
  DWORD l;
  char fname[128];
  wsprintf(fname, "trophy0%d.sav", TrophyRoom.RegNumber);

  int r = TrophyRoom.Rank;
  TrophyRoom.Rank = 0;
  if (TrophyRoom.Score >= 100) TrophyRoom.Rank = 1;
  if (TrophyRoom.Score >= 300) TrophyRoom.Rank = 2;


  HANDLE hfile = CreateFile(fname, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hfile == INVALID_HANDLE_VALUE)
  {
    PrintLog("==>> Error saving trophy!\n");
    return;
  }
  WriteFile(hfile, &TrophyRoom, sizeof(TrophyRoom), &l, NULL);

  WriteFile(hfile, &OptAgres, 4, &l, NULL);
  WriteFile(hfile, &OptDens, 4, &l, NULL);
  WriteFile(hfile, &OptSens, 4, &l, NULL);

  WriteFile(hfile, &OptRes, 4, &l, NULL);
  WriteFile(hfile, &FOGENABLE, 4, &l, NULL);
  WriteFile(hfile, &OptText, 4, &l, NULL);
  WriteFile(hfile, &OptViewR, 4, &l, NULL);
  WriteFile(hfile, &SHADOWS3D, 4, &l, NULL);
  WriteFile(hfile, &OptMsSens, 4, &l, NULL);
  WriteFile(hfile, &OptBrightness, 4, &l, NULL);

  WriteFile(hfile, &KeyMap, sizeof(KeyMap), &l, NULL);
  WriteFile(hfile, &REVERSEMS, 4, &l, NULL);

  WriteFile(hfile, &ScentMode, 4, &l, NULL);
  WriteFile(hfile, &CamoMode, 4, &l, NULL);
  WriteFile(hfile, &RadarMode, 4, &l, NULL);
  WriteFile(hfile, &Tranq, 4, &l, NULL);
  WriteFile(hfile, &OPT_ALPHA_COLORKEY, 4, &l, NULL);

  WriteFile(hfile, &OptSys, 4, &l, NULL);
  WriteFile(hfile, &OptSound, 4, &l, NULL);
  WriteFile(hfile, &OptRender, 4, &l, NULL);
  CloseHandle(hfile);
  PrintLog("Trophy Saved.\n");
}

