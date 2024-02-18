#define _MAIN_
#include "Hunt.h"
#include "stdio.h"

float rav=0;
float rbv=0;

#ifdef _soft
BOOL PHONG = FALSE;
BOOL GOUR  = FALSE;
BOOL ENVMAP = FALSE;
#else
BOOL PHONG = TRUE;
BOOL GOUR  = TRUE;
BOOL ENVMAP = TRUE;
#endif

BOOL NeedRVM = TRUE;
float BinocularPower  = 2.5;
float wpshy = 0;
float wpshz = 0;
float wpnb  = 0;
int wpnlight;

void HideWeapon();

char cheatcode[16] = "DEBUGUP";
int  cheati = 0;


void ResetMousePos()
{
  SetCursorPos(VideoCX, VideoCY);
}



float CalcFogLevel(Vector3d v)
{
  if (!FOGON) return 0;
  BOOL vinfog = TRUE;
  int cf;
  if (!UNDERWATER)
  {
    cf = FogsMap[ ((int)(v.z + CameraZ))>>9 ][ ((int)(v.x + CameraX))>>9 ];
    if ((!cf) && CAMERAINFOG)
    {
      cf = CameraFogI;
      vinfog = FALSE;
    }
  }
  else cf = 127;


  if (! (CAMERAINFOG | cf) ) return 0;
  TFogEntity *fptr;
  fptr = &FogsList[cf];
  CurFogColor = fptr->fogRGB;

  float d = VectorLength(v);

  v.y+=CameraY;

  float fla= -(v.y     - fptr->YBegin*ctHScale) / ctHScale;
  if (!vinfog) if (fla>0) fla=0;

  float flb = -(CameraY - fptr->YBegin*ctHScale) / ctHScale;
  if (!CAMERAINFOG) if (flb>0) flb=0;

  if (fla<0 && flb<0) return 0;

  if (fla<0)
  {
    d*= flb / (flb-fla);
    fla = 0;
  }
  if (flb<0)
  {
    d*= fla / (fla-flb);
    flb = 0;
  }

  float fl = (fla + flb);

  fl *= (d+(fptr->Transp/2)) / fptr->Transp;

  return MIN(fl, fptr->FLimit);
}



void PreCashGroundModel()
{
  SKYDTime = RealTime>>1;
  int x,y;

  int kx = SKYDTime & 255;
  int ky = SKYDTime & 255;
  int SKYDT = SKYDTime>>8;

  int VideoCX16 = VideoCX * 16;
  int VideoCY16 = VideoCY * 16;
  float CameraW16 = CameraW * 16;
  float CameraH16 = CameraH * 16;

  BOOL FogFound = FALSE;
  NeedWater = FALSE;

  MapMinY = 10241024;
  Vector3d rv;


  for (y=-(ctViewR+3); y<(ctViewR+3); y++)
    for (x=-(ctViewR+3); x<(ctViewR+3); x++)
    {

      int r = MAX((MAX(y,-y)), (MAX(x,-x)));
      if ( r>ctViewR1+4 )
        if ( (x & 1) + (y & 1) > 0) continue;

      int xx = (CCX + x) & 1023;
      int yy = (CCY + y) & 1023;

      v[0].x = xx*256 - CameraX;
      v[0].z = yy*256 - CameraZ;
      v[0].y = (float)((int)HMap[yy][xx])*ctHScale - CameraY;


//========= water section ===========//

      //if (RunMode)
      if ((FMap[yy][xx] & fmWaterA)>0)
      {
        rv = v[0];
        rv.y = WaterList[ WMap[yy][xx] ].wlevel*ctHScale - CameraY;

        float wdelta = (float)sin(-pi/2 + RandomMap[yy & 31][xx & 31]/128+RealTime/200.f);

        if ( (FMap[yy][xx] & fmWater) && (r < ctViewR1-4))
        {
          rv.x+=(float)sin(xx+yy + RealTime/200.f) * 16.f;
          rv.z+=(float)sin(pi/2.f + xx+yy + RealTime/200.f) * 16.f;
        }

        rv = RotateVector(rv);
        VMap2[128+y][128+x].v = rv;

        if (fabs(rv.x) > -rv.z + 1524)
        {
          VMap2[128+y][128+x].DFlags = 128;
        }
        else
        {
          NeedWater = TRUE;
          VMap2[128+y][128+x].Light = 168-(int)(wdelta*24);

          float Alpha;
          if (UNDERWATER)
          {
            Alpha =	160 - VectorLength(rv)* 160 / 220 / ctViewR;
            if (Alpha<10) Alpha=10;
          }
          else if (r < ctViewR1+2)
          {
            int wi = WMap[yy][xx];
            Alpha = (float)((WaterList[wi].wlevel - HMap[yy][xx])*2+4)*WaterList[wi].transp;
            Alpha+=VectorLength(rv) / 256;
            Alpha+=wdelta*2;
            if (Alpha<0) Alpha=0;
            Vector3d va = v[0];
            NormVector(va,1.0f);
            va.y=-va.y;
            if (va.y<0) va.y=0;
            Alpha*=6.f/(va.y+0.1f);
            if (Alpha>255) Alpha=255.f;
          }
          else Alpha = 255.f;

          VMap2[128+y][128+x].ALPHA=(int)Alpha;
          VMap2[128+y][128+x].Fog = 0;

          if (rv.z>-256.0) VMap2[128+y][128+x].DFlags=128;
          else
          {
#ifdef _soft
            VMap2[128+y][128+x].scrx = VideoCX - (int)(rv.x / rv.z * CameraW);
            VMap2[128+y][128+x].scry = VideoCY + (int)(rv.y / rv.z * CameraH);

            int DF = 0;
            if (VMap2[128+y][128+x].scrx < 0)     DF+=1;
            if (VMap2[128+y][128+x].scrx > WinEX) DF+=2;
            if (VMap2[128+y][128+x].scry < 0)     DF+=4;
            if (VMap2[128+y][128+x].scry > WinEY) DF+=8;
#else
            VMap2[128+y][128+x].scrx = VideoCX16 - (int)(rv.x / rv.z * CameraW16);
            VMap2[128+y][128+x].scry = VideoCY16 + (int)(rv.y / rv.z * CameraH16);

            int DF = 0;
            if (VMap2[128+y][128+x].scrx < 0)        DF+=1;
            if (VMap2[128+y][128+x].scrx > WinEX*16) DF+=2;
            if (VMap2[128+y][128+x].scry < 0)        DF+=4;
            if (VMap2[128+y][128+x].scry > WinEY*16) DF+=8;
#endif
            VMap2[128+y][128+x].DFlags = DF;

          }
        }
      }



#ifdef _soft
#else
      if (r>ctViewR1-20 && r<ctViewR1+8)
        if ( (x & 1) + (y & 1) > 0)
        {
          float y1;
          float zd = (float)sqrt(v[0].x*v[0].x + v[0].z*v[0].z) / 256.f;
          float k = (zd - (ctViewR1-8)) / 4.f;
          if (k<0) k=0;
          if (k>1) k=1;

          if ((y & 1)==0) y1 = (float)((int)HMap[yy][xx-1]+HMap[yy][xx+1])*ctHScale/2 - CameraY;
          else if ((x & 1)==0) y1 = (float)((int)HMap[yy-1][xx]+HMap[yy+1][xx])*ctHScale/2 - CameraY;
          else
            y1 = (float)((int)HMap[yy-1][xx-1]+HMap[yy+1][xx+1])*ctHScale/2 - CameraY;

          v[0].y = ((v[0].y+2) * (1-k) + (y1+8) * k);
        }
#endif

      rv = RotateVector(v[0]);


      if (fabs(rv.x * FOVK) > -rv.z + 1600)
      {
        VMap[128+y][128+x].v = rv;
        VMap[128+y][128+x].DFlags = 128;
        continue;
      }


      if (HARD3D)
        if (  ((FMap[yy][xx] & fmWater)==0) || UNDERWATER)
          VMap[128+y][128+x].Fog = CalcFogLevel(v[0]);
        else
          VMap[128+y][128+x].Fog = 0;

      VMap[128+y][128+x].ALPHA = 255;

      v[0]=rv;

      if (v[0].z<1024)
        if (FOGENABLE)
          if (FogsMap[yy>>1][xx>>1]) FogFound = TRUE;

      VMap[128+y][128+x].v = v[0];

      int  DF = 0;
      int  db = 0;

      if (v[0].z<256)
      {
        if (Clouds)
        {
          int shmx = (xx + SKYDT) & 127;
          int shmy = (yy + SKYDT) & 127;

          int db1 = SkyMap[shmy * 128 + shmx ];
          int db2 = SkyMap[shmy * 128 + ((shmx+1) & 127) ];
          int db3 = SkyMap[((shmy+1) & 127) * 128 + shmx ];
          int db4 = SkyMap[((shmy+1) & 127) * 128 + ((shmx+1) & 127) ];
          db = (db1 * (256 - kx) + db2 * kx) * (256-ky) +
               (db3 * (256 - kx) + db4 * kx) * ky;
          db>>=17;
          db = db - 40;
          if (db<0) db=0;
          if (db>48) db=48;
        }

        int clt = LMap[yy][xx];
        clt= MAX(64, clt-db);
        VMap[128+y][128+x].Light = clt;
      }



      if (v[0].z>-256.0) DF+=128;
      else
      {

#ifdef _soft
        VMap[128+y][128+x].scrx = VideoCX - (int)(v[0].x / v[0].z * CameraW);
        VMap[128+y][128+x].scry = VideoCY + (int)(v[0].y / v[0].z * CameraH);

        if (VMap[128+y][128+x].scrx < 0)        DF+=1;
        if (VMap[128+y][128+x].scrx > WinEX)    DF+=2;
        if (VMap[128+y][128+x].scry < 0)        DF+=4;
        if (VMap[128+y][128+x].scry > WinEY)    DF+=8;
#else
        VMap[128+y][128+x].scrx = VideoCX16 - (int)(v[0].x / v[0].z * CameraW16);
        VMap[128+y][128+x].scry = VideoCY16 + (int)(v[0].y / v[0].z * CameraH16);

        if (VMap[128+y][128+x].scrx < 0)        DF+=1;
        if (VMap[128+y][128+x].scrx > WinEX*16) DF+=2;
        if (VMap[128+y][128+x].scry < 0)        DF+=4;
        if (VMap[128+y][128+x].scry > WinEY*16) DF+=8;
#endif

      }

      VMap[128+y][128+x].DFlags = DF;
    }

  FOGON = FogFound || UNDERWATER;
}




void AddShadowCircle(int x, int y, int R, int D)
{
  if (UNDERWATER) return;

  int cx = x / 256;
  int cy = y / 256;
  int cr = 1 + R / 256;
  for (int yy=-cr; yy<=cr; yy++)
    for (int xx=-cr; xx<=cr; xx++)
    {
      int tx = (cx+xx)*256;
      int ty = (cy+yy)*256;
      int r = (int)sqrt((float)((tx-x)*(tx-x) + (ty-y)*(ty-y)) );
      if (r>R) continue;
      VMap[cy+yy - CCY + 128][cx+xx - CCX + 128].Light-= D * (R-r) / R;
      if (VMap[cy+yy - CCY + 128][cx+xx - CCX + 128].Light < 32)
        VMap[cy+yy - CCY + 128][cx+xx - CCX + 128].Light = 32;
    }
}





void DrawScene()
{
  dFacesCount = 0;

  ca = (float)cos(CameraAlpha);
  sa = (float)sin(CameraAlpha);

  cb = (float)cos(CameraBeta);
  sb = (float)sin(CameraBeta);

  CCX = ((int)CameraX / 512) * 2;
  CCY = ((int)CameraZ / 512) * 2;

  PreCashGroundModel();

#ifdef _soft
  CreateChRenderList();
#endif

  RenderSkyPlane();

  cb = (float)cos(CameraBeta);
  sb = (float)sin(CameraBeta);


  RenderGround();

  RenderModelsList();

  Render3DHardwarePosts();

  if (NeedWater) RenderWater();

  RenderElements();
}




void DrawOpticCross( int v)
{
  int sx =  VideoCX + (int)(rVertex[v].x / (-rVertex[v].z) * CameraW);
  int sy =  VideoCY - (int)(rVertex[v].y / (-rVertex[v].z) * CameraH);

  if (  (fabs((float)(VideoCX - sx)) > WinW / 2) ||
        (fabs((float)(VideoCY - sy)) > WinH / 4) ) return;

  Render_Cross(sx, sy);
}



void ScanLifeForms()
{
  int li = -1;
  float dm = (float)(ctViewR+2)*256;
  for (int c=0; c<ChCount; c++)
  {
    TCharacter *cptr = &Characters[c];
	if (DinoInfo[cptr->CType].Aquatic) continue;
	if (DinoInfo[cptr->CType].HideBinoc) continue;
    if (!cptr->Health) continue;
    if (cptr->rpos.z > -512) continue;
    float d = (float)sqrt( cptr->rpos.x*cptr->rpos.x + cptr->rpos.y*cptr->rpos.y + cptr->rpos.z*cptr->rpos.z );
    if (d > ctViewR*256) continue;
    float r = (float)(fabs(cptr->rpos.x) + fabs(cptr->rpos.y)) / d;
    if (r > 0.15) continue;
    if (d<dm)
      if (!TraceLook(cptr->pos.x, cptr->pos.y+220, cptr->pos.z,
                     CameraX, CameraY, CameraZ) )
      {

        dm = d;
        li = c;
      }

  }

  if (li==-1) return;
  Render_LifeInfo(li);
}


void DrawPostObjects()
{
  float b;
  TWeapon* wptr = &Weapon;

  Hardware_ZBuffer(FALSE);

  if (DemoPoint.DemoTime) goto SKIPWEAPON;

  GlassL = 0;
  if (BINMODE)
  {
    RenderNearModel(Binocular, 0, 0, 2*(216-72 * BinocularPower), 192,  0,0);
    ScanLifeForms();
    MapMode = FALSE;
  }

  //goto SKIPWIND;
  if (BINMODE || (OPTICMODE && (!WeapInfo[CurrentWeapon].unzoom || Weapon.state==2))) goto SKIPWIND;

  if (!TrophyMode && !SurvivalMode)
    if (!KeyboardState[VK_CAPITAL] & 1)
    {
      BOOL lr = LOWRESTX;
      LOWRESTX = TRUE;
      VideoCX = WinW / 5;
      VideoCY = WinH - (WinH / 3);
      VideoCY = WinH - (WinH * 10 / 23);
      CreateMorphedModel(WindModel.mptr, &WindModel.Animation[0], (int)(Wind.speed*50.f), 1.0);
      RenderNearModel(WindModel.mptr, -10, -37, -96, 192,  CameraAlpha-Wind.alpha,0);

      VideoCX = WinW - (WinW / 5);
      VideoCY = WinH - (WinH * 10 / 23);
      RenderNearModel(CompasModel, +8, -38, -96, 192,  CameraAlpha,0);

      VideoCX = WinW / 2;
      VideoCY = WinH / 2;
      LOWRESTX = lr;
    }

SKIPWIND:


  if (wptr->state == 0) {
	  if (Weapon.BTime) {
		  Weapon.BTime -= TimeDt * 2;
		  if (Weapon.BTime < 0) Weapon.BTime = 0;
	  }
	  goto SKIPWEAPON;
  }

  MapMode = FALSE;

  if (!SurvivalMode) {
	wptr->shakel+= TimeDt / 10000.f;
	if (wptr->shakel > 4.0f) wptr->shakel = 4.0f;
  }

  if (wptr->state == 1)
  {
	  if (UNDERWATER) wptr->FTime += TimeDt/2.f;
	  else wptr->FTime+=TimeDt;
    if (wptr->FTime >= wptr->chinfo[CurrentWeapon].Animation[WeapInfo[CurrentWeapon].getAnim].AniTime)
    {
      wptr->FTime = 0;
      wptr->state = 2;
    }
  }

  if (wptr->state == 4)
  {
	  if (UNDERWATER) wptr->FTime += TimeDt / 2.f;
	  else wptr->FTime += TimeDt;
	  if (wptr->FTime >= wptr->chinfo[CurrentWeapon].Animation[WeapInfo[CurrentWeapon].rldAnim].AniTime)
    {
      wptr->FTime = 0;
      wptr->state = 2;
    }
  }

  if (wptr->state == 2 && wptr->FTime>0)
  {
	  if (UNDERWATER) wptr->FTime += TimeDt / 2.f;
	  else wptr->FTime += TimeDt;
	  if (Muzz && !UNDERWATER) {
		if (wptr->FTime > MuzzModel.Animation[0].AniTime) {
			Muzz = false;
			MuzzFTime = 0;
		} else MuzzFTime = wptr->FTime;
	}

    if (wptr->FTime >= wptr->chinfo[CurrentWeapon].Animation[WeapInfo[CurrentWeapon].shtAnim].AniTime)
    {
      wptr->FTime = 0;
      wptr->state = 2;
	  Muzz = false;
	  MuzzFTime = 0;

	  /*
      if (WeapInfo[CurrentWeapon].Reload && !DEBUG)
        if ( (ShotsLeft[CurrentWeapon] % WeapInfo[CurrentWeapon].Reload) == 0 )
          if ( (ShotsLeft[CurrentWeapon]>0) || (AmmoMag[CurrentWeapon]>0) )
          {
            wptr->state = 4;
            wptr->FTime = 1;
            AddVoicev(wptr->chinfo[CurrentWeapon].SoundFX[3].length,
                      wptr->chinfo[CurrentWeapon].SoundFX[3].lpData, 256);
          }


      if (!ShotsLeft[CurrentWeapon])
        if (AmmoMag[CurrentWeapon])
        {
          AmmoMag[CurrentWeapon]--;
          ShotsLeft[CurrentWeapon] = WeapInfo[CurrentWeapon].Shots;
          if (wptr->chinfo[CurrentWeapon].Animation[3].AniTime)
          {
            wptr->state = 4;
            wptr->FTime = 1;
            AddVoicev(wptr->chinfo[CurrentWeapon].SoundFX[3].length,
                      wptr->chinfo[CurrentWeapon].SoundFX[3].lpData, 256);
          }
        }
		*/
    }
  }

  if (wptr->state == 3)
  {
	  if (UNDERWATER) wptr->FTime += TimeDt / 2.f;
	  else wptr->FTime+=TimeDt;
    if (wptr->FTime >= wptr->chinfo[CurrentWeapon].Animation[WeapInfo[CurrentWeapon].putAnim].AniTime)
    {
      wptr->FTime = 0;
      wptr->state = 0;
      if (CurrentWeapon != TargetWeapon)
      {
        CurrentWeapon = TargetWeapon;
        HideWeapon();
      }
      goto SKIPWEAPON;
    }
  }


/*
  if (!ShotsLeft[CurrentWeapon])
  {
    HideWeapon();
    for (int w=0; w<10; w++)
      if (ShotsLeft[w])
      {
        TargetWeapon=w;
        break;
      }
  }
  */

  int phas;
  switch (wptr->state)
  {
  case 1:
	  phas = WeapInfo[CurrentWeapon].getAnim;
	  break;
  case 2:
	  phas = WeapInfo[CurrentWeapon].shtAnim;
	  break;
  case 3:
	  phas = WeapInfo[CurrentWeapon].putAnim;
	  break;
  case 4:
	  phas = WeapInfo[CurrentWeapon].rldAnim;
	  break;
  }

  CreateMorphedModel(wptr->chinfo[CurrentWeapon].mptr,
                     &wptr->chinfo[CurrentWeapon].Animation[phas], wptr->FTime, 1.0);

  if (Weapon.HoldBreath) {
	  Weapon.BTime += TimeDt;
	  if (Weapon.BTime >= 4000) {
		  Weapon.BTime = 4000;
		  Weapon.HoldBreath = false;
		  if (Weapon.breathPressed==1 && !UNDERWATER) AddVoicev(fxBreathOut.length, fxBreathOut.lpData, 256);
		  Weapon.breathPressed = 2;
	  }
  } else if (Weapon.BTime && !UNDERWATER) {
	  Weapon.BTime -= TimeDt*2;
	  if (Weapon.BTime < 0) Weapon.BTime = 0;
  }
  if (Weapon.HoldBreath) Weapon.breath += 0.05;
  else Weapon.breath -= 0.2;
  if (Weapon.breath > 3.0f) Weapon.breath = 3.0f;
  if (Weapon.breath < 0.f) Weapon.breath = 0.f;

  b = (float)sin((float)RealTime / 300.f) / 100.f;
  float temp = wptr->shakel - wptr->breath;
  if (temp < 0.2f) temp = 0.2f;
  if (temp > 4.0f) temp = 4.0f;
  wpnDAlpha = temp * (float)sin((float)RealTime / 300.f+pi/2) / 200.f;
  wpnDBeta  = temp * (float)sin((float)RealTime / 300.f) / 400.f;
  nv.z = 0;

  //==================== render weapon ===================//


  Vector3d v = Sun3dPos;
  Sun3dPos = RotateVector(Sun3dPos);
  CalcNormals(wptr->chinfo[CurrentWeapon].mptr, wptr->normals);


  if (GOUR)
    CalcGouraud(wptr->chinfo[CurrentWeapon].mptr, wptr->normals);
  else
    for (int c=0; c<1000; c++)
      wptr->chinfo[CurrentWeapon].mptr->VLight[0][c] = 0;

  if (HARD3D) wpnlight = 96 + GetLandLt(PlayerX, PlayerZ) / 4;
  else wpnlight = 200;

  if (Muzz && !UNDERWATER) {
  CreateMorphedModelBetaGamma(MuzzModel.mptr,
	  &MuzzModel.Animation[0], MuzzFTime, 1.0, 0, MuzzGamma);
  RenderNearModel(MuzzModel.mptr, 0, wpshy, wpshz, wpnlight,
	  -wpnDAlpha, -wpnDBeta + wpnb);
  }

  RenderNearModel(wptr->chinfo[CurrentWeapon].mptr, 0, wpshy, wpshz, wpnlight,
                  -wpnDAlpha, -wpnDBeta + wpnb);


#ifdef _soft
#else
  if (PHONG)
  {
    CalcPhongMapping(wptr->chinfo[CurrentWeapon].mptr, wptr->normals);
    RenderModelClipPhongMap(wptr->chinfo[CurrentWeapon].mptr, 0, wpshy, wpshz, -wpnDAlpha, -wpnDBeta+wpnb);
  }

  if (ENVMAP)
  {
    CalcEnvMapping(wptr->chinfo[CurrentWeapon].mptr, wptr->normals);
    RenderModelClipEnvMap(wptr->chinfo[CurrentWeapon].mptr, 0, wpshy, wpshz, -wpnDAlpha, -wpnDBeta+wpnb);
  }
#endif

  Sun3dPos = v;


  //Render_Cross(VideoCX, VideoCY);
  if ((!WeapInfo[CurrentWeapon].Optic || OPTICMODE) && WeapInfo[CurrentWeapon].cross
	  && (!WeapInfo[CurrentWeapon].unzoom || Weapon.state == 2))
	  DrawOpticCross(wptr->chinfo[CurrentWeapon].mptr->VCount-1);

SKIPWEAPON:

  if (ChCallTime)
  {
    ChCallTime-=TimeDt;
    if (ChCallTime<0) ChCallTime=0;
    DrawPicture(WinW - 10 - MenuDinoInfo[TargetCall-10].CallIcon.W, 7,
		MenuDinoInfo[TargetCall-10].CallIcon);
  }

  Hardware_ZBuffer(TRUE);

  if (Weapon.state)
  {
    int y0 = 5;
    if (!SurvivalMode)
    {
      for (int bl=0; bl< ShotsLeft[CurrentWeapon]; bl++)
		  DrawPicture(6 + bl*Weapon.BulletPic[CurrentWeapon].W, y0, Weapon.BulletPic[CurrentWeapon]);
	  y0+=Weapon.BulletPic[CurrentWeapon].H+4;
	  
	  if (AmmoMag[CurrentWeapon])
		  for (int bl=0; bl< MagShotsLeft[CurrentWeapon]; bl++)
			  DrawPicture(6 + bl*Weapon.BulletPic[CurrentWeapon].W, y0, Weapon.BulletPic[CurrentWeapon]);
	}
  }


  if (TrophyMode)
    DrawPicture( VideoCX - TrophyExit.W / 2, 2, TrophyExit);

  if (EXITMODE) {
	  DrawPicture((WinW - ExitPic.W) / 2, (WinH - ExitPic.H) / 2, ExitPic);
	  if (SurvivalMode) {
		  DrawSurvivalText(
			  (WinW - ExitPic.W) / 2,
			  (WinH - ExitPic.H) / 2
		  );
	  }
  }

  if (PAUSE)
    DrawPicture( (WinW - PausePic.W) / 2, (WinH - PausePic.H) / 2, PausePic);

  if (ScoreDispTime) {

	  int x0 = VideoCX - ScorePic.W /2;
	  int y0 = WinH - ScorePic.H - 12;
	  DrawPicture(x0, y0, ScorePic);
	  DrawScoreText(x0, y0);

	  if (ScoreDispTime)
	  {
		  ScoreDispTime -= TimeDt;
		  if (ScoreDispTime < 0)
			  ScoreDispTime = 0;
	  }

  } else {
	  if (TrophyMode || TrophyDisplay)
		  if (TrophyBody != -1 || TrophyDisplay)
		  {
			  TPicture *Pic = &TrophyPic;
			  if (!TrophyMode && (Tranq || Characters[TrophyDisplayC].claimed)) {
				  Pic = &TrophyNoCollectPic;
			  }
			  int x0 = WinW - Pic->W - 16;
			  int y0 = WinH - Pic->H - 12;
			  if (!TrophyMode)
				  x0 = VideoCX - Pic->W / 2;

			  DrawPicture(x0, y0, *Pic);
			  DrawTrophyText(x0, y0);

		  }
  }
  
}





void SwitchMode(LPSTR lps, BOOL& b)
{
  b = !b;
  char buf[200];
  if (b) wsprintf(buf,"%s is ON", lps);
  else wsprintf(buf,"%s is OFF", lps);
  MessageBeep(0xFFFFFFFF);
  AddMessage(buf);
}


void ChangeViewR(int d1, int d2, int d3)
{
  char buf[200];
  ctViewR +=d1;
  ctViewR1+=d2;
  ctViewRM+=d3;
  if (ctViewR<20) ctViewR = 20;
  if (ctViewR>122) ctViewR = 122;

  if (ctViewR1 < 12) ctViewR1=12;
  if (ctViewR1 > ctViewR-10) ctViewR1=ctViewR-10;
  if (ctViewRM <  4) ctViewRM = 4;
  if (ctViewRM > 60) ctViewRM = 60;

  wsprintf(buf,"ViewR = %d (%d + %d) BMP at %d", ctViewR, ctViewR1, ctViewR-ctViewR1, ctViewRM);
  //MessageBeep(0xFFFFFFFF);
  AddMessage(buf);

}


void ChangeCall()
{
  if (!TargetDino) return;
  if (ChCallTime)
    for (int t=0; t<32; t++)
    {
      TargetCall++;
      if (TargetCall>32) TargetCall=10;
      if (TargetDino & (1<<TargetCall)) break;
    }
  //wsprintf(logt,"Call: %s", DinoInfo[ AI_to_CIndex[TargetCall] ].Name);
  //AddMessage(logt);
  //CallLockTime+= 1024;
  ChCallTime = 2048;
}

void ToggleBinocular()
{
  if (Weapon.state) return;
  if (UNDERWATER) return;
  if (!MyHealth) return;
  BINMODE = !BINMODE;
  MapMode = FALSE;
}


void ToggleRunMode()
{
  RunMode = !RunMode;
  if (RunMode) AddMessage("Run mode is ON");
  else AddMessage("Run mode is OFF");
}

void ToggleCrouchMode()
{
	CrouchMode = !CrouchMode;
	if (CrouchMode) AddMessage("Crouch mode is ON");
	else AddMessage("Crouch mode is OFF");
}


void ToggleMapMode()
{
  if (!MyHealth) return;
  if (BINMODE) return;
  if (Weapon.state) return;
  MapMode = !MapMode;
}




void ShowShifts()
{
  sprintf(logt, "Y=%3.4f  Z=%3.4f  A=%3.4f", wpshy/2, wpshz/2, wpnb*180/3.1415);
  AddMessage(logt);
}

LONG APIENTRY MainWndProc( HWND hWnd, UINT message, UINT wParam, LONG lParam)
{
  BOOL A = (GetActiveWindow() == hWnd);

  if (A!=blActive)
  {
    blActive = A;

    if (blActive) SetPriorityClass( GetCurrentProcess(), HIGH_PRIORITY_CLASS);
    else SetPriorityClass( GetCurrentProcess(), IDLE_PRIORITY_CLASS);

    if (!blActive)
    {
      ShutDown3DHardware();
      NeedRVM = TRUE;
    }

    if (blActive)
    {
      Audio_Restore();
      NeedRVM = TRUE;
    }

  }

  if (message == WM_KEYDOWN && !SurvivalMode)
  {
    if ((int)wParam == KeyMap.fkBinoc) ToggleBinocular();
    if ((int)wParam == KeyMap.fkCCall) ChangeCall();
    if ((int)wParam == KeyMap.fkRun  ) ToggleRunMode();
	if ((int)wParam == KeyMap.fkCrouch) ToggleCrouchMode();
    if ((int)wParam == cheatcode[cheati])
    {
      cheati++;
      if (cheati>6)
      {
        cheati=0;
        SwitchMode("Debug mode",DEBUG);
      }
    }
    else cheati=0;
  }


  switch (message)
  {
  case WM_CREATE:
    return 0;


  case WM_KEYDOWN:
  {
    BOOL CTRL = (GetKeyState(VK_SHIFT) & 0x8000);
    switch( (int)wParam )
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    {
		if (SurvivalMode) break;
      if (Weapon.FTime) break;
      int w;
      if (wParam == '0')
        w = 9;
      else
        w = ((int)wParam - '1');
      if (!ShotsLeft[w] && !AmmoMag[w])
      {
        AddMessage("No weapon");
        break;
      }
      TargetWeapon = w;
	  if (!UNDERWATER || WeapInfo[TargetWeapon].harpoon) {
		  if (!Weapon.state)
			  CurrentWeapon = TargetWeapon;
		  HideWeapon();
	  }
      break;
    }

    case 'U':
      if (DEBUG) ChangeViewR(0, 0, -2);
      break;
    case 'I':
      if (DEBUG) ChangeViewR(0, 0, +2);
      break;
    case 'O':
      if (DEBUG) ChangeViewR(0, -2, 0);
      break;
    //case 'P': if (DEBUG) ChangeViewR(0, +2, 0); break;
    case 219:
      if (DEBUG) ChangeViewR(-2, 0, 0);
      break;
    case 221:
      if (DEBUG) ChangeViewR(+2, 0, 0);
      break;

    /*
    case '0': wpshy=0; wpshz=0; wpnb=0; break;
    case '7': if (CTRL) wpshy-=0.25; else wpshy+=0.25;
           ShowShifts();
           break;
    case '8': if (CTRL) wpshz-=0.25; else wpshz+=0.25;
           ShowShifts();
           break;
    case '9': if (CTRL) wpnb-=0.005; else wpnb+=0.005;
           ShowShifts();
           break;*/


    case 'S':
      if (DEBUG && CTRL) SwitchMode("Slow mode",SLOW);
      break;
    case 'T':
      if (DEBUG && CTRL) SwitchMode("Timer",TIMER);
      break;


    case 'M':
      if (CTRL) SwitchMode("Draw 3D models",MODELS);
      break;
    case 'F':
      if (CTRL) SwitchMode("V.Fog",FOGENABLE);
      break;
    case 'L':
      if (CTRL) SwitchMode("Fly",FLY);
      break;
    case 'C':
      if (CTRL) SwitchMode("Clouds shadow",Clouds);
      break;

    case 'E':
      if (CTRL) SwitchMode("Env.Mapping",ENVMAP);
      break;
    case 'G':
      if (CTRL) SwitchMode("Gour.Mapping",GOUR);
      break;
    case 'P':
		if (!CTRL) { if (DEBUG) ChangeViewR(0, +2, 0); } else  SwitchMode("Phong Mapping", PHONG);
		break;

//	case VK_UP:

//	case VK_RIGHT:

//	case VK_LEFT:

//	case VK_DOWN:

    case VK_TAB:
      if (!TrophyMode) ToggleMapMode();
      break;

    case VK_PAUSE:
		if (!SurvivalMode) {
      PAUSE = !PAUSE;
      EXITMODE = FALSE;
      ResetMousePos();
      break;
		}

    case 'N':
      if (EXITMODE) EXITMODE = FALSE;
      break;

    case VK_ESCAPE:
      if (TrophyMode || SurvivalMode)
      {
        SaveTrophy();
        ExitTime = 1;
      }
      else
      {
        if (PAUSE) PAUSE = FALSE;
        else EXITMODE = !EXITMODE;
        if (ExitTime) EXITMODE = FALSE;
        ResetMousePos();
      }
      break;

    case 'Y':
		if (EXITMODE && !SurvivalMode)
		{
			if (MyHealth) ExitTime = 4000;
			else ExitTime = 1;
			EXITMODE = FALSE;
		}
		break;

    case VK_RETURN:
      if (EXITMODE )
      {
		if (MyHealth && !SurvivalMode) ExitTime = 4000;
        else ExitTime = 1;
        EXITMODE = FALSE;
      }
      break;

	case 'Q':
		if (EXITMODE && SurvivalMode)
		{
			ExitTime = 1;
			EXITMODE = FALSE;
		}
		break;

    case 'R':
	if (TrophyBody!=-1) RemoveCurrentTrophy();
      if (EXITMODE)
      {
		  if (SurvivalMode) {
			  SurvivalWave = 0;
			  ChCount = 0;
		  }
		  else LoadTrophy();
        RestartMode = TRUE;
		
        _GameState = 0;
        //DoHalt("");
      }
      break;

    case VK_F9:
      ShutDown3DHardware();
      AudioStop();
      DoHalt("");
      break;

    case VK_F12:
      SaveScreenShot();
      break;

    }   // switch
    break;
  }

  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  /*
  case WM_PAINT:
  case WM_ERASEBKGND:
  case WM_NCPAINT   : break;
  */
  /*case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC  hdc =  BeginPaint(hWnd, &ps );
    EndPaint(hWnd, &ps);
    return 0;
  } */
  default:
    return (DefWindowProc(hWnd, message, wParam, lParam));
  }
  return 0;
}




BOOL CreateMainWindow()
{
  PrintLog("Creating main window...");
  WNDCLASS wc;
  wc.style = CS_OWNDC;
  wc.lpfnWndProc = (WNDPROC)MainWndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInst;
  wc.hIcon = wc.hIcon = (HICON)LoadIcon(hInst,"ACTION");
  wc.hCursor = NULL;
  wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
  wc.lpszMenuName = NULL;
  //wc.lpfnWndProc  = NULL;
  wc.lpszClassName = "HuntRenderWindow";
  if (!RegisterClass(&wc)) return FALSE;

  hwndMain = CreateWindow(
               "HuntRenderWindow","Carnivores 2 Renderer",
               WS_VISIBLE |  WS_POPUP,
               0, 0, 0, 0, NULL,  NULL, hInst, NULL );

  if (hwndMain)
    PrintLog("Ok.\n");

  return TRUE;
}




void ProcessReload() {

	TWeapon *wptr = &Weapon;
	
	if (wptr->state == 2 && wptr->FTime == 0)
		if (WeapInfo[CurrentWeapon].Reload) {
			if (Chambered[CurrentWeapon] < WeapInfo[CurrentWeapon].Reload &&
				ShotsLeft[CurrentWeapon]) {
				Chambered[CurrentWeapon] = WeapInfo[CurrentWeapon].Reload;

				if (wptr->chinfo[CurrentWeapon].Animation[3].AniTime)
				{
					wptr->state = 4;
					wptr->FTime = 1;
					if (UNDERWATER) {
						if (WeapInfo[CurrentWeapon].rldAqSnd >= 0)
							AddVoicev(wptr->chinfo[CurrentWeapon].SoundFX[WeapInfo[CurrentWeapon].rldAqSnd].length,
								wptr->chinfo[CurrentWeapon].SoundFX[WeapInfo[CurrentWeapon].rldAqSnd].lpData, 256);
					}
					else {
						int fx = wptr->chinfo[CurrentWeapon].Anifx[WeapInfo[CurrentWeapon].rldAnim];
						if (fx) AddVoicev(wptr->chinfo[CurrentWeapon].SoundFX[fx].length,
							wptr->chinfo[CurrentWeapon].SoundFX[fx].lpData, 256);
					}
				}
			}
			
		} else if (AmmoMag[CurrentWeapon]) {
			int temp = MagShotsLeft[CurrentWeapon];
			MagShotsLeft[CurrentWeapon] = ShotsLeft[CurrentWeapon];
			ShotsLeft[CurrentWeapon] = temp;

			if (!MagShotsLeft[CurrentWeapon]) AmmoMag[CurrentWeapon]--;

			if (wptr->chinfo[CurrentWeapon].Animation[3].AniTime)
			{
				wptr->state = 4;
				wptr->FTime = 1;
				if (UNDERWATER) {
					if (WeapInfo[CurrentWeapon].rldAqSnd >= 0)
						AddVoicev(wptr->chinfo[CurrentWeapon].SoundFX[WeapInfo[CurrentWeapon].rldAqSnd].length,
							wptr->chinfo[CurrentWeapon].SoundFX[WeapInfo[CurrentWeapon].rldAqSnd].lpData, 256);
				}
				else {
					int fx = wptr->chinfo[CurrentWeapon].Anifx[WeapInfo[CurrentWeapon].rldAnim];
					if (fx) AddVoicev(wptr->chinfo[CurrentWeapon].SoundFX[fx].length,
						wptr->chinfo[CurrentWeapon].SoundFX[fx].lpData, 256);
				}
			}
		}

}



void ProcessShoot()
{
  //if (HeadBackR) return;
	int clickNo = rRand(2);
	if (!ShotsLeft[CurrentWeapon]) {
		if (!alreadyFired && !UNDERWATER) AddVoicev(fxClick[clickNo].length, fxClick[clickNo].lpData, 256);
		return;
	}
  if (WeapInfo[CurrentWeapon].Reload && !DEBUG)
	  if (!Chambered[CurrentWeapon]) {
		  if (!alreadyFired && !UNDERWATER) AddVoicev(fxClick[clickNo].length, fxClick[clickNo].lpData, 256);
		  return;
	  }

  TWeapon *wptr = &Weapon;
  if (UNDERWATER && !WeapInfo[CurrentWeapon].harpoon)
  {
    HideWeapon();
    return;
  }

  if (wptr->state == 2 && wptr->FTime==0)
  {
    wptr->FTime = 1;
    HeadBackR=64;
	Recoil.y = -(float)WeapInfo[CurrentWeapon].recoil / 100.f;
	float rx = rRand(8);
	rx -= 4;
	rx /= 8;
	rx *= (float)WeapInfo[CurrentWeapon].recoil / 100.f;
	Recoil.x += rx;

	if (UNDERWATER) {
		if (WeapInfo[CurrentWeapon].shtAqSnd >= 0)
			AddVoicev(wptr->chinfo[CurrentWeapon].SoundFX[WeapInfo[CurrentWeapon].shtAqSnd].length,
				wptr->chinfo[CurrentWeapon].SoundFX[WeapInfo[CurrentWeapon].shtAqSnd].lpData, 256);
	}
	else {
		int fx = wptr->chinfo[CurrentWeapon].Anifx[WeapInfo[CurrentWeapon].shtAnim];
		if (fx) AddVoicev(wptr->chinfo[CurrentWeapon].SoundFX[fx].length,
			wptr->chinfo[CurrentWeapon].SoundFX[fx].lpData, 256);
	}
	
	TrophyRoom.Last.smade++;

	if (WeapInfo[CurrentWeapon].MuzzFlash) {
		Muzz = true;
		MuzzGamma = rRand(100);
		MuzzGamma /= 50;
		MuzzGamma *= pi;
	}

    for (int s=0; s<=WeapInfo[CurrentWeapon].TraceC; s++)
    {
      float rA = 0;
      float rB = 0;

	  if (UNDERWATER) {
		  rA = siRand(128) * 0.00010 * (2.f - WeapInfo[CurrentWeapon].PrecAq);
		  rB = siRand(128) * 0.00010 * (2.f - WeapInfo[CurrentWeapon].PrecAq);
	  }else{
		  rA = siRand(128) * 0.00010 * (2.f - WeapInfo[CurrentWeapon].Prec);
		  rB = siRand(128) * 0.00010 * (2.f - WeapInfo[CurrentWeapon].Prec);
	  }


      float ca = (float)cos(PlayerAlpha + wpnDAlpha + rA);
      float sa = (float)sin(PlayerAlpha + wpnDAlpha + rA);
      float cb = (float)cos(PlayerBeta + wpnDBeta + rB);
      float sb = (float)sin(PlayerBeta + wpnDBeta + rB);

      nv.x=sa;
      nv.y=0;
      nv.z=-ca;

      nv.x*=cb;
      nv.y=-sb;
      nv.z*=cb;

	  float v = WeapInfo[CurrentWeapon].Veloc;
	  if (UNDERWATER) v = WeapInfo[CurrentWeapon].VelocAq;
	  float l = WeapInfo[CurrentWeapon].Veloc;
	  if (WeapInfo[CurrentWeapon].aqLow) l = WeapInfo[CurrentWeapon].VelocAq;

      AddBullet(PlayerX, PlayerY+HeadY, PlayerZ,
               nv.x * 64* v,
               nv.y * 64* v,
	           nv.z * 64* v,
		  nv.x * 64 * l,
		  nv.y * 64 * l,
		  nv.z * 64 * l,
			   CurrentWeapon
			   );
    }

	//Multiplayer)
	sendGunShot = CurrentWeapon;
	

    Vector3d v;
    v.x = PlayerX;
    v.y = PlayerY;
    v.z = PlayerZ;
    if (!UNDERWATER) MakeNoise(v, ctViewR*200 * WeapInfo[CurrentWeapon].Loud);
	if (WeapInfo[CurrentWeapon].Reload) Chambered[CurrentWeapon]--;
    if (!SurvivalMode) ShotsLeft[CurrentWeapon]--;
	else if (WeapInfo[CurrentWeapon].Reload) {
		ShotsLeft[CurrentWeapon]--;
		if (!ShotsLeft[CurrentWeapon]) ShotsLeft[CurrentWeapon] = WeapInfo[CurrentWeapon].Shots;
	}
  }
}


void ProcessSlide()
{
  if (NOCLIP || UNDERWATER) return;
  float ch = GetLandQHNoObj(PlayerX, PlayerZ);
  float mh = ch;
  float chh;
  int   sd = 0;

  chh=GetLandQHNoObj(PlayerX - 16, PlayerZ);
  if (chh<mh)
  {
    mh = chh;
    sd = 1;
  }
  chh=GetLandQHNoObj(PlayerX + 16, PlayerZ);
  if (chh<mh)
  {
    mh = chh;
    sd = 2;
  }
  chh=GetLandQHNoObj(PlayerX, PlayerZ - 16);
  if (chh<mh)
  {
    mh = chh;
    sd = 3;
  }
  chh=GetLandQHNoObj(PlayerX, PlayerZ + 16);
  if (chh<mh)
  {
    mh = chh;
    sd = 4;
  }

  chh=GetLandQHNoObj(PlayerX - 12, PlayerZ - 12);
  if (chh<mh)
  {
    mh = chh;
    sd = 5;
  }
  chh=GetLandQHNoObj(PlayerX + 12, PlayerZ - 12);
  if (chh<mh)
  {
    mh = chh;
    sd = 6;
  }
  chh=GetLandQHNoObj(PlayerX - 12, PlayerZ + 12);
  if (chh<mh)
  {
    mh = chh;
    sd = 7;
  }
  chh=GetLandQHNoObj(PlayerX + 12, PlayerZ + 12);
  if (chh<mh)
  {
    mh = chh;
    sd = 8;
  }

  if (!NOCLIP)
    if (mh<ch-16)
    {
      float delta = (ch-mh) / 4;
      if (sd == 1)
      {
        PlayerX -= delta;
      }
      if (sd == 2)
      {
        PlayerX += delta;
      }
      if (sd == 3)
      {
        PlayerZ -= delta;
      }
      if (sd == 4)
      {
        PlayerZ += delta;
      }

      delta*=0.7f;
      if (sd == 5)
      {
        PlayerX -= delta;
        PlayerZ -= delta;
      }
      if (sd == 6)
      {
        PlayerX += delta;
        PlayerZ -= delta;
      }
      if (sd == 7)
      {
        PlayerX -= delta;
        PlayerZ += delta;
      }
      if (sd == 8)
      {
        PlayerX += delta;
        PlayerZ += delta;
      }
    }
}



void ProcessPlayerMovement()
{

  POINT ms;

  GetCursorPos(&ms);
  if (REVERSEMS) ms.y = -ms.y+VideoCY*2;
  rav += (float)(ms.x-VideoCX) * (OptMsSens+64) / 600.f / 192.f;
  rbv += (float)(ms.y-VideoCY) * (OptMsSens+64) / 600.f / 192.f;
  if (KeyFlags & kfStrafe)
    SSpeed+= (float)rav * 10;
  else
    PlayerAlpha += rav;
  PlayerBeta  += rbv;

  rav/=(2.f + (float)TimeDt/20.f);
  rbv/=(2.f + (float)TimeDt/20.f);
  ResetMousePos();



  if ( !(KeyFlags & (kfForward | kfBackward)))
    if (VSpeed>0) VSpeed=MAX(0,VSpeed-DeltaT*2);
    else VSpeed=MIN(0,VSpeed+DeltaT*2);

  if ( !(KeyFlags & (kfSLeft | kfSRight)))
    if (SSpeed>0) SSpeed=MAX(0,SSpeed-DeltaT*2);
    else SSpeed=MIN(0,SSpeed+DeltaT*2);

  if (KeyFlags & kfForward)  if (VSpeed>0) VSpeed+=DeltaT;
    else VSpeed+=DeltaT*4;
  if (KeyFlags & kfBackward) if (VSpeed<0) VSpeed-=DeltaT;
    else VSpeed-=DeltaT*4;

  if (KeyFlags & kfSRight )  if (SSpeed>0) SSpeed+=DeltaT;
    else SSpeed+=DeltaT*4;
  if (KeyFlags & kfSLeft  )  if (SSpeed<0) SSpeed-=DeltaT;
    else SSpeed-=DeltaT*4;


  if (SWIM)
  {
    if (VSpeed > 0.25f) VSpeed = 0.25f;
    if (VSpeed <-0.25f) VSpeed =-0.25f;
    if (SSpeed > 0.25f) SSpeed = 0.25f;
    if (SSpeed <-0.25f) SSpeed =-0.25f;
  }
  if ( RunMode && (HeadY == 220.f) && (Weapon.state==0))
  {
    if (VSpeed > 0.7f) VSpeed = 0.7f;
    if (VSpeed <-0.7f) VSpeed =-0.7f;
    if (SSpeed > 0.7f) SSpeed = 0.7f;
    if (SSpeed <-0.7f) SSpeed =-0.7f;
  }
  else
  {
    if (VSpeed > 0.3f) VSpeed = 0.3f;
    if (VSpeed <-0.3f) VSpeed =-0.3f;
    if (SSpeed > 0.30f) SSpeed = 0.30f;
    if (SSpeed <-0.30f) SSpeed =-0.30f;
  }

  if (KeyboardState[KeyMap.fkFire] & 128) {
	  ProcessShoot();
	  alreadyFired = true;
  } else alreadyFired = false;

  if (KeyboardState[KeyMap.fkUp] & 128) ProcessReload();

  //menu option/already used check needed - TODO
  if (KeyboardState[KeyMap.fkDown] & 128) AddShipSupply(PlayerX,PlayerZ);

  if (Weapon.state) {
	  if (KeyboardState[KeyMap.fkLeft] & 128 && !UNDERWATER) {
		  if (Weapon.breathPressed == 0) {
			  AddVoicev(fxBreathIn.length, fxBreathIn.lpData, 256);
			  Weapon.breathPressed = 1;
		  }
		  if (!Weapon.HoldBreath) {
			  Weapon.HoldBreath = true;
		  }
	  }
	  else {
		  if (Weapon.HoldBreath) {
			  Weapon.HoldBreath = false;
			  if (Weapon.breathPressed == 1 && !UNDERWATER) AddVoicev(fxBreathOut.length, fxBreathOut.lpData, 256);
		  }
		  Weapon.breathPressed = 0;
	  }
  }

  if (Multiplayer && Host) {
	  for (int c = 0; c < 6; c++) {
		  if (mDamage[0][c]) {
			  Characters[c].Health -= mDamage[0][c];
			  mDamage[0][c] = 0;
			  if (Characters[c].Health < 0) Characters[c].Health = 0;
			  registerDamage(c);
		  }
	  }
  }

  if (KeyboardState[VK_RETURN] & 128) if (TrophyDisplay && !ScoreDispTime && !Characters[TrophyDisplayC].claimed && !Tranq) AddShipTask(TrophyDisplayC);

  if (KeyboardState [KeyMap.fkShow] & 128) HideWeapon();

  if (BINMODE)
  {
    if (KeyboardState[VK_ADD     ] & 128) BinocularPower+=BinocularPower * TimeDt / 4000.f;
    if (KeyboardState[VK_SUBTRACT] & 128) BinocularPower-=BinocularPower * TimeDt / 4000.f;
    if (BinocularPower < 1.5f) BinocularPower = 1.5f;
    if (BinocularPower > 3.0f) BinocularPower = 3.0f;
  }

  if (KeyFlags & kfCall) MakeCall();

  if (DEBUG)
    if (KeyboardState [VK_CONTROL] & 128)
      if (KeyFlags & kfBackward) VSpeed =-8;
      else VSpeed = 8;

  if (KeyFlags & kfJump)
    if (YSpeed == 0 && !SWIM)
    {
      YSpeed = 600 + (float)fabs(VSpeed) * 600;
      AddVoicev(fxJump.length, fxJump.lpData, 256);
    }

//=========  rotation =========//
  if (KeyFlags & kfRight)  PlayerAlpha+=DeltaT*1.5f;
  if (KeyFlags & kfLeft )  PlayerAlpha-=DeltaT*1.5f;
//  if (KeyFlags & kfLookUp) PlayerBeta-=DeltaT;
//  if (KeyFlags & kfLookDn) PlayerBeta+=DeltaT;

//========= movement ==========//

  ca = (float)cos(PlayerAlpha);
  sa = (float)sin(PlayerAlpha);
  cb = (float)cos(PlayerBeta);
  sb = (float)sin(PlayerBeta);

  nv.x=sa;
  nv.y=0;
  nv.z=-ca;


  PlayerNv = nv;
  if (UNDERWATER || FLY)
  {
    nv.x*=cb;
    nv.y=-sb;
    nv.z*=cb;
    PlayerNv = nv;
  }
  else
  {
    PlayerNv.x*=cb;
    PlayerNv.y=-sb;
    PlayerNv.z*=cb;
  }

  Vector3d sv = nv;
  nv.x*=(float)TimeDt*VSpeed;
  nv.y*=(float)TimeDt*VSpeed;
  nv.z*=(float)TimeDt*VSpeed;

  sv.x*=(float)TimeDt*SSpeed;
  sv.y=0;
  sv.z*=(float)TimeDt*SSpeed;

  if (!TrophyMode)
  {
    TrophyRoom.Last.path+=(TimeDt*VSpeed) / 128.f;
    TrophyRoom.Last.time+=TimeDt/1000.f;
  }

//if (SWIM & (VSpeed>0.1) & (sb>0.60)) HeadY-=40;

  int mvi = 1 + TimeDt / 16;

  for (int mvc = 0; mvc<mvi; mvc++)
  {
    PlayerX+=nv.x / mvi;
    PlayerY+=nv.y / mvi;
    PlayerZ+=nv.z / mvi;

    PlayerX-=sv.z / mvi;
    PlayerZ+=sv.x / mvi;

    if (!NOCLIP) CheckCollision(PlayerX, PlayerZ);

    if (PlayerY <= GetLandQHNoObj(PlayerX, PlayerZ)+16)
    {
      ProcessSlide();
      ProcessSlide();
    }
  }

  if (PlayerY <= GetLandQHNoObj(PlayerX, PlayerZ)+16)
  {
    ProcessSlide();
    ProcessSlide();
  }
//===========================================================
}


void ProcessDemoMovement()
{
  BINMODE = FALSE;

  PAUSE = FALSE;
  MapMode = FALSE;

  if (DemoPoint.DemoTime>6*1000)
    if (!PAUSE)
    {
      EXITMODE = TRUE;
      ResetMousePos();
    }

  if (DemoPoint.DemoTime>12*1000)
  {
    //ResetMousePos();
    //DemoPoint.DemoTime = 0;
    //LoadTrophy();
    DoHalt("");
    return;
  }

  VSpeed = 0.f;

  /*
  DemoPoint.pos = Characters[DemoPoint.CIndex].pos;
  DemoPoint.pos.y+=256;

  float base = 824;
  if (killerDino) {
	  if (killerDino->Clone == AI_TREX && !killedwater)
	  {
		  DemoPoint.pos.y += 512;
		  base = 1424;
	  }
	  if (killerDino->Clone == AI_BRACHDANGER || killerDino->Clone == AI_LANDBRACH)
	  {
		  DemoPoint.pos.y += 850;
		  base = 1424;
	  }

  }
  */

  DemoPoint.pos = Characters[DemoPoint.CIndex].pos;
  float base;
  if (killerDino) {
	  base = DinoInfo[killerDino->CType].camBase;
	  if (killedwater) {
		  base = DinoInfo[killerDino->CType].camBaseWater;
		  DemoPoint.pos.y += DinoInfo[killerDino->CType].camDemoPointWater;
	  }
	  else {
		  DemoPoint.pos.y += DinoInfo[killerDino->CType].camDemoPoint;
	  }
  } else base = 824; //for drowning/poison fog
  

  //if (Characters[DemoPoint.CIndex].Clone ==AI_TREX) DemoPoint.pos.y+=512;


  Vector3d nv = SubVectors(DemoPoint.pos,  CameraPos);
  Vector3d pp = DemoPoint.pos;
  pp.y = CameraPos.y;
  float l = VectorLength( SubVectors(pp,  CameraPos) );

  if (DemoPoint.DemoTime==1)
    if (l < base) DemoPoint.DemoTime = 2;
  NormVector(nv, 1.0f);

  if (DemoPoint.DemoTime == 1)
  {
    DeltaFunc(CameraX, DemoPoint.pos.x, (float)fabs(nv.x) * TimeDt * 3.f);
    DeltaFunc(CameraZ, DemoPoint.pos.z, (float)fabs(nv.z) * TimeDt * 3.f);
  }
  else
  {
    DemoPoint.DemoTime+=TimeDt;
    CameraAlpha+=TimeDt / 1224.f;
    ca = (float)cos(CameraAlpha);
    sa = (float)sin(CameraAlpha);
    //float k = (base - l) / 350.f;
    DeltaFunc(CameraX, DemoPoint.pos.x  - sa * base, (float)TimeDt );
    DeltaFunc(CameraZ, DemoPoint.pos.z  + ca * base, (float)TimeDt );
  }

  float b = FindVectorAlpha( (float)
                             sqrt ( (DemoPoint.pos.x - CameraX)*(DemoPoint.pos.x - CameraX) +
                                    (DemoPoint.pos.z - CameraZ)*(DemoPoint.pos.z - CameraZ) ),
                             DemoPoint.pos.y - CameraY - 400.f);
  if (b>pi) b = b - 2*pi;
  DeltaFunc(CameraBeta, -b, TimeDt / 4000.f);



  float h = GetLandQH(CameraX, CameraZ);
  DeltaFunc(CameraY, h+128, TimeDt / 8.f);
  if (CameraY < h + 80) CameraY = h + 80;
}




void ProcessControls()
{
  int _KeyFlags = KeyFlags;
  KeyFlags = 0;
  GetKeyboardState(KeyboardState);

  
  if (KeyboardState[KeyMap.fkUp] & 128)  KeyFlags += kfLookUp;
  if (KeyboardState[KeyMap.fkDown] & 128)  KeyFlags += kfLookDn;

  if (!SurvivalMode) {
  if (KeyboardState [KeyMap.fkStrafe] & 128) KeyFlags+=kfStrafe;

	if (KeyboardState [KeyMap.fkForward ] & 128) KeyFlags+=kfForward;
	if (KeyboardState [KeyMap.fkBackward] & 128) KeyFlags+=kfBackward;
	//if (KeyboardState[KeyMap.fkCrouch] & 128) KeyFlags += kfDown;


	/*
  if (KeyFlags & kfStrafe)
  {
    if (KeyboardState [KeyMap.fkLeft ] & 128)  KeyFlags+=kfSLeft;
    if (KeyboardState [KeyMap.fkRight] & 128) KeyFlags+=kfSRight;
  }
  else
  {
    if (KeyboardState [KeyMap.fkLeft ] & 128)  KeyFlags+=kfLeft;
    if (KeyboardState [KeyMap.fkRight] & 128) KeyFlags+=kfRight;
  }*/

  if (KeyboardState [KeyMap.fkSLeft]  & 128) KeyFlags+=kfSLeft;
  if (KeyboardState [KeyMap.fkSRight] & 128) KeyFlags+=kfSRight;


  if (KeyboardState [KeyMap.fkJump] & 128) KeyFlags+=kfJump;

  if (KeyboardState [KeyMap.fkCall] & 128)
    if (!(_KeyFlags & kfCall)) KeyFlags+=kfCall;

  }

  DeltaT = (float)TimeDt / 1000.f;

  if ( DemoPoint.DemoTime) ProcessDemoMovement();
  if (!DemoPoint.DemoTime) ProcessPlayerMovement();


//======= Y movement ===========//
  HeadAlpha = HeadBackR / 20000;
  HeadBeta =-HeadBackR / 10000;
  if (HeadBackR)
  {
    HeadBackR-=DeltaT*(80 + (32-(float)fabs(HeadBackR - 32))*4);
    if (HeadBackR<=0)
    {
      HeadBackR = 0;
      HeadBSpeed = 0;
    }
  }

  if ((CrouchMode) | (UNDERWATER) )
  {
    if (HeadY<110.f) HeadY = 110.f;
    HeadY-=DeltaT*(60 + (HeadY-110)*5);
    if (HeadY<110.f) HeadY = 110.f;
  }
  else
  {
    if (HeadY>220.f) HeadY = 220.f;
    HeadY+=DeltaT*(60 + (220 - HeadY) * 5);
    if (HeadY>220.f) HeadY = 220.f;
  }


  float h  = GetLandQH(PlayerX, PlayerZ);
  float hu = GetLandCeilH(PlayerX, PlayerZ)-64;
  float hwater = GetLandUpH(PlayerX, PlayerZ);

  if (DemoPoint.DemoTime) goto SKIPYMOVE;

  if (!UNDERWATER)
  {
    if (PlayerY>h) YSpeed-=DeltaT*3000;
  }
  else if (YSpeed<0)
  {
    YSpeed+=DeltaT*4000;
    if (YSpeed>0) YSpeed=0;
  }

  if (FLY) YSpeed=0;
  PlayerY+=YSpeed*DeltaT;


  if (PlayerY+HeadY>hu)
  {
    if (YSpeed>0) YSpeed=-1;
    PlayerY = hu - HeadY;
    if (PlayerY<h)
    {
      PlayerY = h;
      HeadY = hu - PlayerY;
      if (HeadY<110) HeadY = 110;
    }
  }

  if (PlayerY<h)
  {
    if (YSpeed<-800) HeadY+=YSpeed/100;
    if (PlayerY < h-80) PlayerY = h - 80;
    PlayerY+=(h-PlayerY+32)*DeltaT*4;
    if (PlayerY>h) PlayerY = h;
    if (YSpeed<-600)
      AddVoicev(fxStep[(RealTime % 3)].length,
                fxStep[(RealTime % 3)].lpData, 64);
    YSpeed = 0;
  }

SKIPYMOVE:

  SWIM = FALSE;
  if (!UNDERWATER && (KeyFlags & kfJump) )
    if (PlayerY<hwater-148)
    {
      SWIM = TRUE;
      PlayerY = hwater-148;
      YSpeed = 0;
    }

  float _s = stepdy;

  if (SWIM) stepdy = (float)sin((float)RealTime / 360) * 20;
  else stepdy = (float)MIN(1.f,fabs(VSpeed) + (float)fabs(SSpeed)) * (float)sin((float)RealTime / 80.f) * 22.f;
  float d = stepdy - _s;

  if (!UNDERWATER)
    if (PlayerY<h+64)
      if (d<0 && stepdd >= 0)
        if (ONWATER)
        {
          AddWCircle(CameraX, CameraZ, 1.2);
          AddVoicev(fxStepW[(RealTime % 3)].length,
                    fxStepW[(RealTime % 3)].lpData, 64+(int)(VSpeed*30.f));
        }
        else
          AddVoicev(fxStep[(RealTime % 3)].length,
                    fxStep[(RealTime % 3)].lpData, 24+(int)(VSpeed*50.f));
  stepdd = d;

  if (PlayerBeta> 1.46f) PlayerBeta= 1.46f;
  if (PlayerBeta<-1.26f) PlayerBeta=-1.26f;


//======== set camera pos ===================//

  if (Recoil.y < 0) {
	  Recoil.y += 0.005;
	  if (Recoil.y > 0) Recoil.y = 0;
  }
  if (Recoil.x != 0) DeltaFunc(Recoil.x, 0, 0.005);

  if (!DemoPoint.DemoTime)
  {

	PlayerAlpha += Recoil.x;
	PlayerBeta += Recoil.y;

    CameraAlpha = PlayerAlpha + HeadAlpha;
    CameraBeta  = PlayerBeta  + HeadBeta;

	CameraX = PlayerX - sa * HeadBackR;
    CameraY = PlayerY + HeadY + stepdy;// + 2024;
    CameraZ = PlayerZ + ca * HeadBackR;
  }

  if (CLIP3D)
  {
    if (sb<0) BackViewR = 320.f - 1024.f * sb;
    else BackViewR = 320.f + 512.f * sb;
    BackViewRR = 380 + (int)(1024 * fabs(sb));
    if (UNDERWATER) BackViewR -= 512.f * (float)MIN(0,sb);
  }
  else
  {
    BackViewR = 300;
    BackViewRR = 380;
  }


//==================== SWIM & UNDERWATER =========================//
  ONWATER = GetLandUpH(CameraX, CameraZ) > GetLandH(CameraX, CameraZ);

  if (UNDERWATER)
  {
    UNDERWATER = (GetLandUpH(CameraX, CameraZ)-4>= CameraY);
    if (!UNDERWATER)
    {
      HeadY+=20;
      CameraY+=20;
      AddVoicev(fxWaterOut.length, fxWaterOut.lpData, 256);
      AddWCircle(CameraX, CameraZ, 2.0);
    }
  }
  else
  {
    UNDERWATER = (GetLandUpH(CameraX, CameraZ)+28 >= CameraY);
    if (UNDERWATER)
    {
      HeadY-=20;
      CameraY-=20;
      BINMODE = FALSE;
      AddVoicev(fxWaterIn.length, fxWaterIn.lpData, 256);
      AddWCircle(CameraX, CameraZ, 2.0);
    }
  }

  if (MyHealth)
    if (UNDERWATER)
    {
      MyHealth-=TimeDt*12;
      //if ( !(Takt & 31)) AddElements(CameraX + sa*64*cb, CameraY - 32 - sb*64, CameraZ - ca*64*cb, 4);
      if (MyHealth<=0)
        AddDeadBody(NULL, HUNT_BREATH, TRUE);
    }

  if (UNDERWATER && !WeapInfo[CurrentWeapon].harpoon)
    if (Weapon.state) HideWeapon();

  if (!UNDERWATER) UnderWaterT = 0;
  else if (UnderWaterT<512) UnderWaterT += TimeDt;
  else UnderWaterT = 512;

  if (UNDERWATER)
  {
	  if (MyHealth) {
		CameraW = (float)VideoCX*(1.25f + (1.f + (float)cos(RealTime / 180.f)) / 30 + (1.f - (float)sin(UnderWaterT / 512.f*pi / 2)) / 1.5f);
		CameraH = (float)VideoCX*(1.25f + (1.f + (float)sin(RealTime / 180.f)) / 30 - (1.f - (float)sin(UnderWaterT / 512.f*pi / 2)) / 16.f);
		CameraH *= (WinH*1.3333f / WinW);

		CameraAlpha += (float)cos(RealTime / 360.f) / 120;
		CameraBeta += (float)sin(RealTime / 360.f) / 100;
		CameraY -= (float)sin(RealTime / 360.f) * 4;
	  }

	int w = WMap[(((int)(CameraZ))>>8) ][ (((int)(CameraX))>>8) ];
    FogsList[127].YBegin = (float)WaterList[w].wlevel;
    FogsList[127].fogRGB = WaterList[w].fogRGB;
  }
  else
  {
    CameraW = (float)VideoCX*1.25f;
    CameraH = CameraW * (WinH*1.3333f / WinW);
  }


  if (BINMODE)
  {
    CameraW*=BinocularPower;
    CameraH*=BinocularPower;
  }
  else if (OPTICMODE && (!WeapInfo[CurrentWeapon].unzoom || Weapon.state == 2))
  {
	  CameraW *= WeapInfo[CurrentWeapon].Optic;
	  CameraH *= WeapInfo[CurrentWeapon].Optic;
  }

  FOVK =  CameraW / (VideoCX*1.25f);

  InitClips();

  if (SWIM)
  {
    if (!(Takt & 31)) AddWCircle(CameraX, CameraZ, 1.5);
    CameraBeta -=(float)cos(RealTime/360.f) / 80;
    PlayerX+=DeltaT*32;
    PlayerZ+=DeltaT*32;
  }


  CameraFogI = FogsMap [((int)CameraZ)>>9][((int)CameraX)>>9];
  if (UNDERWATER) CameraFogI=127;
  if (FogsList[CameraFogI].YBegin*ctHScale> CameraY)
    CAMERAINFOG = (CameraFogI>0);
  else
    CAMERAINFOG = FALSE;

  if (CAMERAINFOG)
    if (MyHealth)
      if (FogsList[CameraFogI].Mortal)
      {
        if (MyHealth>100000) MyHealth = 100000;
        MyHealth-=TimeDt*64;
        if (MyHealth<=0)
          AddDeadBody(NULL, HUNT_EAT, TRUE);
      }

  int CameraAmb = AmbMap [((int)CameraZ)>>9][((int)CameraX)>>9];


  if (UNDERWATER)
  {
    SetAmbient(fxUnderwater.length,
               fxUnderwater.lpData,
               240);
    Audio_SetEnvironment(8, ctViewR*256);
  }
  else
  {
    SetAmbient(Ambient[CameraAmb].sfx.length,
               Ambient[CameraAmb].sfx.lpData,
               Ambient[CameraAmb].AVolume);
    Audio_SetEnvironment(Ambient[CameraAmb].rdata[0].REnvir, ctViewR*256);

    Env = Ambient[CameraAmb].rdata[0].REnvir;

    if (Ambient[CameraAmb].RSFXCount)
    {
      Ambient[CameraAmb].RndTime-=TimeDt;
      if (Ambient[CameraAmb].RndTime<=0)
      {
        Ambient[CameraAmb].RndTime = (Ambient[CameraAmb].rdata[0].RFreq / 2 + rRand(Ambient[CameraAmb].rdata[0].RFreq)) * 1000;
        int rr = (rand() % Ambient[CameraAmb].RSFXCount);
        int r = Ambient[CameraAmb].rdata[rr].RNumber;
        AddVoice3dv(RandSound[r].length, RandSound[r].lpData,
                    CameraX + siRand(4096),
                    CameraY + siRand(256),
                    CameraZ + siRand(4096),
                    Ambient[CameraAmb].rdata[rr].RVolume);
      }
    }
  }


  if (NOCLIP) CameraY+=1024;
  //======= results ==========//
  if (CameraBeta> 1.46f) CameraBeta= 1.46f;
  if (CameraBeta<-1.26f) CameraBeta=-1.26f;

  PlayerPos.x = PlayerX;
  PlayerPos.y = PlayerY;
  PlayerPos.z = PlayerZ;

  CameraPos.x = CameraX;
  CameraPos.y = CameraY;
  CameraPos.z = CameraZ;

}















void ProcessGame()
{
  if (RestartMode)
  {
    ShutDown3DHardware();
    AudioStop();
    NeedRVM = TRUE;

  }

  if (!_GameState)
  {

	  /*
	  for (int di = 0; di < DINOINFO_MAX; di++) {
		  DinoInfo[di].trophyLocTotal1 = 0;
		  DinoInfo[di].trophyLocTotal2 = 0;
	  }

	  char Buff[100];
	  for (int c = 0; c < TROPHY2_COUNT; c++) {
		  sprintf(Buff, "SETTING LOC TOTAL INC %i", c);
		  PrintLog(Buff);
		  sprintf(Buff, " = %i\n", TrophyIndex[c]);
		  PrintLog(Buff);
		  DinoInfo[TrophyIndex[c]].trophyLocTotal1++;
		  DinoInfo[TrophyIndex[c]].trophyLocTotal2++;
	  }
	  */

    PrintLog("Entered game\n");
    ReInitGame();
    while (ShowCursor(FALSE)>=0);

	if (Multiplayer) {
		if (!_MultiplayerState) {

			if (Host) {
				StartupServerCommsThread();
			} else {
				StartupClientCommsThread();
			}

		}
		_MultiplayerState = 1;
	}


//test
	/*
	long long_data = PlayerX;

	char printable2[25];
	_itoa(long_data, printable2, 10);
	PrintLog("SENDINGVALUE:");
	PrintLog(printable2);
	PrintLog("\n");

	byte tdata[4];
	tdata[0] = (int)((long_data >> 24) & 0xFF);
	tdata[1] = (int)((long_data >> 16) & 0xFF);
	tdata[2] = (int)((long_data >> 8) & 0XFF);
	tdata[3] = (int)((long_data & 0XFF));

	const char *p = reinterpret_cast<const char*>(tdata);
	const byte *tdata2 = reinterpret_cast<const byte*>(p);

	long anotherLongInt = ((tdata2[0] << 24)
		+ (tdata2[1] << 16)
		+ (tdata2[2] << 8)
		+ (tdata2[3]));

	char printable[25];
	_itoa(anotherLongInt, printable, 10);

	PrintLog("SENTVALUE:");
	PrintLog(printable);
	PrintLog("\n");
	*/



  }

  _GameState = 1;

  if (NeedRVM)
  {
	SetWindowPos(hwndMain, HWND_TOP, 0,0,0,0,  SWP_SHOWWINDOW);
	SetFocus(hwndMain);
	Activate3DHardware();
	NeedRVM = FALSE;
  }

  ProcessSyncro();

  if (!PAUSE || !MyHealth)
  {
    ProcessControls();
    AudioSetCameraPos(CameraX, CameraY, CameraZ, CameraAlpha, CameraBeta);
    Audio_UploadGeometry();
    AnimateCharacters();
	AnimateBullets();
	if (Multiplayer) {
		AnimateMHunters();
	}
    AnimateProcesses();
  }

  if (DEBUG || ObservMode || TrophyMode)
    if (MyHealth) MyHealth = MAX_HEALTH;
  if (DEBUG) ShotsLeft[CurrentWeapon] = WeapInfo[CurrentWeapon].Shots;

  DrawScene();

  if (!TrophyMode)
    if (MapMode) DrawHMap();

  DrawPostObjects();

  ShowControlElements();

  ShowVideo();
}



int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpszCmdLine, int nCmdShow)
{
	
	MSG msg;

  hInst = hInstance;
  CreateLog();

  CreateMainWindow();

  Init3DHardware();
  InitEngine();
  InitAudioSystem(hwndMain, hlog, OptSound);

  StartLoading();
  PrintLoad("Loading...");

  PrintLog("== Loading resources ==\n");
  hcArrow = LoadCursor(NULL, IDC_ARROW);


  PrintLog("Loading common resources:");
  PrintLoad("Loading common resources...");

  if (OptDayNight==2)
    LoadModelEx(SunModel,    "HUNTDAT\\MOON.3DF");
  else
    LoadModelEx(SunModel,    "HUNTDAT\\SUN2.3DF");
  LoadModelEx(CompasModel, "HUNTDAT\\COMPAS.3DF");
  LoadModelEx(Binocular,   "HUNTDAT\\BINOCUL.3DF");

  LoadCharacterInfo(WCircleModel, "HUNTDAT\\WCIRCLE2.CAR");
  LoadCharacterInfo(ShipModel, "HUNTDAT\\ship2a.car");
  LoadCharacterInfo(SShipModel, "HUNTDAT\\sship.car");
  LoadCharacterInfo(BagModel, "HUNTDAT\\bag1.car");
  LoadCharacterInfo(WindModel, "HUNTDAT\\WIND.CAR");

  LoadCharacterInfo(MuzzModel, "HUNTDAT\\MUZZ4.CAR");

  LoadWav("HUNTDAT\\SOUNDFX\\a_underw.wav",  fxUnderwater);

  LoadWav("HUNTDAT\\SOUNDFX\\blip.wav", fxBlip);

  LoadWav("HUNTDAT\\SOUNDFX\\click1.wav", fxClick[0]);
  LoadWav("HUNTDAT\\SOUNDFX\\click2.wav", fxClick[1]);
  LoadWav("HUNTDAT\\SOUNDFX\\click3.wav", fxClick[2]);

  LoadWav("HUNTDAT\\SOUNDFX\\breath1.wav", fxBreathIn);
  LoadWav("HUNTDAT\\SOUNDFX\\breath2.wav", fxBreathOut);

  LoadWav("HUNTDAT\\SOUNDFX\\collect1.wav", fxCollect[0]);
  LoadWav("HUNTDAT\\SOUNDFX\\collect2.wav", fxCollect[1]);
  LoadWav("HUNTDAT\\SOUNDFX\\collect3.wav", fxCollect[2]);

  LoadWav("HUNTDAT\\SOUNDFX\\IMPACT\\ground1.wav", fxImpactGround[0]);
  LoadWav("HUNTDAT\\SOUNDFX\\IMPACT\\ground2.wav", fxImpactGround[1]);
  LoadWav("HUNTDAT\\SOUNDFX\\IMPACT\\ground3.wav", fxImpactGround[2]);
  LoadWav("HUNTDAT\\SOUNDFX\\IMPACT\\model1.wav", fxImpactModel[0]);
  LoadWav("HUNTDAT\\SOUNDFX\\IMPACT\\model2.wav", fxImpactModel[1]);
  LoadWav("HUNTDAT\\SOUNDFX\\IMPACT\\model3.wav", fxImpactModel[2]);
  LoadWav("HUNTDAT\\SOUNDFX\\IMPACT\\water1.wav", fxImpactWater[0]);
  LoadWav("HUNTDAT\\SOUNDFX\\IMPACT\\water2.wav", fxImpactWater[1]);
  LoadWav("HUNTDAT\\SOUNDFX\\IMPACT\\water3.wav", fxImpactWater[2]);
  LoadWav("HUNTDAT\\SOUNDFX\\IMPACT\\char1.wav", fxImpactChar[0]);
  LoadWav("HUNTDAT\\SOUNDFX\\IMPACT\\char2.wav", fxImpactChar[1]);
  LoadWav("HUNTDAT\\SOUNDFX\\IMPACT\\char3.wav", fxImpactChar[2]);

  LoadWav("HUNTDAT\\SOUNDFX\\STEPS\\hwalk1.wav",  fxStep[0]);
  LoadWav("HUNTDAT\\SOUNDFX\\STEPS\\hwalk2.wav",  fxStep[1]);
  LoadWav("HUNTDAT\\SOUNDFX\\STEPS\\hwalk3.wav",  fxStep[2]);

  LoadWav("HUNTDAT\\SOUNDFX\\STEPS\\footw1.wav",  fxStepW[0]);
  LoadWav("HUNTDAT\\SOUNDFX\\STEPS\\footw2.wav",  fxStepW[1]);
  LoadWav("HUNTDAT\\SOUNDFX\\STEPS\\footw3.wav",  fxStepW[2]);

  LoadWav("HUNTDAT\\SOUNDFX\\hum_die1.wav",  fxScream[0]);
  LoadWav("HUNTDAT\\SOUNDFX\\hum_die2.wav",  fxScream[1]);
  LoadWav("HUNTDAT\\SOUNDFX\\hum_die3.wav",  fxScream[2]);
  LoadWav("HUNTDAT\\SOUNDFX\\hum_die4.wav",  fxScream[3]);

  LoadPictureTGA(PausePic,   "HUNTDAT\\MENU\\pause.tga");
  conv_pic(PausePic);
  if (SurvivalMode) LoadPictureTGA(ExitPic, "HUNTDAT\\MENU\\exit_s.tga");
  else LoadPictureTGA(ExitPic,    "HUNTDAT\\MENU\\exit.tga");
  conv_pic(ExitPic);
  LoadPictureTGA(TrophyExit, "HUNTDAT\\MENU\\trophy_e.tga");
  conv_pic(TrophyExit);
  LoadPictureTGA(MapPic,     "HUNTDAT\\MENU\\mapframe.tga");
  conv_pic(MapPic);

  LoadPictureTGA(TFX_ENVMAP,    "HUNTDAT\\FX\\envmap.tga");
  ApplyAlphaFlags(TFX_ENVMAP.lpImage, TFX_ENVMAP.W*TFX_ENVMAP.W);
  LoadPictureTGA(TFX_SPECULAR,  "HUNTDAT\\FX\\specular.tga");
  ApplyAlphaFlags(TFX_SPECULAR.lpImage, TFX_SPECULAR.W*TFX_SPECULAR.W);


  PrintLog(" Done.\n");

  PrintLoad("Loading area...");
  LoadResources();

  PrintLoad("Starting game...");
  PrintLog("Loading area: Done.\n");

  EndLoading();

  ProcessSyncro();
  blActive = TRUE;

  alreadyFired = false;

  PrintLog("Entering messages loop.\n");
  for( ; ; ){
    if( PeekMessage( &msg, NULL, NULL, NULL, PM_REMOVE ) )
    {
      if (msg.message == WM_QUIT)  break;
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    }
    else
    {
      if (blActive) ProcessGame();
      else Sleep(100);
    }
  }

  if (Multiplayer) {
	  if (Host) {
		  ShutDownServer();
	  }
	  else {
		  ShutDownClient();
	  }
  }

  AudioStop();
  Audio_Shutdown();

  ShutDown3DHardware();

  ShutDownEngine();

  ShowCursor(TRUE);
  PrintLog("Game normal shutdown.\n");

  CloseLog();
  return msg.wParam;
}
