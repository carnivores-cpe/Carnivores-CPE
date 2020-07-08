#include "Hunt.h"
#include "stdio.h"

BOOL NewPhase;

#define fx_DIE    0

/*
#define RAP_RUN    0
#define RAP_WALK   1
#define RAP_SWIM   2
#define RAP_SLIDE  3
#define RAP_JUMP   4
#define RAP_DIE    5
#define RAP_EAT    6
#define RAP_SLP    7
#define RAP_IDLE1  8
#define RAP_IDLE2  9

#define SPN_RUN    0
#define SPN_WALK   1
#define SPN_SLIDE  2
#define SPN_SWIM   1
#define SPN_IDLE1  3
#define SPN_IDLE2  4
#define SPN_JUMP   5
#define SPN_DIE    6
#define SPN_EAT    7
#define SPN_SLP    8

#define VEL_RUN    0
#define VEL_WALK   1
#define VEL_SWIM   3
#define VEL_SLIDE  2
#define VEL_JUMP   4
#define VEL_DIE    5
#define VEL_EAT    6
#define VEL_SLP    7
#define VEL_IDLE1  8
#define VEL_IDLE2  9

#define CER_WALK   0
#define CER_RUN    1
#define CER_IDLE1  2
#define CER_IDLE2  3
#define CER_IDLE3  4
#define CER_DIE    5
#define CER_SLP    6
#define CER_EAT    7
#define CER_SWIM   0

*/

#define MOSA_RUN    0
#define MOSA_WALK   1
#define MOSA_JUMP   2
#define MOSA_DIE    3
#define MOSA_EAT    4
#define MOSA_SLP    5

/*
#define FISH_WALK   0
#define FISH_RUN    1

#define REX_RUN    0
#define REX_WALK   1
#define REX_SCREAM 2
#define REX_SWIM   3
#define REX_SEE    4
#define REX_SEE1   5
#define REX_SMEL   6
#define REX_SMEL1  7
#define REX_DIE    8
#define REX_EAT    9
#define REX_SLP    10


#define ICTH_WALK           0
#define ICTH_WALK_IDLE1     1
#define ICTH_WALK_IDLE2     2
#define ICTH_WINGDOWN_LAND  3
#define ICTH_SWIM           4
#define ICTH_SWIM_IDLE1     5
#define ICTH_SWIM_IDLE2     6
#define ICTH_WINGDOWN_WATER 7
#define ICTH_FLY            8
#define ICTH_FLY2           9
#define ICTH_TAKEOFF        10
#define ICTH_LANDING        11
#define ICTH_FALL           12
#define ICTH_LAND_DIE       13
#define ICTH_WATER_DIE      14
#define ICTH_SLEEP          15


#define MOS_RUN    0
#define MOS_WALK   1
#define MOS_DIE    2
#define MOS_IDLE1  3
#define MOS_IDLE2  4
#define MOS_SLP    5


#define DMT_WALK   0
#define DMT_RUN    1
#define DMT_IDLE1  2
#define DMT_IDLE2  3
#define DMT_DIE    4
#define DMT_SLP    5


#define ANK_RUN    0
#define ANK_WALK   1
#define ANK_IDLE1  2
#define ANK_IDLE2  3
#define ANK_DIE    4
#define ANK_SLP    5


#define STG_RUN    0
#define STG_WALK   1
#define STG_DIE    2
#define STG_IDLE1  3
#define STG_IDLE2  4
#define STG_SLP    5

#define GAL_RUN    0
#define GAL_WALK   1
#define GAL_SLIDE  2
#define GAL_DIE    3
#define GAL_IDLE1  4
#define GAL_IDLE2  5
#define GAL_SLP    6


#define TRI_RUN    0
#define TRI_WALK   1
#define TRI_IDLE1  2
#define TRI_IDLE2  3
#define TRI_IDLE3  4
#define TRI_DIE    5
#define TRI_SLP    6


#define PAC_WALK   0
#define PAC_RUN    1
#define PAC_SLIDE  2
#define PAC_DIE    3
#define PAC_IDLE1  4
#define PAC_IDLE2  5
#define PAC_SLP    6

#define PAR_WALK   0
#define PAR_RUN    1
#define PAR_IDLE1  2
#define PAR_IDLE2  3
#define PAR_DIE    4
#define PAR_SLP    5

#define DIM_FLY    0
#define DIM_FLYP   1
#define DIM_FALL   2
#define DIM_DIE    3

#define BRA_WALK   0
#define BRA_IDLE1  1
#define BRA_IDLE2  2
#define BRA_IDLE3  3
#define BRA_DIE    4
#define BRA_SLP    5
#define BRA_RUN    6
#define BRA_EAT    10
*/

int CurDino;


void SetNewTargetPlace(TCharacter *cptr, float R);
void SetNewTargetPlaceRegion(TCharacter *cptr, float R);
void SetNewTargetPlaceVanilla(TCharacter *cptr, float R);


void ProcessPrevPhase(TCharacter *cptr)
{
	cptr->PPMorphTime += TimeDt;
	if (cptr->PPMorphTime > PMORPHTIME) cptr->PrevPhase = cptr->Phase;

	cptr->PrevPFTime += TimeDt;
	cptr->PrevPFTime %= cptr->pinfo->Animation[cptr->PrevPhase].AniTime;
}


void ActivateCharacterFxAquatic(TCharacter *cptr)
{
	if (cptr->AI) //== not hunter ==//
		if (!UNDERWATER) return;
	int fx = cptr->pinfo->Anifx[cptr->Phase];
	if (fx == -1) return;

	if (VectorLength(SubVectors(PlayerPos, cptr->pos)) > 68 * 256) return;

	AddVoice3d(cptr->pinfo->SoundFX[fx].length,
		cptr->pinfo->SoundFX[fx].lpData,
		cptr->pos.x, cptr->pos.y, cptr->pos.z);
}


void ActivateCharacterFx(TCharacter *cptr)
{
	if (cptr->AI) //== not hunter ==//
		if (UNDERWATER) return;
	int fx = cptr->pinfo->Anifx[cptr->Phase];
	if (fx == -1) return;

	if (VectorLength(SubVectors(PlayerPos, cptr->pos)) > 68 * 256) return;

	AddVoice3d(cptr->pinfo->SoundFX[fx].length,
		cptr->pinfo->SoundFX[fx].lpData,
		cptr->pos.x, cptr->pos.y, cptr->pos.z);
}


void ResetCharacter(TCharacter *cptr)
{
	cptr->AI = DinoInfo[cptr->CType].AI;
	cptr->pinfo = &ChInfo[cptr->CType];
	cptr->Clone = DinoInfo[cptr->CType].Clone;
	cptr->State = 0;
	cptr->StateF = 0;
	cptr->Phase = 0;
	cptr->FTime = 0;
	cptr->PrevPhase = 0;
	cptr->PrevPFTime = 0;
	cptr->PPMorphTime = 0;
	cptr->beta = 0;
	cptr->gamma = 0;
	cptr->tggamma = 0;
	cptr->bend = 0;
	cptr->rspeed = 0;
	cptr->AfraidTime = 0;
	cptr->BloodTTime = 0;
	cptr->BloodTime = 0;

	if (DinoInfo[cptr->CType].killTypeCount > 1) {
		cptr->killType = rRand(DinoInfo[cptr->CType].killTypeCount - 1);
	}

	if (DinoInfo[cptr->CType].roarCount > 0) {
		cptr->roarAnim = DinoInfo[cptr->CType].roarAnim[rRand(DinoInfo[cptr->CType].roarCount - 1)];
	}

	cptr->speed_run = DinoInfo[cptr->CType].runspd;
	cptr->speed_walk = DinoInfo[cptr->CType].wlkspd;
	cptr->speed_jump = DinoInfo[cptr->CType].jmpspd;
	cptr->speed_swim = DinoInfo[cptr->CType].swmspd;
	cptr->speed_fly = DinoInfo[cptr->CType].flyspd;
	cptr->speed_glide = DinoInfo[cptr->CType].gldspd;
	cptr->speed_takeoff = DinoInfo[cptr->CType].tkfspd;
	cptr->speed_land = DinoInfo[cptr->CType].lndspd;

	cptr->lastTBeta = 0;
	cptr->turny = 0;
	cptr->bdepth = (float)0;

	cptr->lookx = (float)cos(cptr->alpha);
	cptr->lookz = (float)sin(cptr->alpha);

	cptr->Health = DinoInfo[cptr->CType].Health0;
	if (OptAgres > 128) cptr->Health = (cptr->Health*OptAgres) / 128;

	cptr->scale = (float)(DinoInfo[cptr->CType].Scale0 + rRand(DinoInfo[cptr->CType].ScaleA)) / 1000.f;

	cptr->RType = DinoInfo[cptr->CType].RType0[0];

	cptr->followLeader = false;
}


void AddDeadBody(TCharacter *cptr, int phase)
{
	if (!MyHealth) return;

	if (ExitTime)
		AddMessage("Transportation cancelled.");
	ExitTime = 0;

	OPTICMODE = FALSE;
	BINMODE = FALSE;
	Characters[ChCount].CType = 0;
	Characters[ChCount].alpha = CameraAlpha;
	ResetCharacter(&Characters[ChCount]);

	int v = rRand(3);
	if (phase != HUNT_BREATH)
		AddVoicev(fxScream[r].length, fxScream[r].lpData, 256);

	Characters[ChCount].Health = 0;
	MyHealth = 0;
	if (cptr)
	{
		killerDino = cptr;

		if (GetLandUpH(killerDino->pos.x, killerDino->pos.z) -
			GetLandH(killerDino->pos.x, killerDino->pos.z) >
			DinoInfo[killerDino->CType].waterLevel * killerDino->scale) {
			killedwater = TRUE;
		}
		else {
			killedwater = FALSE;
		}

		float pl = DinoInfo[cptr->CType].killType[cptr->killType].offset;
		Characters[ChCount].pos.x = cptr->pos.x + cptr->lookx * pl * cptr->scale;
		Characters[ChCount].pos.z = cptr->pos.z + cptr->lookz * pl * cptr->scale;
		Characters[ChCount].pos.y = GetLandQH(Characters[ChCount].pos.x, Characters[ChCount].pos.z);
	}
	else
	{
		Characters[ChCount].pos.x = PlayerX;
		Characters[ChCount].pos.z = PlayerZ;
		Characters[ChCount].pos.y = PlayerY;
	}

	Characters[ChCount].Phase = phase;
	Characters[ChCount].PrevPhase = phase;

	ActivateCharacterFx(&Characters[ChCount]);


	DemoPoint.pos = Characters[ChCount].pos;
	DemoPoint.DemoTime = 1;
	DemoPoint.CIndex = ChCount;

	/*
	//if (phase > 0) {
	if (DinoInfo[cptr->CType].killType[cptr->killType].elevate) {
		Characters[ChCount].scale = cptr->scale;
		Characters[ChCount].alpha = cptr->alpha;
		cptr->bend = 0;
		DemoPoint.CIndex = CurDino;
	}
	*/

	ChCount++;
}



float AngleDifference(float a, float b)
{
	a -= b;
	a = (float)fabs(a);
	if (a > pi) a = 2 * pi - a;
	return a;
}

float CorrectedAlpha(float a, float b)
{
	float d = (float)fabs(a - b);
	if (d < pi) return (a + b) / 2;
	else d = (a + pi * 2 - b);

	if (d < 0) d += 2 * pi;
	if (d > 2 * pi) d -= 2 * pi;
	return d;
}

void ThinkY_Beta_Gamma(TCharacter *cptr, float blook, float glook, float blim, float glim)
{
	cptr->pos.y = GetLandH(cptr->pos.x, cptr->pos.z);

	//=== beta ===//
	float hlook = GetLandH(cptr->pos.x + cptr->lookx * blook, cptr->pos.z + cptr->lookz * blook);
	float hlook2 = GetLandH(cptr->pos.x - cptr->lookx * blook, cptr->pos.z - cptr->lookz * blook);
	DeltaFunc(cptr->beta, (hlook2 - hlook) / (blook * 3.2f), TimeDt / 800.f);

	if (cptr->beta > blim) cptr->beta = blim;
	if (cptr->beta < -blim) cptr->beta = -blim;

	//=== gamma ===//
	hlook = GetLandH(cptr->pos.x + cptr->lookz * glook, cptr->pos.z - cptr->lookx*glook);
	hlook2 = GetLandH(cptr->pos.x - cptr->lookz * glook, cptr->pos.z + cptr->lookx*glook);
	cptr->tggamma = (hlook - hlook2) / (glook * 3.2f);
	if (cptr->tggamma > glim) cptr->tggamma = glim;
	if (cptr->tggamma < -glim) cptr->tggamma = -glim;
	/*
	  if (DEBUG) cptr->tggamma = 0;
	  if (DEBUG) cptr->beta    = 0;
	  */
}




int CheckPlaceCollisionP(Vector3d &v)
{
	int ccx = (int)v.x / 256;
	int ccz = (int)v.z / 256;

	if (ccx < 4 || ccz < 4 || ccx>1008 || ccz>1008) return 1;

	int F = (FMap[ccz][ccx - 1] | FMap[ccz - 1][ccx] | FMap[ccz - 1][ccx - 1] |
		FMap[ccz][ccx] |
		FMap[ccz + 1][ccx] | FMap[ccz][ccx + 1] | FMap[ccz + 1][ccx + 1]);

	if (F & (fmWater + fmNOWAY)) return 1;


	float h = GetLandH(v.x, v.z);
	v.y = h;

	float hh = GetLandH(v.x - 164, v.z - 164);
	if (fabs(hh - h) > 160) return 1;
	hh = GetLandH(v.x + 164, v.z - 164);
	if (fabs(hh - h) > 160) return 1;
	hh = GetLandH(v.x - 164, v.z + 164);
	if (fabs(hh - h) > 160) return 1;
	hh = GetLandH(v.x + 164, v.z + 164);
	if (fabs(hh - h) > 160) return 1;

	for (int z = -2; z <= 2; z++)
		for (int x = -2; x <= 2; x++)
			if (OMap[ccz + z][ccx + x] != 255)
			{
				int ob = OMap[ccz + z][ccx + x];
				if (MObjects[ob].info.Radius < 10) continue;
				float CR = (float)MObjects[ob].info.Radius + 64;

				float oz = (ccz + z) * 256.f + 128.f;
				float ox = (ccx + x) * 256.f + 128.f;

				float r = (float)sqrt((ox - v.x)*(ox - v.x) + (oz - v.z)*(oz - v.z));
				if (r < CR) return 1;
			}

	return 0;
}


//checks place collision without checking for water I think?
int AquaticCheckPlaceCollisionP(Vector3d &v)
{
	int ccx = (int)v.x / 256;
	int ccz = (int)v.z / 256;

	//keep this
	if (ccx < 4 || ccz < 4 || ccx>1008 || ccz>1008) return 1;

	//what does this mean??
	int F = (FMap[ccz][ccx - 1] | FMap[ccz - 1][ccx] | FMap[ccz - 1][ccx - 1] |
		FMap[ccz][ccx] |
		FMap[ccz + 1][ccx] | FMap[ccz][ccx + 1] | FMap[ccz + 1][ccx + 1]);

	//what does this mean??
	if (F & fmNOWAY) return 1;

	//keep this
	float h = GetLandH(v.x, v.z);
	v.y = h;

	//keep this
	float hh = GetLandH(v.x - 164, v.z - 164);
	if (fabs(hh - h) > 160) return 1;
	hh = GetLandH(v.x + 164, v.z - 164);
	if (fabs(hh - h) > 160) return 1;
	hh = GetLandH(v.x - 164, v.z + 164);
	if (fabs(hh - h) > 160) return 1;
	hh = GetLandH(v.x + 164, v.z + 164);
	if (fabs(hh - h) > 160) return 1;

	//keep this
	for (int z = -2; z <= 2; z++)
		for (int x = -2; x <= 2; x++)
			if (OMap[ccz + z][ccx + x] != 255)
			{
				int ob = OMap[ccz + z][ccx + x];
				if (MObjects[ob].info.Radius < 10) continue;
				float CR = (float)MObjects[ob].info.Radius + 64;

				float oz = (ccz + z) * 256.f + 128.f;
				float ox = (ccx + x) * 256.f + 128.f;

				float r = (float)sqrt((ox - v.x)*(ox - v.x) + (oz - v.z)*(oz - v.z));
				if (r < CR) return 1;
			}

	return 0;
}



int CheckPlaceCollisionFish(TCharacter *cptr, Vector3d &v, float mosaDepth, int maxDepth, int minDepth)
{

	int ccx = (int)v.x / 256;
	int ccz = (int)v.z / 256;

	if (ccx < 4 || ccz < 4 || ccx>1018 || ccz>1018) return 1;

	/*if (wc)
		if ((FMap[ccz][ccx - 1] | FMap[ccz - 1][ccx] | FMap[ccz - 1][ccx - 1] |
			FMap[ccz][ccx] |
			FMap[ccz + 1][ccx] | FMap[ccz][ccx + 1] | FMap[ccz + 1][ccx + 1]) & fmWater)
			return 1;
	 */

	if (DinoInfo[cptr->CType].AvoidCount)
	{
		for (int i = 0; i < DinoInfo[cptr->CType].AvoidCount; i++) {
			if (ccx < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMax) &&
				ccz < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMax) &&
				ccx >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMin) &&
				ccz >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMin)) return 1;
		}
	}

	// #C1 KEEP THIS
	if ((GetLandUpH(v.x, v.z) - GetLandH(v.x, v.z)) < minDepth ||
		(GetLandUpH(v.x + 256, v.z) - GetLandH(v.x + 256, v.z)) < minDepth ||
		(GetLandUpH(v.x, v.z + 256) - GetLandH(v.x, v.z + 256)) < minDepth ||
		(GetLandUpH(v.x + 256, v.z + 256) - GetLandH(v.x + 256, v.z + 256)) < minDepth ||
		(GetLandUpH(v.x - 256, v.z) - GetLandH(v.x - 256, v.z)) < minDepth ||
		(GetLandUpH(v.x, v.z - 256) - GetLandH(v.x, v.z - 256)) < minDepth ||
		(GetLandUpH(v.x - 256, v.z - 256) - GetLandH(v.x - 256, v.z - 256)) < minDepth ||
		(GetLandUpH(v.x + 256, v.z - 256) - GetLandH(v.x + 256, v.z - 256)) < minDepth ||
		(GetLandUpH(v.x - 256, v.z + 256) - GetLandH(v.x - 256, v.z + 256)) < minDepth) return 1;

	if ((GetLandUpH(v.x, v.z) - GetLandH(v.x, v.z)) > maxDepth ||
		(GetLandUpH(v.x + 256, v.z) - GetLandH(v.x + 256, v.z)) > maxDepth ||
		(GetLandUpH(v.x, v.z + 256) - GetLandH(v.x, v.z + 256)) > maxDepth ||
		(GetLandUpH(v.x + 256, v.z + 256) - GetLandH(v.x + 256, v.z + 256)) > maxDepth ||
		(GetLandUpH(v.x - 256, v.z) - GetLandH(v.x - 256, v.z)) > maxDepth ||
		(GetLandUpH(v.x, v.z - 256) - GetLandH(v.x, v.z - 256)) > maxDepth ||
		(GetLandUpH(v.x - 256, v.z - 256) - GetLandH(v.x - 256, v.z - 256)) > maxDepth ||
		(GetLandUpH(v.x + 256, v.z - 256) - GetLandH(v.x + 256, v.z - 256)) > maxDepth ||
		(GetLandUpH(v.x - 256, v.z + 256) - GetLandH(v.x - 256, v.z + 256)) > maxDepth) return 1;

	// #C1 REMOVE THESE
	//if (mosaDepth > GetLandUpH(v.x, v.z) - 700) return 1;
	//if (mosaDepth < GetLandH(v.x, v.z) + 500) return 1;

	/*
	float h = GetLandH(v.x, v.z);
	if (fabs(h - v.y) > 64) return 1;

	v.y = h;

	float hh = GetLandH(v.x - 64, v.z - 64);
	if (fabs(hh - h) > 100) return 1;
	hh = GetLandH(v.x + 64, v.z - 64);
	if (fabs(hh - h) > 100) return 1;
	hh = GetLandH(v.x - 64, v.z + 64);
	if (fabs(hh - h) > 100) return 1;
	hh = GetLandH(v.x + 64, v.z + 64);
	if (fabs(hh - h) > 100) return 1;

	if (mc)
		for (int z = -2; z <= 2; z++)
			for (int x = -2; x <= 2; x++)
				if (OMap[ccz + z][ccx + x] != 255)
				{
					int ob = OMap[ccz + z][ccx + x];
					if (MObjects[ob].info.Radius < 10) continue;
					float CR = (float)MObjects[ob].info.Radius + 64;

					float oz = (ccz + z) * 256.f + 128.f;
					float ox = (ccx + x) * 256.f + 128.f;

					float r = (float)sqrt((ox - v.x)*(ox - v.x) + (oz - v.z)*(oz - v.z));
					if (r < CR) return 1;
				}

				*/
	return 0;
}



int CheckPlaceCollisionMosasaurus(TCharacter *cptr, Vector3d &v, float mosaDepth)
{

	int ccx = (int)v.x / 256;
	int ccz = (int)v.z / 256;

	if (ccx < 4 || ccz < 4 || ccx>1018 || ccz>1018) return 1;

	/*if (wc)
		if ((FMap[ccz][ccx - 1] | FMap[ccz - 1][ccx] | FMap[ccz - 1][ccx - 1] |
			FMap[ccz][ccx] |
			FMap[ccz + 1][ccx] | FMap[ccz][ccx + 1] | FMap[ccz + 1][ccx + 1]) & fmWater)
			return 1;
	 */

	if (DinoInfo[cptr->CType].AvoidCount)
	{
		for (int i = 0; i < DinoInfo[cptr->CType].AvoidCount; i++) {
			if (ccx < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMax) &&
				ccz < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMax) &&
				ccx >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMin) &&
				ccz >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMin)) return 1;
		}
	}

	// #C1 KEEP THIS
	if ((GetLandUpH(v.x, v.z) - GetLandH(v.x, v.z)) < 1500 ||
		(GetLandUpH(v.x + 256, v.z) - GetLandH(v.x + 256, v.z)) < 1500 ||
		(GetLandUpH(v.x, v.z + 256) - GetLandH(v.x, v.z + 256)) < 1500 ||
		(GetLandUpH(v.x + 256, v.z + 256) - GetLandH(v.x + 256, v.z + 256)) < 1500 ||
		(GetLandUpH(v.x - 256, v.z) - GetLandH(v.x - 256, v.z)) < 1500 ||
		(GetLandUpH(v.x, v.z - 256) - GetLandH(v.x, v.z - 256)) < 1500 ||
		(GetLandUpH(v.x - 256, v.z - 256) - GetLandH(v.x - 256, v.z - 256)) < 1500 ||
		(GetLandUpH(v.x + 256, v.z - 256) - GetLandH(v.x + 256, v.z - 256)) < 1500 ||
		(GetLandUpH(v.x - 256, v.z + 256) - GetLandH(v.x - 256, v.z + 256)) < 1500) return 1;

	// #C1 REMOVE THESE
	//if (mosaDepth > GetLandUpH(v.x, v.z) - 700) return 1;
	//if (mosaDepth < GetLandH(v.x, v.z) + 500) return 1;

	/*
	float h = GetLandH(v.x, v.z);
	if (fabs(h - v.y) > 64) return 1;

	v.y = h;

	float hh = GetLandH(v.x - 64, v.z - 64);
	if (fabs(hh - h) > 100) return 1;
	hh = GetLandH(v.x + 64, v.z - 64);
	if (fabs(hh - h) > 100) return 1;
	hh = GetLandH(v.x - 64, v.z + 64);
	if (fabs(hh - h) > 100) return 1;
	hh = GetLandH(v.x + 64, v.z + 64);
	if (fabs(hh - h) > 100) return 1;

	if (mc)
		for (int z = -2; z <= 2; z++)
			for (int x = -2; x <= 2; x++)
				if (OMap[ccz + z][ccx + x] != 255)
				{
					int ob = OMap[ccz + z][ccx + x];
					if (MObjects[ob].info.Radius < 10) continue;
					float CR = (float)MObjects[ob].info.Radius + 64;

					float oz = (ccz + z) * 256.f + 128.f;
					float ox = (ccx + x) * 256.f + 128.f;

					float r = (float)sqrt((ox - v.x)*(ox - v.x) + (oz - v.z)*(oz - v.z));
					if (r < CR) return 1;
				}

				*/
	return 0;
}




int CheckPlaceCollision(TCharacter *cptr, Vector3d &v, BOOL wc, BOOL mc)
{
	int ccx = (int)v.x / 256;
	int ccz = (int)v.z / 256;

	if (ccx < 4 || ccz < 4 || ccx>1018 || ccz>1018) return 1;

	if (wc)
		if ((FMap[ccz][ccx - 1] | FMap[ccz - 1][ccx] | FMap[ccz - 1][ccx - 1] |
			FMap[ccz][ccx] |
			FMap[ccz + 1][ccx] | FMap[ccz][ccx + 1] | FMap[ccz + 1][ccx + 1]) & fmWater)
			return 1;


	float h = GetLandH(v.x, v.z);
	if (!(FMap[ccz][ccx] & fmWater))
		if (fabs(h - v.y) > 64) return 1;

	if (DinoInfo[cptr->CType].AvoidCount)
	{
		for (int i = 0; i < DinoInfo[cptr->CType].AvoidCount; i++) {
			if (ccx < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMax) &&
				ccz < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMax) &&
				ccx >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMin) &&
				ccz >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMin)) return 1;
		}
	}

	v.y = h;

	float hh = GetLandH(v.x - 64, v.z - 64);
	if (fabs(hh - h) > 100) return 1;
	hh = GetLandH(v.x + 64, v.z - 64);
	if (fabs(hh - h) > 100) return 1;
	hh = GetLandH(v.x - 64, v.z + 64);
	if (fabs(hh - h) > 100) return 1;
	hh = GetLandH(v.x + 64, v.z + 64);
	if (fabs(hh - h) > 100) return 1;

	if (mc)
		for (int z = -2; z <= 2; z++)
			for (int x = -2; x <= 2; x++)
				if (OMap[ccz + z][ccx + x] != 255)
				{
					int ob = OMap[ccz + z][ccx + x];
					if (MObjects[ob].info.Radius < 10) continue;
					float CR = (float)MObjects[ob].info.Radius + 64;

					float oz = (ccz + z) * 256.f + 128.f;
					float ox = (ccx + x) * 256.f + 128.f;

					float r = (float)sqrt((ox - v.x)*(ox - v.x) + (oz - v.z)*(oz - v.z));
					if (r < CR) return 1;
				}

	return 0;
}

int CheckPlaceCollisionLandBrahi(TCharacter *cptr, Vector3d &v, BOOL wc, BOOL mc)
{
	int ccx = (int)v.x / 256;
	int ccz = (int)v.z / 256;

	if (ccx < 4 || ccz < 4 || ccx>1018 || ccz>1018) return 1;

	/*if (wc)
		if ((FMap[ccz][ccx - 1] | FMap[ccz - 1][ccx] | FMap[ccz - 1][ccx - 1] |
			FMap[ccz][ccx] |
			FMap[ccz + 1][ccx] | FMap[ccz][ccx + 1] | FMap[ccz + 1][ccx + 1]) & fmWater)
			return 1;
	 */

	if (DinoInfo[cptr->CType].AvoidCount)
	{
		for (int i = 0; i < DinoInfo[cptr->CType].AvoidCount; i++) {
			if (ccx < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMax) &&
				ccz < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMax) &&
				ccx >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMin) &&
				ccz >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMin)) return 1;
		}
	}

	if (wc){
		if ((FMap[ccz][ccx - 1] | FMap[ccz - 1][ccx] | FMap[ccz - 1][ccx - 1] |
			FMap[ccz][ccx] |
			FMap[ccz + 1][ccx] | FMap[ccz][ccx + 1] | FMap[ccz + 1][ccx + 1]) & fmWater){ 
			return 1;
		}
	} else {
		if ((GetLandUpH(v.x, v.z) - GetLandH(v.x, v.z)) > 550 ||
			(GetLandUpH(v.x + 256, v.z) - GetLandH(v.x + 256, v.z)) > 550 ||
			(GetLandUpH(v.x, v.z + 256) - GetLandH(v.x, v.z + 256)) > 550 ||
			(GetLandUpH(v.x + 256, v.z + 256) - GetLandH(v.x + 256, v.z + 256)) > 550 ||
			(GetLandUpH(v.x - 256, v.z) - GetLandH(v.x - 256, v.z)) > 550 ||
			(GetLandUpH(v.x, v.z - 256) - GetLandH(v.x, v.z - 256)) > 550 ||
			(GetLandUpH(v.x - 256, v.z - 256) - GetLandH(v.x - 256, v.z - 256)) > 550 ||
			(GetLandUpH(v.x + 256, v.z - 256) - GetLandH(v.x + 256, v.z - 256)) > 550 ||
			(GetLandUpH(v.x - 256, v.z + 256) - GetLandH(v.x - 256, v.z + 256)) > 550) return 1;
	}

	float h = GetLandH(v.x, v.z);
	if (fabs(h - v.y) > 64) return 1;

	v.y = h;

	float hh = GetLandH(v.x - 64, v.z - 64);
	if (fabs(hh - h) > 100) return 1;
	hh = GetLandH(v.x + 64, v.z - 64);
	if (fabs(hh - h) > 100) return 1;
	hh = GetLandH(v.x - 64, v.z + 64);
	if (fabs(hh - h) > 100) return 1;
	hh = GetLandH(v.x + 64, v.z + 64);
	if (fabs(hh - h) > 100) return 1;

	if (mc)
		for (int z = -2; z <= 2; z++)
			for (int x = -2; x <= 2; x++)
				if (OMap[ccz + z][ccx + x] != 255)
				{
					int ob = OMap[ccz + z][ccx + x];
					if (MObjects[ob].info.Radius < 10) continue;
					float CR = (float)MObjects[ob].info.Radius + 64;

					float oz = (ccz + z) * 256.f + 128.f;
					float ox = (ccx + x) * 256.f + 128.f;

					float r = (float)sqrt((ox - v.x)*(ox - v.x) + (oz - v.z)*(oz - v.z));
					if (r < CR) return 1;
				}

	return 0;
}

int CheckPlaceCollisionBrahi(TCharacter *cptr, Vector3d &v, BOOL wc, BOOL mc)
{
	int ccx = (int)v.x / 256;
	int ccz = (int)v.z / 256;

	if (ccx < 4 || ccz < 4 || ccx>1018 || ccz>1018) return 1;

	/*if (wc)
		if ((FMap[ccz][ccx - 1] | FMap[ccz - 1][ccx] | FMap[ccz - 1][ccx - 1] |
			FMap[ccz][ccx] |
			FMap[ccz + 1][ccx] | FMap[ccz][ccx + 1] | FMap[ccz + 1][ccx + 1]) & fmWater)
			return 1;
	 */

	if (DinoInfo[cptr->CType].AvoidCount)
	{
		for (int i = 0; i < DinoInfo[cptr->CType].AvoidCount; i++) {
			if (ccx < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMax) &&
				ccz < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMax) &&
				ccx >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMin) &&
				ccz >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMin)) return 1;
		}
	}

	if (wc) {
		if ((GetLandUpH(v.x, v.z) - GetLandH(v.x, v.z)) > 550 ||
			(GetLandUpH(v.x + 256, v.z) - GetLandH(v.x + 256, v.z)) > 550 ||
			(GetLandUpH(v.x, v.z + 256) - GetLandH(v.x, v.z + 256)) > 550 ||
			(GetLandUpH(v.x + 256, v.z + 256) - GetLandH(v.x + 256, v.z + 256)) > 550 ||
			(GetLandUpH(v.x - 256, v.z) - GetLandH(v.x - 256, v.z)) > 550 ||
			(GetLandUpH(v.x, v.z - 256) - GetLandH(v.x, v.z - 256)) > 550 ||
			(GetLandUpH(v.x - 256, v.z - 256) - GetLandH(v.x - 256, v.z - 256)) > 550 ||
			(GetLandUpH(v.x + 256, v.z - 256) - GetLandH(v.x + 256, v.z - 256)) > 550 ||
			(GetLandUpH(v.x - 256, v.z + 256) - GetLandH(v.x - 256, v.z + 256)) > 550) return 1;
	}

	float h = GetLandH(v.x, v.z);
	if (fabs(h - v.y) > 64) return 1;

	v.y = h;

	float hh = GetLandH(v.x - 64, v.z - 64);
	if (fabs(hh - h) > 100) return 1;
	hh = GetLandH(v.x + 64, v.z - 64);
	if (fabs(hh - h) > 100) return 1;
	hh = GetLandH(v.x - 64, v.z + 64);
	if (fabs(hh - h) > 100) return 1;
	hh = GetLandH(v.x + 64, v.z + 64);
	if (fabs(hh - h) > 100) return 1;

	if (mc)
		for (int z = -2; z <= 2; z++)
			for (int x = -2; x <= 2; x++)
				if (OMap[ccz + z][ccx + x] != 255)
				{
					int ob = OMap[ccz + z][ccx + x];
					if (MObjects[ob].info.Radius < 10) continue;
					float CR = (float)MObjects[ob].info.Radius + 64;

					float oz = (ccz + z) * 256.f + 128.f;
					float ox = (ccx + x) * 256.f + 128.f;

					float r = (float)sqrt((ox - v.x)*(ox - v.x) + (oz - v.z)*(oz - v.z));
					if (r < CR) return 1;
				}

	return 0;
}






int CheckPlaceCollision2(TCharacter *cptr, Vector3d &v, BOOL wc)
{
	int ccx = (int)v.x / 256;
	int ccz = (int)v.z / 256;

	if (ccx < 4 || ccz < 4 || ccx>1018 || ccz>1018) return 1;

	if (wc)
		if ((FMap[ccz][ccx - 1] | FMap[ccz - 1][ccx] | FMap[ccz - 1][ccx - 1] |
			FMap[ccz][ccx] |
			FMap[ccz + 1][ccx] | FMap[ccz][ccx + 1] | FMap[ccz + 1][ccx + 1]) & fmWater)
			return 1;

	if (DinoInfo[cptr->CType].AvoidCount)
	{
		for (int i = 0; i < DinoInfo[cptr->CType].AvoidCount; i++) {
			if (ccx < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMax) &&
				ccz < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMax) &&
				ccx >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMin) &&
				ccz >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMin)) return 1;
		}
	}

	float h = GetLandH(v.x, v.z);
	/*if (! (FMap[ccz][ccx] & fmWater) )
	  if (fabs(h - v.y) > 64) return 1;*/
	v.y = h;

	float hh = GetLandH(v.x - 64, v.z - 64);
	if (fabs(hh - h) > 100) return 1;
	hh = GetLandH(v.x + 64, v.z - 64);
	if (fabs(hh - h) > 100) return 1;
	hh = GetLandH(v.x - 64, v.z + 64);
	if (fabs(hh - h) > 100) return 1;
	hh = GetLandH(v.x + 64, v.z + 64);
	if (fabs(hh - h) > 100) return 1;

	return 0;
}



int CheckPossiblePath(TCharacter *cptr, BOOL wc, BOOL mc)
{
	Vector3d p = cptr->pos;
	float lookx = (float)cos(cptr->tgalpha);
	float lookz = (float)sin(cptr->tgalpha);
	int c = 0;
	for (int t = 0; t < 20; t++)
	{

		if (cptr->Clone == AI_BRACH) {
			p.x += lookx * 256.f;
			p.z += lookz * 256.f;
			if (CheckPlaceCollisionBrahi(cptr, p, wc, mc)) c++;
		}
		else if (cptr->Clone == AI_BRACHDANGER) {
			p.x += lookx * DinoInfo[cptr->CType].maxGrad;//128
			p.z += lookz * DinoInfo[cptr->CType].maxGrad;//128
			if (CheckPlaceCollisionBrahi(cptr, p, wc, mc)) c++;
		} if (cptr->Clone == AI_LANDBRACH) {
			p.x += lookx * DinoInfo[cptr->CType].maxGrad;//128
			p.z += lookz * DinoInfo[cptr->CType].maxGrad;//128
			if (CheckPlaceCollisionLandBrahi(cptr, p, wc, mc)) c++;
		}
		else if ((cptr->Clone == AI_MOSA)) {
			p.x += lookx * 64.f;
			p.z += lookz * 64.f;
			if (CheckPlaceCollisionMosasaurus(cptr, p, cptr->depth)) c++;

		}
		else if ((cptr->Clone == AI_FISH)) {
			p.x += lookx * 64.f;
			p.z += lookz * 64.f;
			if (CheckPlaceCollisionFish(cptr, p, cptr->depth,
				DinoInfo[cptr->CType].maxDepth,
				DinoInfo[cptr->CType].minDepth)) c++;

		}
		else {
			p.x += lookx * 64.f;
			p.z += lookz * 64.f;
			if (CheckPlaceCollision(cptr, p, wc, mc)) c++;
		}
	}
	return c;
}


void LookForAWay(TCharacter *cptr, BOOL wc, BOOL mc)
{
	float alpha = cptr->tgalpha;
	float dalpha = 15.f;
	float afound = alpha;
	int maxp = 16;
	int curp;

	if (!CheckPossiblePath(cptr, wc, mc))
	{
		cptr->NoWayCnt = 0;
		return;
	}

	cptr->NoWayCnt++;
	for (int i = 0; i < 12; i++)
	{
		cptr->tgalpha = alpha + dalpha * pi / 180.f;
		curp = CheckPossiblePath(cptr, wc, mc) + (i >> 1);
		if (!curp) return;
		if (curp < maxp)
		{
			maxp = curp;
			afound = cptr->tgalpha;
		}

		cptr->tgalpha = alpha - dalpha * pi / 180.f;
		curp = CheckPossiblePath(cptr, wc, mc) + (i >> 1);
		if (!curp) return;
		if (curp < maxp)
		{
			maxp = curp;
			afound = cptr->tgalpha;
		}

		dalpha += 15.f;
	}

	cptr->tgalpha = afound;
}




BOOL ReplaceCharacterForward(TCharacter *cptr)
{
	float al = CameraAlpha + (float)siRand(2048) / 2048.f;
	float sa = (float)sin(al);
	float ca = (float)cos(al);
	Vector3d p;
	p.x = PlayerX + sa * (ctViewR + rRand(10)) * 256;
	p.z = PlayerZ - ca * (ctViewR + rRand(10)) * 256;
	p.y = GetLandH(p.x, p.z);

	if (p.x < 16 * 256) return FALSE;
	if (p.z < 16 * 256) return FALSE;
	if (p.x > 1000 * 256) return FALSE;
	if (p.z > 1000 * 256) return FALSE;

	if (CheckPlaceCollisionP(p)) return FALSE;

	cptr->State = 0;
	cptr->pos = p;
	//cptr->tgx = cptr->pos.x + siRand(2048);
	//cptr->tgz = cptr->pos.z + siRand(2048);
	SetNewTargetPlace(cptr, 2048);

	if (cptr->Clone == AI_DIMOR) //===== dimor ========//
		cptr->pos.y += 1048.f;
	return TRUE;
}


void Characters_AddSecondaryOne(int ctype)
{
	if (ChCount > 64) return;
	Characters[ChCount].CType = ctype;
	int tr = 0;
replace1:
	tr++;
	if (tr > 128) return;
	Characters[ChCount].pos.x = PlayerX + siRand(20040);
	Characters[ChCount].pos.z = PlayerZ + siRand(20040);
	Characters[ChCount].pos.y = GetLandH(Characters[ChCount].pos.x,
		Characters[ChCount].pos.z);

	if (CheckPlaceCollisionP(Characters[ChCount].pos)) goto replace1;

	if (fabs(Characters[ChCount].pos.x - PlayerX) +
		fabs(Characters[ChCount].pos.z - PlayerZ) < 256 * 40)
		goto replace1;

	Characters[ChCount].tgx = Characters[ChCount].pos.x;
	Characters[ChCount].tgz = Characters[ChCount].pos.z;
	Characters[ChCount].tgtime = 0;

	ResetCharacter(&Characters[ChCount]);
	ChCount++;
}



void MoveCharacterFish(TCharacter *cptr, float dx, float dz)
{
	//return;
	Vector3d p = cptr->pos;

	if (CheckPlaceCollisionFish(cptr, p, cptr->depth,
		DinoInfo[cptr->CType].maxDepth,
		DinoInfo[cptr->CType].minDepth))
	{
		cptr->pos.x += dx / 2;
		cptr->pos.z += dz / 2;
		return;
	}

	p.x += dx;
	p.z += dz;

	if (!CheckPlaceCollisionFish(cptr, p, cptr->depth,
		DinoInfo[cptr->CType].maxDepth,
		DinoInfo[cptr->CType].minDepth))
	{
		cptr->pos = p;
		return;
	}

	p = cptr->pos;
	p.x += dx / 2;
	p.z += dz / 2;
	if (!CheckPlaceCollisionFish(cptr, p, cptr->depth,
		DinoInfo[cptr->CType].maxDepth,
		DinoInfo[cptr->CType].minDepth)) cptr->pos = p;
	p = cptr->pos;

	p.x += dx / 4;
	//if (!CheckPlaceCollision2(p)) cptr->pos = p;
	p.z += dz / 4;
	//if (!CheckPlaceCollision2(p)) cptr->pos = p;
	cptr->pos = p;
}



void MoveCharacterMosasaurus(TCharacter *cptr, float dx, float dz)
{
	//return;
	Vector3d p = cptr->pos;

	if (CheckPlaceCollisionMosasaurus(cptr, p, cptr->depth))
	{
		cptr->pos.x += dx / 2;
		cptr->pos.z += dz / 2;
		return;
	}

	p.x += dx;
	p.z += dz;

	if (!CheckPlaceCollisionMosasaurus(cptr, p, cptr->depth))
	{
		cptr->pos = p;
		return;
	}

	p = cptr->pos;
	p.x += dx / 2;
	p.z += dz / 2;
	if (!CheckPlaceCollisionMosasaurus(cptr, p, cptr->depth)) cptr->pos = p;
	p = cptr->pos;

	p.x += dx / 4;
	//if (!CheckPlaceCollision2(p)) cptr->pos = p;
	p.z += dz / 4;
	//if (!CheckPlaceCollision2(p)) cptr->pos = p;
	cptr->pos = p;
}



void MoveCharacter(TCharacter *cptr, float dx, float dz, BOOL wc, BOOL mc)
{
	//return;
	Vector3d p = cptr->pos;

	if (CheckPlaceCollision2(cptr, p, wc))
	{
		cptr->pos.x += dx / 2;
		cptr->pos.z += dz / 2;
		return;
	}

	p.x += dx;
	p.z += dz;

	if (!CheckPlaceCollision2(cptr, p, wc))
	{
		cptr->pos = p;
		return;
	}

	p = cptr->pos;
	p.x += dx / 2;
	p.z += dz / 2;
	if (!CheckPlaceCollision2(cptr, p, wc)) cptr->pos = p;
	p = cptr->pos;

	p.x += dx / 4;
	//if (!CheckPlaceCollision2(p)) cptr->pos = p;
	p.z += dz / 4;
	//if (!CheckPlaceCollision2(p)) cptr->pos = p;
	cptr->pos = p;
}




void MoveCharacter2(TCharacter *cptr, float dx, float dz)
{
	cptr->pos.x += dx;
	cptr->pos.z += dz;
}


void SetNewTargetPlace_Icth(TCharacter *cptr, float R)
{
	Vector3d p;
	int tr = 0;
	bool outsideRegion = false;
	bool stayRegion = false;
	//float spacing;
	if (Region[cptr->RType].stayInRegion && cptr->AI < 0) {
		stayRegion = true;
		//spacing = (DinoInfo[cptr->CType].XMax - DinoInfo[cptr->CType].XMin)/10;
		//if (spacing > 20) spacing = 20;
		if (cptr->pos.x < (Region[cptr->RType].XMin) * 256) outsideRegion = true;
		if (cptr->pos.x > (Region[cptr->RType].XMax) * 256) outsideRegion = true;
		if (cptr->pos.z < (Region[cptr->RType].YMin) * 256) outsideRegion = true;
		if (cptr->pos.z > (Region[cptr->RType].YMax) * 256) outsideRegion = true;
	}
	//PrintLog("iT");//TEST20200412
replace:
	//PrintLog("-");//TEST20200412
	p.x = cptr->pos.x + siRand((int)R);
	p.z = cptr->pos.z + siRand((int)R);
	if (stayRegion) {
		if (p.x < (Region[cptr->RType].XMin) * 256) p.x = Region[cptr->RType].XMin * 256;
		if (p.x > (Region[cptr->RType].XMax) * 256) p.x = Region[cptr->RType].XMax * 256;
		if (p.z < (Region[cptr->RType].YMin) * 256) p.z = Region[cptr->RType].YMin * 256;
		if (p.z > (Region[cptr->RType].YMax) * 256) p.z = Region[cptr->RType].YMax * 256;
	}
	if (p.x < 512) p.x = 512;
	if (p.x > 1018 * 256) p.x = 1018 * 256;
	if (p.z < 512) p.z = 512;
	if (p.z > 1018 * 256) p.z = 1018 * 256;
	tr++;
	if (tr < 16)
		if (fabs(p.x - cptr->pos.x) + fabs(p.z - cptr->pos.z) < R / 2.f) goto replace;

	if (DinoInfo[cptr->CType].AvoidCount && tr < 128)
	{
		for (int i = 0; i < DinoInfo[cptr->CType].AvoidCount; i++) {
			if (Characters[ChCount].pos.x < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMax + 1) * 256 &&
				Characters[ChCount].pos.z < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMax + 1) * 256 &&
				Characters[ChCount].pos.x >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMin - 1) * 256 &&
				Characters[ChCount].pos.z >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMin - 1) * 256) goto replace;
		}
	}

	if (stayRegion && outsideRegion && tr > 64) {
		if (fabs(p.x - cptr->pos.x) + fabs(p.z - cptr->pos.z) > R * 25.f) {
			stayRegion = false;
			goto replace;
		}
	}

	//if (tr < 128)
	if (tr < 1024)
	{
		if (!waterNear(p.x, p.z, 50)) goto replace;
		if (cptr->spawnAlt + 400 < GetLandUpH(p.x, p.z)) goto replace;
	}
	/*
	else if (R < 10240 && !(tr % 20)) {
		if (!waterNear(p.x, p.z, 50)) {
			R *= 2;
			goto replace;
		}
	}
	*/

	if (tr < 256)
		if (AquaticCheckPlaceCollisionP(p)) goto replace;

	cptr->tgtime = 0;
	cptr->tgx = p.x;
	cptr->tgz = p.z;
}



void SetNewTargetPlace_IcthOld(TCharacter *cptr, float R)
{
	Vector3d p;
	int tr = 0;
	PrintLog("iT");//TEST20200412
replace:
	PrintLog("-");//TEST20200412
	p.x = cptr->pos.x + siRand((int)R);
	if (p.x < 512) p.x = 512;
	if (p.x > 1018 * 256) p.x = 1018 * 256;
	p.z = cptr->pos.z + siRand((int)R);
	if (p.z < 512) p.z = 512;
	if (p.z > 1018 * 256) p.z = 1018 * 256;
	tr++;
	if (tr < 16)
		if (fabs(p.x - cptr->pos.x) + fabs(p.z - cptr->pos.z) < R / 2.f) goto replace;

	if (tr < 128)
	{
		if (!waterNear(p.x, p.z, 50)) goto replace;
	}

	if (tr < 256)
		if (AquaticCheckPlaceCollisionP(p)) goto replace;

	cptr->tgtime = 0;
	cptr->tgx = p.x;
	cptr->tgz = p.z;
}



void SetNewTargetPlace(TCharacter *cptr, float R)
{

	if (cptr->AI < 0) {
		SetNewTargetPlaceRegion(cptr, R);
	}
	else {
		SetNewTargetPlaceVanilla(cptr, R);
	}


}



void SetNewTargetPlaceVanilla(TCharacter *cptr, float R)
{
	Vector3d p;
	int tr = 0;
	//PrintLog("PAR_START--");
replace:
	//PrintLog("PAR_IT--");
	p.x = cptr->pos.x + siRand((int)R);
	if (p.x < 512) p.x = 512;
	if (p.x > 1018 * 256) p.x = 1018 * 256;
	p.z = cptr->pos.z + siRand((int)R);
	if (p.z < 512) p.z = 512;
	if (p.z > 1018 * 256) p.z = 1018 * 256;
	p.y = GetLandH(p.x, p.z);
	tr++;
	if (tr < 128)
		if (fabs(p.x - cptr->pos.x) + fabs(p.z - cptr->pos.z) < R / 2.f) goto replace;

	R += 512;

	if (tr < 256)
		if (CheckPlaceCollisionP(p)) goto replace;

	cptr->tgtime = 0;
	cptr->tgx = p.x;
	cptr->tgz = p.z;
}



void SetNewTargetPlaceRegion(TCharacter *cptr, float R)
{
	Vector3d p;
	int tr = 0;
	bool outsideRegion = false;
	bool stayRegion = false;
	//float spacing;
	if (Region[cptr->RType].stayInRegion && cptr->AI < 0) {
		stayRegion = true;
		//spacing = (DinoInfo[cptr->CType].XMax - DinoInfo[cptr->CType].XMin)/10;
		//if (spacing > 20) spacing = 20;
		if (cptr->pos.x < (Region[cptr->RType].XMin) * 256) outsideRegion = true;
		if (cptr->pos.x > (Region[cptr->RType].XMax) * 256) outsideRegion = true;
		if (cptr->pos.z < (Region[cptr->RType].YMin) * 256) outsideRegion = true;
		if (cptr->pos.z > (Region[cptr->RType].YMax) * 256) outsideRegion = true;
	}
	//PrintLog("hT");//TEST20200415
replace:
	//PrintLog("-");//TEST20200415
	p.x = cptr->pos.x + siRand((int)R);
	p.z = cptr->pos.z + siRand((int)R);
	if (stayRegion) {
		if (p.x < (Region[cptr->RType].XMin) * 256) p.x = Region[cptr->RType].XMin * 256;
		if (p.x > (Region[cptr->RType].XMax) * 256) p.x = Region[cptr->RType].XMax * 256;
		if (p.z < (Region[cptr->RType].YMin) * 256) p.z = Region[cptr->RType].YMin * 256;
		if (p.z > (Region[cptr->RType].YMax) * 256) p.z = Region[cptr->RType].YMax * 256;
	}
	if (p.x < 512) p.x = 512;
	if (p.x > 1018 * 256) p.x = 1018 * 256;

	if (p.z < 512) p.z = 512;
	if (p.z > 1018 * 256) p.z = 1018 * 256;
	p.y = GetLandH(p.x, p.z);
	tr++;
	if (tr < 128) {
		if (fabs(p.x - cptr->pos.x) + fabs(p.z - cptr->pos.z) < R / 2.f) goto replace;

		if (DinoInfo[cptr->CType].AvoidCount)
		{
			for (int i = 0; i < DinoInfo[cptr->CType].AvoidCount; i++) {
				if (Characters[ChCount].pos.x < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMax + 1) * 256 &&
					Characters[ChCount].pos.z < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMax + 1) * 256 &&
					Characters[ChCount].pos.x >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMin - 1) * 256 &&
					Characters[ChCount].pos.z >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMin - 1) * 256) goto replace;
			}
		}

	}

	if (stayRegion && outsideRegion && tr > 64) {
		if (fabs(p.x - cptr->pos.x) + fabs(p.z - cptr->pos.z) > 2048 * 25.f) {
			stayRegion = false;
			goto replace;
		}
	}

	R += 512;

	if (tr < 256)
		if (CheckPlaceCollisionP(p)) goto replace;

	cptr->tgtime = 0;
	cptr->tgx = p.x;
	cptr->tgz = p.z;
}



void SetNewTargetPlace_Brahi(TCharacter *cptr, float R)
{
	Vector3d p;
	int tr = 0;
	//PrintLog("bT");//TEST202004111501
replace:
	//PrintLog("-");//TEST202004111501
	p.x = cptr->pos.x + siRand((int)R);
	if (p.x < 512) p.x = 512;
	if (p.x > 1018 * 256) p.x = 1018 * 256;
	p.z = cptr->pos.z + siRand((int)R);
	if (p.z < 512) p.z = 512;
	if (p.z > 1018 * 256) p.z = 1018 * 256;
	tr++;
	if (tr < 16)
		if (fabs(p.x - cptr->pos.x) + fabs(p.z - cptr->pos.z) < R / 2.f) goto replace;

	p.y = GetLandH(p.x, p.z);
	float wy = GetLandUpH(p.x, p.z) - p.y;

	if (tr < 128)
	{
		if (cptr->Clone == AI_LANDBRACH) {
			if (DinoInfo[cptr->CType].canSwim) {
				if (wy > 400) goto replace;
			} else {
				if (wy > 0) goto replace;
			}
		} else {
			if (wy > 400) goto replace;
			if (wy < 200) goto replace;
		}
		if (DinoInfo[cptr->CType].AvoidCount)
		{
			for (int i = 0; i < DinoInfo[cptr->CType].AvoidCount; i++) {
				if (Characters[ChCount].pos.x < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMax + 1) * 256 &&
					Characters[ChCount].pos.z < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMax + 1) * 256 &&
					Characters[ChCount].pos.x >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMin - 1) * 256 &&
					Characters[ChCount].pos.z >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMin - 1) * 256) goto replace;
			}
		}
	}

	cptr->tgtime = 0;
	cptr->tgx = p.x;
	cptr->tgz = p.z;
}



void SetNewTargetPlaceFish(TCharacter *cptr, float R)
{
	Vector3d p;
	int tr = 0;
	bool outsideRegion = false;
	bool stayRegion = false;
	//float spacing;
	if (Region[cptr->RType].stayInRegion && cptr->AI < 0) {
		stayRegion = true;
		//spacing = (DinoInfo[cptr->CType].XMax - DinoInfo[cptr->CType].XMin)/10;
		//if (spacing > 20) spacing = 20;
		if (cptr->pos.x < (Region[cptr->RType].XMin) * 256) outsideRegion = true;
		if (cptr->pos.x > (Region[cptr->RType].XMax) * 256) outsideRegion = true;
		if (cptr->pos.z < (Region[cptr->RType].YMin) * 256) outsideRegion = true;
		if (cptr->pos.z > (Region[cptr->RType].YMax) * 256) outsideRegion = true;
	}
	//PrintLog("fTXZ");//TEST202004091129
replace:
	//PrintLog("-");//TEST202004091129
	p.x = cptr->pos.x + siRand((int)R);
	p.z = cptr->pos.z + siRand((int)R);
	if (stayRegion) {
		if (p.x < (Region[cptr->RType].XMin) * 256) p.x = Region[cptr->RType].XMin * 256;
		if (p.x > (Region[cptr->RType].XMax) * 256) p.x = Region[cptr->RType].XMax * 256;
		if (p.z < (Region[cptr->RType].YMin) * 256) p.z = Region[cptr->RType].YMin * 256;
		if (p.z > (Region[cptr->RType].YMax) * 256) p.z = Region[cptr->RType].YMax * 256;
	}
	if (p.x < 512) p.x = 512;
	if (p.x > 1018 * 256) p.x = 1018 * 256;
	if (p.z < 512) p.z = 512;
	if (p.z > 1018 * 256) p.z = 1018 * 256;

	tr++;
	if (tr < 128) {
		if (fabs(p.x - cptr->pos.x) + fabs(p.z - cptr->pos.z) < R / 2.f) goto replace;

		if (DinoInfo[cptr->CType].AvoidCount)
		{
			for (int i = 0; i < DinoInfo[cptr->CType].AvoidCount; i++) {
				if (Characters[ChCount].pos.x < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMax + 1) * 256 &&
					Characters[ChCount].pos.z < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMax + 1) * 256 &&
					Characters[ChCount].pos.x >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMin - 1) * 256 &&
					Characters[ChCount].pos.z >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMin - 1) * 256) goto replace;
			}
		}

	}

	if (stayRegion && outsideRegion && tr > 64) {
		if (fabs(p.x - cptr->pos.x) + fabs(p.z - cptr->pos.z) > R * 25.f) {
			stayRegion = false;
			goto replace;
		}
	}

	p.y = GetLandH(p.x, p.z);
	float wy = GetLandUpH(p.x, p.z) - GetLandH(p.x, p.z);

	if (tr < 2024)
	{

		if (wy < DinoInfo[cptr->CType].minDepth) {
			if (stayRegion && outsideRegion && tr > 64) stayRegion = false; //dont change > sign, I only want to stop trying to return to region is tries is under 64
			goto replace;
		}
		if (wy > DinoInfo[cptr->CType].maxDepth) {
			if (stayRegion && outsideRegion && tr > 64) stayRegion = false;
			goto replace;
		}

	}

	float targetDepthTemp;
	float tdistTemp = fabs((float)sqrt(
		((p.x - cptr->pos.x)*(p.x - cptr->pos.x)) +
		((p.z - cptr->pos.z) * (p.z - cptr->pos.z))));
	/*
	float tdistTemp = fabs((float)sqrt(
		((p.x - cptr->pos.x)*(p.x - cptr->pos.x)) +
		((p.z - cptr->pos.z) * (p.z - cptr->pos.z))) / 3);
	*/
	tr = 0;

	//PrintLog("fTY");//TEST202004091129

replace2:
	//PrintLog("-");//TEST202004091129
	targetDepthTemp = siRand((int)(tdistTemp));
	//targetDepthTemp = siRand((int)(R/3));
	tr++;

	/*
	if (tr < 1024) {
		if (fabs(targetDepthTemp) > tdistTemp) {
			if (targetDepthTemp > 0) {
				targetDepthTemp = tdistTemp;
			}
			else {
				targetDepthTemp = -tdistTemp;
			}
		}
	}
	*/

	targetDepthTemp = cptr->depth + targetDepthTemp;
	if (targetDepthTemp < GetLandH(p.x, p.z) + DinoInfo[cptr->CType].spacingDepth) {
		if (tr < 3024) {
			goto replace2;
		}
		else {
			targetDepthTemp = GetLandH(p.x, p.z) + DinoInfo[cptr->CType].spacingDepth;
		}
	}
	if (targetDepthTemp > GetLandUpH(p.x, p.z) - DinoInfo[cptr->CType].spacingDepth) {
		if (tr < 3024) {
			goto replace2;
		}
		else {
			targetDepthTemp = GetLandUpH(p.x, p.z) - DinoInfo[cptr->CType].spacingDepth;
		}
	}

	cptr->tgtime = 0;
	cptr->tgx = p.x;
	cptr->tgz = p.z;
	cptr->tdepth = targetDepthTemp;
	cptr->lastTBeta = cptr->beta;
	cptr->turny = 0;
}



void SetNewTargetPlaceMosasaurus(TCharacter *cptr, float R)
{
	Vector3d p;
	int tr = 0;
replace:
	p.x = cptr->pos.x + siRand((int)R);
	if (p.x < 512) p.x = 512;
	if (p.x > 1018 * 256) p.x = 1018 * 256;
	p.z = cptr->pos.z + siRand((int)R);
	if (p.z < 512) p.z = 512;
	if (p.z > 1018 * 256) p.z = 1018 * 256;

	tr++;
	//if (tr < 128)
	if (fabs(p.x - cptr->pos.x) + fabs(p.z - cptr->pos.z) < R / 2.f) goto replace;

	p.y = GetLandH(p.x, p.z);
	float wy = GetLandUpH(p.x, p.z) - GetLandH(p.x, p.z);

	if (tr < 8024)
	{
		if (wy < 1500) goto replace;
	}

	if (tr < 128)
	{
		if (DinoInfo[cptr->CType].AvoidCount)
		{
			for (int i = 0; i < DinoInfo[cptr->CType].AvoidCount; i++) {
				if (Characters[ChCount].pos.x < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMax + 1) * 256 &&
					Characters[ChCount].pos.z < (Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMax + 1) * 256 &&
					Characters[ChCount].pos.x >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].XMin - 1) * 256 &&
					Characters[ChCount].pos.z >(Avoid[DinoInfo[cptr->CType].Avoidances[i]].YMin - 1) * 256) goto replace;
			}
		}
	}

	float targetDepthTemp;
	float tdistTemp = fabs((float)sqrt(
		((p.x - cptr->pos.x)*(p.x - cptr->pos.x)) +
		((p.z - cptr->pos.z) * (p.z - cptr->pos.z))) / 3);
	/*
	float tdistTemp = fabs((float)sqrt(
		((p.x - cptr->pos.x)*(p.x - cptr->pos.x)) +
		((p.z - cptr->pos.z) * (p.z - cptr->pos.z))) / 3);
	*/
replace2:
	targetDepthTemp = siRand((int)(R / 3));
	//targetDepthTemp = siRand((int)(R/3));
	tr++;

	if (tr < 1024) {
		if (fabs(targetDepthTemp) > tdistTemp) {
			if (targetDepthTemp > 0) {
				targetDepthTemp = tdistTemp;
			}
			else {
				targetDepthTemp = -tdistTemp;
			}
		}
	}

	targetDepthTemp = cptr->depth + targetDepthTemp;
	if (targetDepthTemp < GetLandH(p.x, p.z) + 400) {
		if (tr < 8024) {
			goto replace2;
		}
		else {
			targetDepthTemp = GetLandH(p.x, p.z) + 400;
		}
	}
	if (targetDepthTemp > GetLandUpH(p.x, p.z) - 700) {
		if (tr < 8024) {
			goto replace2;
		}
		else {
			targetDepthTemp = GetLandUpH(p.x, p.z) - 700;
		}
	}

	cptr->tgtime = 0;
	cptr->tgx = p.x;
	cptr->tgz = p.z;
	cptr->tdepth = targetDepthTemp;
	cptr->lastTBeta = cptr->beta;
	cptr->turny = 0;
}





void AnimateHuntDead(TCharacter *cptr)
{

	//if (!cptr->FTime) ActivateCharacterFx(cptr);

	ProcessPrevPhase(cptr);
	BOOL NewPhase = FALSE;

	cptr->FTime += TimeDt;
	if (cptr->FTime >= cptr->pinfo->Animation[cptr->Phase].AniTime)
	{
		NewPhase = TRUE;
		if (cptr->Phase == 2)
			cptr->FTime = cptr->pinfo->Animation[cptr->Phase].AniTime - 1;
		else
			cptr->FTime = 0;

		if (cptr->Phase == 1)
		{
			cptr->FTime = 0;
			cptr->Phase = 2;
		}

		ActivateCharacterFx(cptr);
	}


	float h = GetLandH(cptr->pos.x, cptr->pos.z);
	DeltaFunc(cptr->pos.y, h, TimeDt / 5.f);

	if (cptr->Phase == 2)
		if (cptr->pos.y > h + 3)
		{
			cptr->FTime = 0;
			//MessageBeep(0xFFFFFFFF);
		}


	if (cptr->pos.y < h + 256)
	{
		//=== beta ===//
		float blook = 256;
		float hlook = GetLandH(cptr->pos.x + cptr->lookx * blook, cptr->pos.z + cptr->lookz * blook);
		float hlook2 = GetLandH(cptr->pos.x - cptr->lookx * blook, cptr->pos.z - cptr->lookz * blook);
		DeltaFunc(cptr->beta, (hlook2 - hlook) / (blook * 3.2f), TimeDt / 1800.f);

		if (cptr->beta > 0.4f) cptr->beta = 0.4f;
		if (cptr->beta < -0.4f) cptr->beta = -0.4f;

		//=== gamma ===//
		float glook = 256;
		hlook = GetLandH(cptr->pos.x + cptr->lookz * glook, cptr->pos.z - cptr->lookx*glook);
		hlook2 = GetLandH(cptr->pos.x - cptr->lookz * glook, cptr->pos.z + cptr->lookx*glook);
		cptr->tggamma = (hlook - hlook2) / (glook * 3.2f);
		if (cptr->tggamma > 0.4f) cptr->tggamma = 0.4f;
		if (cptr->tggamma < -0.4f) cptr->tggamma = -0.4f;
		DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 1800.f);
	}

//	if (!(GetLandUpH(killerDino->pos.x, killerDino->pos.z) - GetLandH(killerDino->pos.x, killerDino->pos.z) >
//		DinoInfo[killerDino->CType].waterLevel * killerDino->scale))
	if (!killedwater)
	{
		if (DinoInfo[killerDino->CType].killTypeCount) {
			if (DinoInfo[killerDino->CType].killType[killerDino->killType].elevate) {


				cptr->pos = killerDino->pos;
				cptr->FTime = killerDino->FTime;
				cptr->beta = killerDino->beta;
				cptr->gamma = killerDino->gamma;
				cptr->scale = killerDino->scale;
				cptr->alpha = killerDino->alpha;
				killerDino->bend = 0;

			}

		}

	}

}



void AnimateTRexDead(TCharacter *cptr)
{

	if (cptr->Phase != DinoInfo[cptr->CType].dieAnim)
	{
		if (cptr->PPMorphTime > 128)
		{
			cptr->PrevPhase = cptr->Phase;
			cptr->PrevPFTime = cptr->FTime;
			cptr->PPMorphTime = 0;
		}

		cptr->FTime = 0;
		cptr->Phase = DinoInfo[cptr->CType].dieAnim;
		ActivateCharacterFx(cptr);
	}
	else
	{
		ProcessPrevPhase(cptr);

		cptr->FTime += TimeDt;
		if (cptr->FTime >= cptr->pinfo->Animation[cptr->Phase].AniTime)
			cptr->FTime = cptr->pinfo->Animation[cptr->Phase].AniTime - 1;
	}

	//======= movement ===========//
	DeltaFunc(cptr->vspeed, 0, TimeDt / 800.f);
	cptr->pos.x += cptr->lookx * cptr->vspeed * TimeDt;
	cptr->pos.z += cptr->lookz * cptr->vspeed * TimeDt;

	ThinkY_Beta_Gamma(cptr, 200, 196, 0.6f, 0.5f);
	DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 1600.f);
}



void AnimateDimorDead(TCharacter *cptr)
{

	if (cptr->Phase != DinoInfo[cptr->CType].fallAnim && cptr->Phase != DinoInfo[cptr->CType].dieAnim)
	{
		if (cptr->PPMorphTime > 128)
		{
			cptr->PrevPhase = cptr->Phase;
			cptr->PrevPFTime = cptr->FTime;
			cptr->PPMorphTime = 0;
		}

		cptr->FTime = 0;
		cptr->Phase = DinoInfo[cptr->CType].fallAnim;
		cptr->rspeed = 0;
		ActivateCharacterFx(cptr);
		return;
	}

	ProcessPrevPhase(cptr);

	cptr->FTime += TimeDt;
	if (cptr->FTime >= cptr->pinfo->Animation[cptr->Phase].AniTime)
		if (cptr->Phase == DinoInfo[cptr->CType].dieAnim)
			cptr->FTime = cptr->pinfo->Animation[cptr->Phase].AniTime - 1;
		else
			cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;


	//======= movement ===========//
	if (cptr->Phase == DinoInfo[cptr->CType].dieAnim)
		DeltaFunc(cptr->vspeed, 0, TimeDt / 400.f);
	else
		DeltaFunc(cptr->vspeed, 0, TimeDt / 1200.f);

	cptr->pos.x += cptr->lookx * cptr->vspeed * TimeDt;
	cptr->pos.z += cptr->lookz * cptr->vspeed * TimeDt;

	if (cptr->Phase == DinoInfo[cptr->CType].fallAnim)
	{
		float wh = GetLandUpH(cptr->pos.x, cptr->pos.z);
		float lh = GetLandH(cptr->pos.x, cptr->pos.z);
		BOOL OnWater = (wh > lh);
		if (OnWater)
			if (cptr->pos.y >= wh && (cptr->pos.y + cptr->rspeed * TimeDt / 1024) < wh)
			{
				AddWCircle(cptr->pos.x + siRand(128), cptr->pos.z + siRand(128), 2.0);
				AddWCircle(cptr->pos.x + siRand(128), cptr->pos.z + siRand(128), 2.5);
				AddWCircle(cptr->pos.x + siRand(128), cptr->pos.z + siRand(128), 3.0);
				AddWCircle(cptr->pos.x + siRand(128), cptr->pos.z + siRand(128), 3.5);
				AddWCircle(cptr->pos.x + siRand(128), cptr->pos.z + siRand(128), 3.0);
			}
		cptr->pos.y += cptr->rspeed * TimeDt / 1024;
		cptr->rspeed -= TimeDt * 2.56;

		if (cptr->pos.y < lh)
		{
			cptr->pos.y = lh;
			if (OnWater)
			{
				AddElements(cptr->pos.x + siRand(128), lh, cptr->pos.z + siRand(128), 4, 10);
				AddElements(cptr->pos.x + siRand(128), lh, cptr->pos.z + siRand(128), 4, 10);
				AddElements(cptr->pos.x + siRand(128), lh, cptr->pos.z + siRand(128), 4, 10);
			}

			if (cptr->PPMorphTime > 128)
			{
				cptr->PrevPhase = cptr->Phase;
				cptr->PrevPFTime = cptr->FTime;
				cptr->PPMorphTime = 0;
			}

			cptr->Phase = DinoInfo[cptr->CType].dieAnim;
			cptr->FTime = 0;
			ActivateCharacterFx(cptr);
		}
	}
	else
	{
		ThinkY_Beta_Gamma(cptr, 140, 126, 0.6f, 0.5f);
		DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 1600.f);
	}
}



//universal animate dead proc
void AnimateDeadCommon(TCharacter *cptr)
{

	if (cptr->Phase != DinoInfo[cptr->CType].dieAnim && cptr->Phase != DinoInfo[cptr->CType].sleepAnim)
	{
		if (cptr->PPMorphTime > 128)
		{
			cptr->PrevPhase = cptr->Phase;
			cptr->PrevPFTime = cptr->FTime;
			cptr->PPMorphTime = 0;
		}

		cptr->FTime = 0;
		cptr->Phase = DinoInfo[cptr->CType].dieAnim;
		ActivateCharacterFx(cptr);
	}
	else
	{
		ProcessPrevPhase(cptr);

		cptr->FTime += TimeDt;
		if (cptr->FTime >= cptr->pinfo->Animation[cptr->Phase].AniTime)
			if (Tranq)
			{
				cptr->FTime = 0;
				cptr->Phase = DinoInfo[cptr->CType].sleepAnim;
				ActivateCharacterFx(cptr);
			}
			else
				cptr->FTime = cptr->pinfo->Animation[cptr->Phase].AniTime - 1;
	}

	//======= movement ===========//
	DeltaFunc(cptr->vspeed, 0, TimeDt / 800.f);
	cptr->pos.x += cptr->lookx * cptr->vspeed * TimeDt;
	cptr->pos.z += cptr->lookz * cptr->vspeed * TimeDt;

	ThinkY_Beta_Gamma(cptr, 100, 96, 0.6f, 0.5f);

	DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 1600.f);
}





void AnimateCarnivore(TCharacter *cptr)
{
	NewPhase = FALSE;
	int _Phase = cptr->Phase;
	int _FTime = cptr->FTime;
	float _tgalpha = cptr->tgalpha;

TBEGIN:
	float targetx = cptr->tgx;
	float targetz = cptr->tgz;
	float targetdx = targetx - cptr->pos.x;
	float targetdz = targetz - cptr->pos.z;

	float tdist = (float)sqrt(targetdx * targetdx + targetdz * targetdz);

	float playerdx = PlayerX - cptr->pos.x - cptr->lookx * 108;
	float playerdz = PlayerZ - cptr->pos.z - cptr->lookz * 108;

	if (cptr->Clone == AI_ALLO) {
		playerdx = PlayerX - cptr->pos.x - cptr->lookx * 100 * cptr->scale;
		playerdz = PlayerZ - cptr->pos.z - cptr->lookz * 100 * cptr->scale;
	}

	float pdist = (float)sqrt(playerdx * playerdx + playerdz * playerdz);
	if (cptr->State == 2)
	{
		if (AIInfo[cptr->Clone].jumper) {
			if (cptr->Phase != DinoInfo[cptr->CType].jumpAnim) NewPhase = TRUE;
		}
		cptr->State = 1;
		if (cptr->Clone == AI_SPINO || cptr->Clone == AI_CERAT) cptr->Phase = DinoInfo[cptr->CType].runAnim;
	}


	if (GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z) > DinoInfo[cptr->CType].waterLevel * cptr->scale)
		cptr->StateF |= csONWATER;
	else
		cptr->StateF &= (!csONWATER);

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) goto NOTHINK;

	//============================================//
	if (!MyHealth) cptr->State = 0;
	if (cptr->State)
	{
		if (pdist > ctViewR * DinoInfo[cptr->CType].aggress + OptAgres / AIInfo[cptr->Clone].agressMulti || DinoInfo[cptr->CType].aggress <= 0)
		{
			nv.x = playerdx;
			nv.z = playerdz;
			nv.y = 0;
			NormVector(nv, 2048.f);
			cptr->tgx = cptr->pos.x - nv.x;
			cptr->tgz = cptr->pos.z - nv.z;
			cptr->tgtime = 0;
			cptr->AfraidTime -= TimeDt;
			if (cptr->AfraidTime <= 0)
			{
				cptr->AfraidTime = 0;
				cptr->State = 0;
			}

		}
		else
		{
			cptr->tgx = PlayerX;
			cptr->tgz = PlayerZ;
			cptr->tgtime = 0;
		}

		if (AIInfo[cptr->Clone].jumper) {
			if (!(cptr->StateF & csONWATER))
				if (pdist < 1324 * cptr->scale && pdist>900 * cptr->scale)
					if (AngleDifference(cptr->alpha, FindVectorAlpha(playerdx, playerdz)) < 0.2f)
						cptr->Phase = DinoInfo[cptr->CType].jumpAnim;
		}


		if (pdist < DinoInfo[cptr->CType].killDist && DinoInfo[cptr->CType].killDist > 0) {
			int killAlt = DinoInfo[cptr->CType].waterLevel;
			if (killAlt < 256) killAlt = 256;
			if (fabs(PlayerY - cptr->pos.y) < killAlt + 20)
			{

				if (DinoInfo[cptr->CType].killTypeCount > 0) {

					if (!(cptr->StateF & csONWATER))
					{
						cptr->vspeed /= 8.0f;
						cptr->State = 1;
						cptr->Phase = DinoInfo[cptr->CType].killType[cptr->killType].anim;
						AddDeadBody(cptr, DinoInfo[cptr->CType].killType[cptr->killType].hunteranim);
					}
					else AddDeadBody(cptr, HUNT_EAT);

				} else {
					AddDeadBody(cptr, HUNT_EAT);
					cptr->State = 0;
				}

			}
		}


	}

	if (!cptr->State)
	{
		if (cptr->Clone == AI_VELO || cptr->Clone == AI_CERAT) cptr->AfraidTime = 0;

		if (tdist < 456)
		{
			SetNewTargetPlace(cptr, 8048.f);
			goto TBEGIN;
		}
	}

NOTHINK:
	if (pdist < 2048) cptr->NoFindCnt = 0;
	if (cptr->NoFindCnt) cptr->NoFindCnt--;
	else
	{
		cptr->tgalpha = CorrectedAlpha(FindVectorAlpha(targetdx, targetdz), cptr->alpha);//FindVectorAlpha(targetdx, targetdz);
		if (cptr->State && pdist > 1648)
		{
			cptr->tgalpha += (float)sin(RealTime / 824.f) / AIInfo[cptr->Clone].tGAIncrement;
			if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
			if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;
		}
	}

	LookForAWay(cptr, !DinoInfo[cptr->CType].canSwim, TRUE);
	if (cptr->NoWayCnt > 12)
	{
		cptr->NoWayCnt = 0;
		cptr->NoFindCnt = 16 + rRand(20);
	}


	if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
	if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;

	//===============================================//

	ProcessPrevPhase(cptr);


	//======== select new phase =======================//
	cptr->FTime += TimeDt;

	if (cptr->FTime >= cptr->pinfo->Animation[cptr->Phase].AniTime)
	{
		cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;
		NewPhase = TRUE;
	}

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount)  goto ENDPSELECT;

	if (AIInfo[cptr->Clone].jumper) {
		if (NewPhase && _Phase == DinoInfo[cptr->CType].jumpAnim)
		{
			cptr->Phase = DinoInfo[cptr->CType].runAnim;
			goto ENDPSELECT;
		}

		if (cptr->Phase == DinoInfo[cptr->CType].jumpAnim) goto ENDPSELECT;
	}

	if (NewPhase)
		if (!cptr->State)
		{




			if (DinoInfo[cptr->CType].idleCount) {

				if (rRand(AIInfo[cptr->Clone].idleStartD) > 110) {
					cptr->Phase = DinoInfo[cptr->CType].idleAnim[rRand(DinoInfo[cptr->CType].idleCount - 1)];
					goto ENDPSELECT;
				}
				else {
					cptr->Phase = DinoInfo[cptr->CType].walkAnim;
				}

			}
			else {
				cptr->Phase = DinoInfo[cptr->CType].walkAnim;
			}

		}
		else cptr->Phase = DinoInfo[cptr->CType].runAnim;

	bool idlePhase = false;
	for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
		if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) idlePhase = true;
	}

	if (!idlePhase)
		if (!cptr->State) cptr->Phase = DinoInfo[cptr->CType].walkAnim;
		else if (fabs(cptr->tgalpha - cptr->alpha) < 1.0 ||
			fabs(cptr->tgalpha - cptr->alpha) > 2 * pi - 1.0)
			cptr->Phase = DinoInfo[cptr->CType].runAnim;
		else cptr->Phase = DinoInfo[cptr->CType].walkAnim;

	if (DinoInfo[cptr->CType].canSwim) {
		if (cptr->StateF & csONWATER) cptr->Phase = DinoInfo[cptr->CType].swimAnim;
	}

	if (cptr->Clone != AI_CERAT) {
		if (cptr->Slide > 40) cptr->Phase = DinoInfo[cptr->CType].slideAnim;
	}


ENDPSELECT:

	//====== process phase changing ===========//
	if ((_Phase != cptr->Phase) || NewPhase)
		ActivateCharacterFx(cptr);

	if (_Phase != cptr->Phase)
	{
		//==== set proportional FTime for better morphing =//

		if (MORPHP) {
			if ((_Phase == DinoInfo[cptr->CType].runAnim ||
				_Phase == DinoInfo[cptr->CType].walkAnim) &&
				(cptr->Phase == DinoInfo[cptr->CType].runAnim ||
					cptr->Phase == DinoInfo[cptr->CType].walkAnim))
				cptr->FTime = _FTime * cptr->pinfo->Animation[cptr->Phase].AniTime / cptr->pinfo->Animation[_Phase].AniTime + 64;
			else if (!NewPhase) cptr->FTime = 0;
		}

		if (cptr->PPMorphTime > 128)
		{
			cptr->PrevPhase = _Phase;
			cptr->PrevPFTime = _FTime;
			cptr->PPMorphTime = 0;
		}
	}

	cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;



	//========== rotation to tgalpha ===================//

	float rspd, currspeed, tgbend;
	float dalpha = (float)fabs(cptr->tgalpha - cptr->alpha);
	float drspd = dalpha;
	if (drspd > pi) drspd = 2 * pi - drspd;

	if (AIInfo[cptr->Clone].jumper) {
		if (cptr->Phase == DinoInfo[cptr->CType].jumpAnim) goto SKIPROT;
	}
	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) goto SKIPROT;
	for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
		if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) goto SKIPROT;
	}

	if (drspd > 0.02)
		if (cptr->tgalpha > cptr->alpha) currspeed = 0.6f + drspd * 1.2f;
		else currspeed = -0.6f - drspd * 1.2f;
	else currspeed = 0;
	if (cptr->AfraidTime) currspeed *= 2.5;

	if (dalpha > pi) currspeed *= -1;
	if ((cptr->StateF & csONWATER) || cptr->Phase == DinoInfo[cptr->CType].walkAnim) currspeed /= 1.4f;

	if (cptr->AfraidTime) DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 160.f);
	else DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 180.f);

	tgbend = drspd / AIInfo[cptr->Clone].targetBendRotSpd;
	if (tgbend > pi / 5) tgbend = pi / 5;

	tgbend *= SGN(currspeed);
	if (fabs(tgbend) > fabs(cptr->bend)) DeltaFunc(cptr->bend, tgbend, (float)TimeDt / 800.f);
	else DeltaFunc(cptr->bend, tgbend, (float)TimeDt / 600.f);


	rspd = cptr->rspeed * TimeDt / 1024.f;
	if (drspd < fabs(rspd)) cptr->alpha = cptr->tgalpha;
	else cptr->alpha += rspd;


	if (cptr->alpha > pi * 2) cptr->alpha -= pi * 2;
	if (cptr->alpha < 0) cptr->alpha += pi * 2;

SKIPROT:

	if (cptr->Clone != AI_CERAT) {
		//======= set slide mode ===========//
		if (!cptr->Slide && cptr->vspeed > 0.6 && (cptr->Phase != DinoInfo[cptr->CType].jumpAnim || !AIInfo[cptr->Clone].jumper))
			if (AngleDifference(cptr->tgalpha, cptr->alpha) > pi * 2 / 3.f)
			{
				cptr->Slide = (int)(cptr->vspeed*700.f);
				cptr->slidex = cptr->lookx;
				cptr->slidez = cptr->lookz;
				cptr->vspeed = 0;
			}
	}

	//========== movement ==============================//
	cptr->lookx = (float)cos(cptr->alpha);
	cptr->lookz = (float)sin(cptr->alpha);

	float curspeed = 0;
	if (cptr->Phase == DinoInfo[cptr->CType].runAnim) curspeed = cptr->speed_run;
	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) curspeed = cptr->speed_walk;
	if (DinoInfo[cptr->CType].canSwim) {
		if (cptr->Phase == DinoInfo[cptr->CType].swimAnim) curspeed = cptr->speed_swim;
	}
	if (AIInfo[cptr->Clone].jumper) {
		if (cptr->Phase == DinoInfo[cptr->CType].jumpAnim) curspeed = cptr->speed_jump;
	}

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) curspeed = 0.0f;

	if (cptr->Phase == DinoInfo[cptr->CType].runAnim && cptr->Slide && cptr->Clone != AI_CERAT)
	{
		curspeed /= 8;
		if (drspd > pi / 2.f) curspeed = 0;
		else if (drspd > pi / 4.f) curspeed *= 2.f - 4.f*drspd / pi;
	}
	else if (drspd > pi / 2.f) curspeed *= 2.f - 2.f*drspd / pi;

	//========== process speed =============//

	DeltaFunc(cptr->vspeed, curspeed, TimeDt / 500.f);

	if (AIInfo[cptr->Clone].jumper) {
		if (cptr->Phase == DinoInfo[cptr->CType].jumpAnim) cptr->vspeed = 1.1f;
	}

	MoveCharacter(cptr, cptr->lookx * cptr->vspeed * TimeDt * cptr->scale,
		cptr->lookz * cptr->vspeed * TimeDt * cptr->scale, !DinoInfo[cptr->CType].canSwim, TRUE);

	if (cptr->Clone != AI_CERAT) {
		//========== slide ==============//
		if (cptr->Slide)
		{
			MoveCharacter(cptr, cptr->slidex * cptr->Slide / 600.f * TimeDt * cptr->scale,
				cptr->slidez * cptr->Slide / 600.f * TimeDt * cptr->scale, !DinoInfo[cptr->CType].canSwim, TRUE);

			cptr->Slide -= TimeDt;
			if (cptr->Slide < 0) cptr->Slide = 0;
		}
	}


	//============ Y movement =================//
	if (cptr->StateF & csONWATER && DinoInfo[cptr->CType].canSwim)
	{
		cptr->pos.y = GetLandUpH(cptr->pos.x, cptr->pos.z) - (DinoInfo[cptr->CType].waterLevel + 20) * cptr->scale;
		cptr->beta /= 2;
		cptr->tggamma = 0;
	}
	else
	{
		ThinkY_Beta_Gamma(cptr,
			AIInfo[cptr->Clone].yBetaGamma1,
			AIInfo[cptr->Clone].yBetaGamma2,
			AIInfo[cptr->Clone].yBetaGamma3,
			AIInfo[cptr->Clone].yBetaGamma4);
	}

	//=== process to tggamma ===//
	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) cptr->tggamma += cptr->rspeed / AIInfo[cptr->Clone].walkTargetGammaRot;
	else cptr->tggamma += cptr->rspeed / AIInfo[cptr->Clone].targetGammaRot;
	if (AIInfo[cptr->Clone].jumper) {
		if (cptr->Phase == DinoInfo[cptr->CType].jumpAnim) cptr->tggamma = 0;
	}

	DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 1624.f);


	//==================================================//

}


//universal animate proc
void AnimateHerbivore(TCharacter *cptr)
{
	NewPhase = FALSE;
	int _Phase = cptr->Phase;
	int _FTime = cptr->FTime;
	float _tgalpha = cptr->tgalpha;
	if (cptr->AfraidTime) cptr->AfraidTime = MAX(0, cptr->AfraidTime - TimeDt);
	if (cptr->State == 2)
	{
		NewPhase = TRUE;
		cptr->State = 1;
	}

TBEGIN:
	float targetx = cptr->tgx;
	float targetz = cptr->tgz;
	float targetdx = targetx - cptr->pos.x;
	float targetdz = targetz - cptr->pos.z;

	float tdist = (float)sqrt(targetdx * targetdx + targetdz * targetdz);

	float playerdx = PlayerX - cptr->pos.x;
	float playerdz = PlayerZ - cptr->pos.z;

	if (cptr->Clone == AI_CHASM) {
		playerdx = PlayerX - cptr->pos.x - cptr->lookx * 300 * cptr->scale;
		playerdz = PlayerZ - cptr->pos.z - cptr->lookz * 300 * cptr->scale;
	}

	float pdist = (float)sqrt(playerdx * playerdx + playerdz * playerdz);


	if (GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z) > DinoInfo[cptr->CType].waterLevel * cptr->scale)
		cptr->StateF |= csONWATER;
	else
		cptr->StateF &= (!csONWATER);

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) goto NOTHINK;

	//=========== run away =================//

	if (cptr->State)
	{

		if (pdist < 6000) cptr->AfraidTime = 8000;

		if (!cptr->AfraidTime)
		{
			cptr->State = 0;
			SetNewTargetPlace(cptr, AIInfo[cptr->Clone].targetDistance);
			goto TBEGIN;
		}

		if (pdist > 128 * DinoInfo[cptr->CType].aggress + OptAgres / 8 || DinoInfo[cptr->CType].aggress <= 0)
		{
			nv.x = playerdx;
			nv.z = playerdz;
			nv.y = 0;
			NormVector(nv, 2048.f);
			cptr->tgx = cptr->pos.x - nv.x;
			cptr->tgz = cptr->pos.z - nv.z;
			cptr->tgtime = 0;
		}
		else
		{
			cptr->tgx = PlayerX;
			cptr->tgz = PlayerZ;
			cptr->tgtime = 0;
		}



		if (MyHealth) {
			if (pdist < DinoInfo[cptr->CType].killDist && DinoInfo[cptr->CType].killDist > 0)
			{
				int killAlt = DinoInfo[cptr->CType].waterLevel;
				if (killAlt < 256) killAlt = 256;
				if (fabs(PlayerY - cptr->pos.y) < killAlt + 20)
				{
					if (DinoInfo[cptr->CType].killTypeCount > 0) {

						if (!(cptr->StateF & csONWATER))
						{
							cptr->vspeed /= 8.0f;
							cptr->State = 1;
							cptr->Phase = DinoInfo[cptr->CType].killType[cptr->killType].anim;
							AddDeadBody(cptr, DinoInfo[cptr->CType].killType[cptr->killType].hunteranim);
						}
						else AddDeadBody(cptr, HUNT_EAT);

					}
					else {
						AddDeadBody(cptr, HUNT_EAT);
						cptr->State = 0;
					}
				}
			}
		}


	}


	//======== exploring area ===============//
	if (!cptr->State)
	{
		cptr->AfraidTime = 0;

		if (tdist < 456)
		{
			SetNewTargetPlace(cptr, 8048.f);
			goto TBEGIN;
		}
	}


	//============================================//

NOTHINK:

	if (cptr->NoFindCnt) cptr->NoFindCnt--;
	else
	{
		cptr->tgalpha = CorrectedAlpha(FindVectorAlpha(targetdx, targetdz), cptr->alpha);//FindVectorAlpha(targetdx, targetdz);
		if (cptr->AfraidTime)
		{
			cptr->tgalpha += (float)sin(RealTime / 1024.f) / 3.f;
			if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
			if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;
		}
	}


	LookForAWay(cptr, !DinoInfo[cptr->CType].canSwim, TRUE);
	if (cptr->NoWayCnt > AIInfo[cptr->Clone].noWayCntMin)
	{
		cptr->NoWayCnt = 0;
		cptr->NoFindCnt = AIInfo[cptr->Clone].noFindWayMed + rRand(AIInfo[cptr->Clone].noFindWayRange);
	}

	if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
	if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;

	//===============================================//

	ProcessPrevPhase(cptr);

	//======== select new phase =======================//
	cptr->FTime += TimeDt;

	if (cptr->FTime >= cptr->pinfo->Animation[cptr->Phase].AniTime)
	{
		cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;
		NewPhase = TRUE;
	}

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount)  goto ENDPSELECT;


	if (NewPhase)
		if (!cptr->State)
		{
			if (DinoInfo[cptr->CType].idleCount) {

				bool idlePhase = false;
				for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
					if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) idlePhase = true;
				}

				if (idlePhase) {
					if (rRand(128) > 64 && cptr->Phase == DinoInfo[cptr->CType].idleAnim[DinoInfo[cptr->CType].idleCount - 1])
						cptr->Phase = DinoInfo[cptr->CType].walkAnim;
					else cptr->Phase = DinoInfo[cptr->CType].idleAnim[rRand(DinoInfo[cptr->CType].idleCount - 1)];
					goto ENDPSELECT;
				}
				if (rRand(128) > AIInfo[cptr->Clone].idleStart) cptr->Phase = DinoInfo[cptr->CType].idleAnim[0];
				else cptr->Phase = DinoInfo[cptr->CType].walkAnim;

			}
			else {
				cptr->Phase = DinoInfo[cptr->CType].walkAnim;
			}

		}
		else if (cptr->AfraidTime) cptr->Phase = DinoInfo[cptr->CType].runAnim;
		else cptr->Phase = DinoInfo[cptr->CType].walkAnim;

	if (DinoInfo[cptr->CType].canSwim) {
		if (cptr->StateF & csONWATER) cptr->Phase = DinoInfo[cptr->CType].swimAnim;
	}

ENDPSELECT:

	//====== process phase changing ===========//
	if ((_Phase != cptr->Phase) || NewPhase)
		ActivateCharacterFx(cptr);

	if (_Phase != cptr->Phase)
	{
		if ((_Phase == DinoInfo[cptr->CType].runAnim ||
			_Phase == DinoInfo[cptr->CType].walkAnim) &&
			(cptr->Phase == DinoInfo[cptr->CType].runAnim ||
				cptr->Phase == DinoInfo[cptr->CType].walkAnim))
			cptr->FTime = _FTime * cptr->pinfo->Animation[cptr->Phase].AniTime / cptr->pinfo->Animation[_Phase].AniTime + 64;
		else if (!NewPhase) cptr->FTime = 0;

		if (cptr->PPMorphTime > 128)
		{
			cptr->PrevPhase = _Phase;
			cptr->PrevPFTime = _FTime;
			cptr->PPMorphTime = 0;
		}
	}

	cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;



	//========== rotation to tgalpha ===================//

	float rspd, currspeed, tgbend;
	float dalpha = (float)fabs(cptr->tgalpha - cptr->alpha);
	float drspd = dalpha;
	if (drspd > pi) drspd = 2 * pi - drspd;

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) goto SKIPROT;


	for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
		if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) goto SKIPROT;
	}

	if (drspd > 0.02)
		if (cptr->tgalpha > cptr->alpha) currspeed = 0.2f + drspd * 1.0f;
		else currspeed = -0.2f - drspd * 1.0f;
	else currspeed = 0;

	if (cptr->AfraidTime) currspeed *= 1.5;
	if (dalpha > pi) currspeed *= -1;
	if ((cptr->State & csONWATER) || cptr->Phase == DinoInfo[cptr->CType].walkAnim) currspeed /= 1.4f;

	DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 400.f);

	tgbend = drspd / AIInfo[cptr->Clone].targetBendRotSpd;
	if (tgbend > pi / AIInfo[cptr->Clone].targetBendMin) tgbend = pi / AIInfo[cptr->Clone].targetBendMin;

	tgbend *= SGN(currspeed);
	if (fabs(tgbend) > fabs(cptr->bend)) DeltaFunc(cptr->bend, tgbend, (float)TimeDt / AIInfo[cptr->Clone].targetBendDelta1);
	else DeltaFunc(cptr->bend, tgbend, (float)TimeDt / AIInfo[cptr->Clone].targetBendDelta2);


	rspd = cptr->rspeed * TimeDt / 612.f;
	if (drspd < fabs(rspd)) cptr->alpha = cptr->tgalpha;
	else cptr->alpha += rspd;


	if (cptr->alpha > pi * 2) cptr->alpha -= pi * 2;
	if (cptr->alpha < 0) cptr->alpha += pi * 2;

SKIPROT:

	//========== movement ==============================//
	cptr->lookx = (float)cos(cptr->alpha);
	cptr->lookz = (float)sin(cptr->alpha);

	float curspeed = 0;
	if (cptr->Phase == DinoInfo[cptr->CType].runAnim) curspeed = cptr->speed_run;
	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) curspeed = cptr->speed_walk;

	if (DinoInfo[cptr->CType].canSwim) {
		if (cptr->Phase == DinoInfo[cptr->CType].swimAnim) curspeed = cptr->speed_swim;
	}

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) curspeed = 0.0f;


	if (drspd > pi / 2.f) curspeed *= 2.f - 2.f*drspd / pi;

	//========== process speed =============//
	curspeed *= cptr->scale;
	if (curspeed > cptr->vspeed) DeltaFunc(cptr->vspeed, curspeed, TimeDt / 1024.f);
	else DeltaFunc(cptr->vspeed, curspeed, TimeDt / 256.f);

	MoveCharacter(cptr, cptr->lookx * cptr->vspeed * TimeDt,
		cptr->lookz * cptr->vspeed * TimeDt, !DinoInfo[cptr->CType].canSwim, TRUE);


	//============ Y movement =================//
	if (cptr->StateF & csONWATER && DinoInfo[cptr->CType].canSwim)
	{
		cptr->pos.y = GetLandUpH(cptr->pos.x, cptr->pos.z) - (DinoInfo[cptr->CType].waterLevel + 20) * cptr->scale;
		cptr->beta /= 2;
		cptr->tggamma = 0;
	}
	else {
		ThinkY_Beta_Gamma(cptr, 128, 64, 0.6f, AIInfo[cptr->Clone].yBetaGamma4);
	}

	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) cptr->tggamma += cptr->rspeed / AIInfo[cptr->Clone].walkTargetGammaRot;
	else cptr->tggamma += cptr->rspeed / AIInfo[cptr->Clone].targetGammaRot;
	DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 2048.f);
}



/*
void AnimateVeloPackOld(TCharacter *cptr)
{
	NewPhase = FALSE;
	int _Phase = cptr->Phase;
	int _FTime = cptr->FTime;
	float _tgalpha = cptr->tgalpha;


TBEGIN:
	float targetx = cptr->tgx;
	float targetz = cptr->tgz;
	float targetdx = targetx - cptr->pos.x;
	float targetdz = targetz - cptr->pos.z;

	float tdist = (float)sqrt(targetdx * targetdx + targetdz * targetdz);

	float playerdx = PlayerX - cptr->pos.x - cptr->lookx * 108;
	float playerdz = PlayerZ - cptr->pos.z - cptr->lookz * 108;
	float pdist = (float)sqrt(playerdx * playerdx + playerdz * playerdz);
	if (cptr->State == 2)
	{
		if (cptr->Phase != VEL_JUMP) NewPhase = TRUE;
		cptr->State = 1;
	}


	if (GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z) > 140 * cptr->scale)
		cptr->StateF |= csONWATER;
	else
		cptr->StateF &= (!csONWATER);

	if (cptr->Phase == VEL_EAT) goto NOTHINK;

	//============================================//
	if (!MyHealth) cptr->State = 0;

	bool packAttack = false;
	if (cptr->pack >= 0) {
		if (Packs[cptr->pack].attack) packAttack = true;
	}
	bool indAttack = pdist > ctViewR * DinoInfo[cptr->CType].aggress + OptAgres / 8 || DinoInfo[cptr->CType].aggress <= 0;

	if (cptr->State)
	{
		if (cptr->AfraidTime > 0) {
			Packs[cptr->pack]._alert = true;
		}

		if (indAttack && !packAttack)
		{
			nv.x = playerdx;
			nv.z = playerdz;
			nv.y = 0;
			NormVector(nv, 2048.f);
			cptr->tgx = cptr->pos.x - nv.x;
			cptr->tgz = cptr->pos.z - nv.z;
			cptr->tgtime = 0;
			cptr->AfraidTime -= TimeDt;

			bool packAlert = false;
			if (cptr->pack >= 0) {
				if (Packs[cptr->pack].alert == true) packAlert = true;
			}

			if (cptr->AfraidTime <= 0 && !packAlert)
			{
				cptr->AfraidTime = 0;
				cptr->State = 0;
			}
		}
		else
		{
			cptr->tgx = PlayerX;
			cptr->tgz = PlayerZ;
			cptr->tgtime = 0;

			if (indAttack) Packs[cptr->pack]._attack == true;

		}

		if (!(cptr->StateF & csONWATER))
			if (pdist < 1324 * cptr->scale && pdist>900 * cptr->scale)
				if (AngleDifference(cptr->alpha, FindVectorAlpha(playerdx, playerdz)) < 0.2f)
					cptr->Phase = VEL_JUMP;

		if (pdist < DinoInfo[cptr->CType].killDist && DinoInfo[cptr->CType].killDist > 0)
			if (fabs(PlayerY - cptr->pos.y - 120) < 256)
			{
				if (!(cptr->StateF & csONWATER))
				{
					cptr->vspeed /= 8.0f;
					cptr->State = 1;
					cptr->Phase = VEL_EAT;
				}

				AddDeadBody(cptr, DinoInfo[cptr->CType].hunterDeathAnim);
			}
	}

	if (!cptr->State)
	{

		bool skipPack = true;

		if (cptr->pack >= 0) {
			if (Packs[cptr->pack].leader != cptr) skipPack = false;
		}

		if (!skipPack) {

			if (Packs[cptr->pack].alert)
			{
				cptr->State = 2;
				goto TBEGIN;
			}

			cptr->AfraidTime = 0;

			float leaderdx = Packs[cptr->pack].leader->pos.x - cptr->pos.x;
			float leaderdz = Packs[cptr->pack].leader->pos.z - cptr->pos.z;
			float leaderdist = (float)sqrt(leaderdx * leaderdx + leaderdz * leaderdz);

			if (leaderdist > DinoInfo[cptr->CType].packDensity * 1.25)
			{
				cptr->followLeader = true;
			}

			if (cptr->followLeader && leaderdist < DinoInfo[cptr->CType].packDensity * 0.75)
			{
				cptr->followLeader = false;
			}

			if (cptr->followLeader)
			{
				cptr->tgx = Packs[cptr->pack].leader->pos.x;
				cptr->tgz = Packs[cptr->pack].leader->pos.z;
			}
			else
			{

				if (tdist < 456) {
					SetNewTargetPlace(cptr, 8048.f);
					goto TBEGIN;
				}

			}

		} else {
			cptr->AfraidTime = 0;
			if (tdist < 456)
			{
				SetNewTargetPlace(cptr, 8048.f);
				goto TBEGIN;
			}
		}

	}

NOTHINK:
	if (pdist < 2048) cptr->NoFindCnt = 0;
	if (cptr->NoFindCnt) cptr->NoFindCnt--;
	else
	{
		cptr->tgalpha = CorrectedAlpha(FindVectorAlpha(targetdx, targetdz), cptr->alpha);//FindVectorAlpha(targetdx, targetdz);
		if (cptr->State && pdist > 1648)
		{
			cptr->tgalpha += (float)sin(RealTime / 824.f) / 2.f;
			if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
			if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;
		}
	}

	LookForAWay(cptr, FALSE, TRUE);
	if (cptr->NoWayCnt > 12)
	{
		cptr->NoWayCnt = 0;
		cptr->NoFindCnt = 16 + rRand(20);
	}


	if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
	if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;

	//===============================================//

	ProcessPrevPhase(cptr);


	//======== select new phase =======================//
	cptr->FTime += TimeDt;

	if (cptr->FTime >= cptr->pinfo->Animation[cptr->Phase].AniTime)
	{
		cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;
		NewPhase = TRUE;
	}

	if (cptr->Phase == VEL_EAT)  goto ENDPSELECT;
	if (NewPhase && _Phase == VEL_JUMP)
	{
		cptr->Phase = VEL_RUN;
		goto ENDPSELECT;
	}


	if (cptr->Phase == VEL_JUMP) goto ENDPSELECT;

	if (NewPhase)
		if (!cptr->State)
			if (rRand(118) > 110)
			{
				cptr->Phase = VEL_IDLE1 + rRand(1);
				goto ENDPSELECT;
			}
			else cptr->Phase = VEL_WALK;
		else cptr->Phase = VEL_RUN;
	if (cptr->Phase != VEL_IDLE1 && cptr->Phase != VEL_IDLE2)
		if (!cptr->State) cptr->Phase = VEL_WALK;
		else if (fabs(cptr->tgalpha - cptr->alpha) < 1.0 ||
			fabs(cptr->tgalpha - cptr->alpha) > 2 * pi - 1.0)
			cptr->Phase = VEL_RUN;
		else cptr->Phase = VEL_WALK;

	if (cptr->StateF & csONWATER) cptr->Phase = VEL_SWIM;
	if (cptr->Slide > 40) cptr->Phase = VEL_SLIDE;


ENDPSELECT:

	//====== process phase changing ===========//
	if ((_Phase != cptr->Phase) || NewPhase)
		ActivateCharacterFx(cptr);

	if (_Phase != cptr->Phase)
	{
		//==== set proportional FTime for better morphing =//
		if (MORPHP)
			if (_Phase <= 3 && cptr->Phase <= 3)
				cptr->FTime = _FTime * cptr->pinfo->Animation[cptr->Phase].AniTime / cptr->pinfo->Animation[_Phase].AniTime + 64;
			else if (!NewPhase) cptr->FTime = 0;

		if (cptr->PPMorphTime > 128)
		{
			cptr->PrevPhase = _Phase;
			cptr->PrevPFTime = _FTime;
			cptr->PPMorphTime = 0;
		}
	}

	cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;



	//========== rotation to tgalpha ===================//

	float rspd, currspeed, tgbend;
	float dalpha = (float)fabs(cptr->tgalpha - cptr->alpha);
	float drspd = dalpha;
	if (drspd > pi) drspd = 2 * pi - drspd;

	if (cptr->Phase == VEL_JUMP || cptr->Phase == VEL_EAT ||
		cptr->Phase == VEL_IDLE1 || cptr->Phase == VEL_IDLE2) goto SKIPROT;

	if (drspd > 0.02)
		if (cptr->tgalpha > cptr->alpha) currspeed = 0.6f + drspd * 1.2f;
		else currspeed = -0.6f - drspd * 1.2f;
	else currspeed = 0;
	if (cptr->AfraidTime) currspeed *= 2.5;

	if (dalpha > pi) currspeed *= -1;
	if ((cptr->StateF & csONWATER) || cptr->Phase == VEL_WALK) currspeed /= 1.4f;

	if (cptr->AfraidTime) DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 160.f);
	else DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 180.f);

	tgbend = drspd / 3;
	if (tgbend > pi / 5) tgbend = pi / 5;

	tgbend *= SGN(currspeed);
	if (fabs(tgbend) > fabs(cptr->bend)) DeltaFunc(cptr->bend, tgbend, (float)TimeDt / 800.f);
	else DeltaFunc(cptr->bend, tgbend, (float)TimeDt / 600.f);


	rspd = cptr->rspeed * TimeDt / 1024.f;
	if (drspd < fabs(rspd)) cptr->alpha = cptr->tgalpha;
	else cptr->alpha += rspd;


	if (cptr->alpha > pi * 2) cptr->alpha -= pi * 2;
	if (cptr->alpha < 0) cptr->alpha += pi * 2;

SKIPROT:

	//======= set slide mode ===========//
	if (!cptr->Slide && cptr->vspeed > 0.6 && cptr->Phase != VEL_JUMP)
		if (AngleDifference(cptr->tgalpha, cptr->alpha) > pi * 2 / 3.f)
		{
			cptr->Slide = (int)(cptr->vspeed*700.f);
			cptr->slidex = cptr->lookx;
			cptr->slidez = cptr->lookz;
			cptr->vspeed = 0;
		}



	//========== movement ==============================//
	cptr->lookx = (float)cos(cptr->alpha);
	cptr->lookz = (float)sin(cptr->alpha);

	float curspeed = 0;
	if (cptr->Phase == VEL_RUN) curspeed = cptr->speed_run;
	if (cptr->Phase == VEL_JUMP) curspeed = cptr->speed_jump;
	if (cptr->Phase == VEL_WALK) curspeed = cptr->speed_walk;
	if (cptr->Phase == VEL_SWIM) curspeed = cptr->speed_swim;
	if (cptr->Phase == VEL_EAT) curspeed = 0.0f;

	if (cptr->Phase == VEL_RUN && cptr->Slide)
	{
		curspeed /= 8;
		if (drspd > pi / 2.f) curspeed = 0;
		else if (drspd > pi / 4.f) curspeed *= 2.f - 4.f*drspd / pi;
	}
	else if (drspd > pi / 2.f) curspeed *= 2.f - 2.f*drspd / pi;

	//========== process speed =============//

	DeltaFunc(cptr->vspeed, curspeed, TimeDt / 500.f);

	if (cptr->Phase == VEL_JUMP) cptr->vspeed = 1.1f;

	MoveCharacter(cptr, cptr->lookx * cptr->vspeed * TimeDt * cptr->scale,
		cptr->lookz * cptr->vspeed * TimeDt * cptr->scale, FALSE, TRUE);


	//========== slide ==============//
	if (cptr->Slide)
	{
		MoveCharacter(cptr, cptr->slidex * cptr->Slide / 600.f * TimeDt * cptr->scale,
			cptr->slidez * cptr->Slide / 600.f * TimeDt * cptr->scale, FALSE, TRUE);

		cptr->Slide -= TimeDt;
		if (cptr->Slide < 0) cptr->Slide = 0;
	}


	//============ Y movement =================//
	if (cptr->StateF & csONWATER)
	{
		cptr->pos.y = GetLandUpH(cptr->pos.x, cptr->pos.z) - 160 * cptr->scale;
		cptr->beta /= 2;
		cptr->tggamma = 0;
	}
	else
	{
		ThinkY_Beta_Gamma(cptr, 48, 24, 0.5f, 0.4f);
	}

	//=== process to tggamma ===//
	if (cptr->Phase == VEL_WALK) cptr->tggamma += cptr->rspeed / 7.0f;
	else cptr->tggamma += cptr->rspeed / 5.0f;
	if (cptr->Phase == VEL_JUMP) cptr->tggamma = 0;

	DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 1624.f);


	//==================================================//

}
*/



void AnimateTRex(TCharacter *cptr)
{
	NewPhase = FALSE;
	int _Phase = cptr->Phase;
	int _FTime = cptr->FTime;
	float _tgalpha = cptr->tgalpha;
	BOOL LookMode = FALSE;



TBEGIN:
	float targetx = cptr->tgx;
	float targetz = cptr->tgz;
	float targetdx = targetx - cptr->pos.x;
	float targetdz = targetz - cptr->pos.z;

	float tdist = (float)sqrt(targetdx * targetdx + targetdz * targetdz);

	float playerdx = PlayerX - cptr->pos.x - cptr->lookx * 108;
	float playerdz = PlayerZ - cptr->pos.z - cptr->lookz * 108;
	float pdist = (float)sqrt(playerdx * playerdx + playerdz * playerdz);
	float palpha = FindVectorAlpha(playerdx, playerdz);
	//if (cptr->State==2) { NewPhase=TRUE; cptr->State=1; }
	if (cptr->State == 5)
	{
		NewPhase = TRUE;
		cptr->State = 1;
		cptr->Phase = DinoInfo[cptr->CType].walkAnim;
		cptr->FTime = 0;
		cptr->tgx = PlayerX;
		cptr->tgz = PlayerZ;
		goto TBEGIN;
	}

	if (GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z) > DinoInfo[cptr->CType].waterLevel * cptr->scale)
		cptr->StateF |= csONWATER;
	else
		cptr->StateF &= (!csONWATER);

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) goto NOTHINK;

	//============================================//
	if (!MyHealth) cptr->State = 0;
	if (cptr->State)
	{
		cptr->tgx = PlayerX;
		cptr->tgz = PlayerZ;
		cptr->tgtime = 0;
		if (cptr->State > 1)
			if (AngleDifference(cptr->alpha, palpha) < 0.4f)
			{
				if (cptr->State == 2) {
					if (DinoInfo[cptr->CType].idleCount) {
						cptr->Phase = DinoInfo[cptr->CType].idleAnim[rRand(DinoInfo[cptr->CType].idleCount - 1)];
						cptr->State = 1;
						cptr->rspeed = 0;
					} else if (DinoInfo[cptr->CType].idle2Count) {
						cptr->Phase = DinoInfo[cptr->CType].idle2Anim[rRand(DinoInfo[cptr->CType].idle2Count - 1)];
						cptr->State = 1;
						cptr->rspeed = 0;
					}
					else if (DinoInfo[cptr->CType].roarCount) {
						cptr->Phase = cptr->roarAnim;
						cptr->State = 1;
						cptr->rspeed = 0;
					} else {
						cptr->Phase = DinoInfo[cptr->CType].runAnim;
						cptr->State = 1;
					}
				}
				else {
					if (DinoInfo[cptr->CType].idle2Count) {
						cptr->Phase = DinoInfo[cptr->CType].idle2Anim[rRand(DinoInfo[cptr->CType].idle2Count - 1)];
						cptr->State = 1;
						cptr->rspeed = 0;
					}
				}
			}




		if (pdist < DinoInfo[cptr->CType].killDist && DinoInfo[cptr->CType].killDist > 0)
		{
			int killAlt = DinoInfo[cptr->CType].waterLevel;
			if (killAlt < 256) killAlt = 256;
			if (fabs(PlayerY - cptr->pos.y) < killAlt + 20)
			{
				if (DinoInfo[cptr->CType].killTypeCount > 0) {

					if (!(cptr->StateF & csONWATER))
					{
						cptr->vspeed /= 8.0f;
						cptr->State = 1;
						cptr->Phase = DinoInfo[cptr->CType].killType[cptr->killType].anim;
						AddDeadBody(cptr, DinoInfo[cptr->CType].killType[cptr->killType].hunteranim);
					}
					else AddDeadBody(cptr, HUNT_EAT);

				}
				else {
					AddDeadBody(cptr, HUNT_EAT);
					cptr->State = 0;
				}

			}
		}



	}

	if (!cptr->State)
		if (tdist < 1224)
		{
			SetNewTargetPlace(cptr, 8048.f);
			goto TBEGIN;
		}


NOTHINK:
	if (pdist < 2048) cptr->NoFindCnt = 0;
	if (cptr->NoFindCnt) cptr->NoFindCnt--;
	else
	{
		cptr->tgalpha = CorrectedAlpha(FindVectorAlpha(targetdx, targetdz), cptr->alpha);//FindVectorAlpha(targetdx, targetdz);
		if (cptr->State && pdist > 5648)
		{
			cptr->tgalpha += (float)sin(RealTime / 824.f) / 6.f;
			if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
			if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;
		}
	}

	LookForAWay(cptr, !DinoInfo[cptr->CType].canSwim, !cptr->State);
	if (cptr->NoWayCnt > 12)
	{
		cptr->NoWayCnt = 0;
		cptr->NoFindCnt = 16 + rRand(20);
	}


	if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
	if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;

	//===============================================//

	ProcessPrevPhase(cptr);


	//======== select new phase =======================//
	for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
		if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) LookMode = TRUE;
	}

	for (int i = 0; i < DinoInfo[cptr->CType].idle2Count; i++) {
		if (cptr->Phase == DinoInfo[cptr->CType].idle2Anim[i]) LookMode = TRUE;
	}

	cptr->FTime += TimeDt;

	if (cptr->FTime >= cptr->pinfo->Animation[cptr->Phase].AniTime)
	{
		cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;
		NewPhase = TRUE;
	}

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount)    goto ENDPSELECT;

	if (!NewPhase)
		if (DinoInfo[cptr->CType].roarCount > 0 && cptr->Phase == cptr->roarAnim) goto ENDPSELECT;

	if (!cptr->State)
		if (NewPhase)
			if (rRand(128) > 110)
			{
				if (rRand(128) > 64) {
					if (DinoInfo[cptr->CType].idleCount) {
						cptr->Phase = DinoInfo[cptr->CType].idleAnim[rRand(DinoInfo[cptr->CType].idleCount - 1)];
					}
				}
				else {
					if (DinoInfo[cptr->CType].idle2Count) {
						cptr->Phase = DinoInfo[cptr->CType].idle2Anim[rRand(DinoInfo[cptr->CType].idle2Count - 1)];
					}
				}
				goto ENDPSELECT;
			}


	if (!NewPhase) if (LookMode) goto ENDPSELECT;

	if (cptr->State)
		if (NewPhase && LookMode)
		{
			if (DinoInfo[cptr->CType].roarCount > 0) {
				cptr->Phase = cptr->roarAnim;
				goto ENDPSELECT;
			}// else cptr->Phase = DinoInfo[cptr->CType].runAnim;
		}

	if (!cptr->State || cptr->State > 1) cptr->Phase = DinoInfo[cptr->CType].walkAnim;
	else if (fabs(cptr->tgalpha - cptr->alpha) < 1.0 ||
		fabs(cptr->tgalpha - cptr->alpha) > 2 * pi - 1.0)
		cptr->Phase = DinoInfo[cptr->CType].runAnim;
	else cptr->Phase = DinoInfo[cptr->CType].walkAnim;

	if (DinoInfo[cptr->CType].canSwim) {
		if (cptr->StateF & csONWATER) cptr->Phase = DinoInfo[cptr->CType].swimAnim;
	}

ENDPSELECT:

	//====== process phase changing ===========//
	if ((_Phase != cptr->Phase) || NewPhase)
		ActivateCharacterFx(cptr);

	if (_Phase != cptr->Phase)
	{
		//==== set proportional FTime for better morphing =//

		if (MORPHP) {
			if ((_Phase == DinoInfo[cptr->CType].runAnim ||
				_Phase == DinoInfo[cptr->CType].walkAnim) &&
				(cptr->Phase == DinoInfo[cptr->CType].runAnim ||
					cptr->Phase == DinoInfo[cptr->CType].walkAnim))
				cptr->FTime = _FTime * cptr->pinfo->Animation[cptr->Phase].AniTime / cptr->pinfo->Animation[_Phase].AniTime + 64;
			else if (!NewPhase) cptr->FTime = 0;
		}

		if (cptr->PPMorphTime > 128)
		{
			cptr->PrevPhase = _Phase;
			cptr->PrevPFTime = _FTime;
			cptr->PPMorphTime = 0;
		}
	}

	cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;



	//========== rotation to tgalpha ===================//

	float rspd, currspeed, tgbend;
	float dalpha = (float)fabs(cptr->tgalpha - cptr->alpha);
	float drspd = dalpha;
	if (drspd > pi) drspd = 2 * pi - drspd;

	if (DinoInfo[cptr->CType].roarCount > 0 && cptr->Phase == cptr->roarAnim) goto SKIPROT;
	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) goto SKIPROT;
	if (LookMode) goto SKIPROT;

	if (drspd > 0.02)
		if (cptr->tgalpha > cptr->alpha) currspeed = 0.7f + drspd * 1.4f;
		else currspeed = -0.7f - drspd * 1.4f;
	else currspeed = 0;
	if (cptr->AfraidTime) currspeed *= 2.5;

	if (dalpha > pi) currspeed *= -1;

	if (cptr->State) DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 440.f);
	else DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 620.f);

	tgbend = drspd / 2;
	if (tgbend > pi / 6.f) tgbend = pi / 6.f;

	tgbend *= SGN(currspeed);
	DeltaFunc(cptr->bend, tgbend, (float)TimeDt / 1800.f);




	rspd = cptr->rspeed * TimeDt / 1024.f;
	if (drspd < fabs(rspd)) cptr->alpha = cptr->tgalpha;
	else cptr->alpha += rspd;


	if (cptr->alpha > pi * 2) cptr->alpha -= pi * 2;
	if (cptr->alpha < 0) cptr->alpha += pi * 2;

SKIPROT:

	//========== movement ==============================//
	cptr->lookx = (float)cos(cptr->alpha);
	cptr->lookz = (float)sin(cptr->alpha);

	float curspeed = 0;
	if (cptr->Phase == DinoInfo[cptr->CType].runAnim) curspeed = cptr->speed_run;
	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) curspeed = cptr->speed_walk;
	if (DinoInfo[cptr->CType].canSwim) {
		if (cptr->Phase == DinoInfo[cptr->CType].swimAnim) curspeed = cptr->speed_swim;
	}

	if (drspd > pi / 2.f) curspeed *= 2.f - 2.f*drspd / pi;

	//========== process speed =============//

	DeltaFunc(cptr->vspeed, curspeed, TimeDt / 200.f);

	MoveCharacter(cptr, cptr->lookx * cptr->vspeed * TimeDt * cptr->scale,
		cptr->lookz * cptr->vspeed * TimeDt * cptr->scale, !DinoInfo[cptr->CType].canSwim, TRUE);

	//============ Y movement =================//
	if ((cptr->StateF & csONWATER) && DinoInfo[cptr->CType].canSwim)
	{
		cptr->pos.y = GetLandUpH(cptr->pos.x, cptr->pos.z) - (DinoInfo[cptr->CType].waterLevel - 20) * cptr->scale;
		cptr->beta /= 2;
		cptr->tggamma = 0;
	}
	else
	{
		ThinkY_Beta_Gamma(cptr, 348, 324, 0.5f, 0.4f);
	}



	//=== process to tggamma ===//
	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) cptr->tggamma += cptr->rspeed / 16.0f;
	else cptr->tggamma += cptr->rspeed / 12.0f;

	DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 2024.f);


	//==================================================//

}














void AnimateClassicAmbient(TCharacter *cptr)
{
	NewPhase = FALSE;
	int _Phase = cptr->Phase;
	int _FTime = cptr->FTime;
	float _tgalpha = cptr->tgalpha;
	if (cptr->AfraidTime) cptr->AfraidTime = MAX(0, cptr->AfraidTime - TimeDt);
	if (cptr->State == 2)
	{
		NewPhase = TRUE;
		cptr->State = 1;
	}

TBEGIN:
	float targetx = cptr->tgx;
	float targetz = cptr->tgz;
	float targetdx = targetx - cptr->pos.x;
	float targetdz = targetz - cptr->pos.z;

	float tdist = (float)sqrt(targetdx * targetdx + targetdz * targetdz);

	float playerdx = PlayerX - cptr->pos.x;
	float playerdz = PlayerZ - cptr->pos.z;
	float pdist = (float)sqrt(playerdx * playerdx + playerdz * playerdz);

	if (GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z) > DinoInfo[cptr->CType].waterLevel * cptr->scale)
		cptr->StateF |= csONWATER;
	else
		cptr->StateF &= (!csONWATER);


	//=========== run away =================//

	if (cptr->State)
	{
		if (!cptr->AfraidTime)
		{
			if (pdist < 2048.f) {
				if (cptr->Clone == AI_GALL) cptr->State = 1;
				cptr->AfraidTime = (5 + rRand(5)) * 1024;
			}

			if (!cptr->AfraidTime)
				if (pdist > 4096.f)
				{
					cptr->State = 0;
					SetNewTargetPlace(cptr, 2048.f);
					goto TBEGIN;
				}
		}


		nv.x = playerdx;
		nv.z = playerdz;
		nv.y = 0;
		NormVector(nv, 2048.f);
		cptr->tgx = cptr->pos.x - nv.x;
		cptr->tgz = cptr->pos.z - nv.z;
		cptr->tgtime = 0;
	}

	if (pdist > (ctViewR + 20) * 256 && cptr->AI > 0)
		if (ReplaceCharacterForward(cptr)) goto TBEGIN;


	//======== exploring area ===============//
	if (!cptr->State)
	{
		cptr->AfraidTime = 0;
		if (pdist < 812.f)
		{
			cptr->State = 1;
			cptr->AfraidTime = (5 + rRand(5)) * 1024;
			cptr->Phase = DinoInfo[cptr->CType].runAnim;
			goto TBEGIN;
		}


		if (tdist < 456)
		{
			SetNewTargetPlace(cptr, 2048.f);
			goto TBEGIN;
		}
	}


	//============================================//

	if (cptr->NoFindCnt) cptr->NoFindCnt--;
	else cptr->tgalpha = CorrectedAlpha(FindVectorAlpha(targetdx, targetdz), cptr->alpha);//FindVectorAlpha(targetdx, targetdz);
	LookForAWay(cptr, !DinoInfo[cptr->CType].canSwim, TRUE);
	if (cptr->NoWayCnt > 8)
	{
		cptr->NoWayCnt = 0;
		if (cptr->Clone == AI_GALL){
			cptr->NoFindCnt = 8 + rRand(40);
		} else {
			cptr->NoFindCnt = 8 + rRand(80);
		}
	}

	if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
	if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;

	//===============================================//

	ProcessPrevPhase(cptr);

	//======== select new phase =======================//
	cptr->FTime += TimeDt;

	if (cptr->FTime >= cptr->pinfo->Animation[cptr->Phase].AniTime)
	{
		cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;
		NewPhase = TRUE;
	}

	if (NewPhase)


		if (!cptr->State)
		{


			bool idlePhase = false;
			for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
				if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) idlePhase = true;
			}


			if (idlePhase) {
				if (rRand(128) > 76 && cptr->Phase == DinoInfo[cptr->CType].idleAnim[DinoInfo[cptr->CType].idleCount - 1])
					cptr->Phase = DinoInfo[cptr->CType].walkAnim;
				else cptr->Phase = DinoInfo[cptr->CType].idleAnim[rRand(DinoInfo[cptr->CType].idleCount - 1)];
				goto ENDPSELECT;
			}
			if (rRand(128) > 120) cptr->Phase = DinoInfo[cptr->CType].idleAnim[0];
			else cptr->Phase = DinoInfo[cptr->CType].walkAnim;

		}
		else if (cptr->AfraidTime) cptr->Phase = DinoInfo[cptr->CType].runAnim;
		else cptr->Phase = DinoInfo[cptr->CType].walkAnim;

	if (DinoInfo[cptr->CType].canSwim) {
		if (cptr->StateF & csONWATER) cptr->Phase = DinoInfo[cptr->CType].swimAnim;
	}

ENDPSELECT:

	//====== process phase changing ===========//
	if ((_Phase != cptr->Phase) || NewPhase)
		ActivateCharacterFx(cptr);

	if (_Phase != cptr->Phase)
	{
		if ((_Phase == DinoInfo[cptr->CType].runAnim ||
			_Phase == DinoInfo[cptr->CType].walkAnim) &&
			(cptr->Phase == DinoInfo[cptr->CType].runAnim ||
				cptr->Phase == DinoInfo[cptr->CType].walkAnim))
			cptr->FTime = _FTime * cptr->pinfo->Animation[cptr->Phase].AniTime / cptr->pinfo->Animation[_Phase].AniTime + 64;
		else if (!NewPhase) cptr->FTime = 0;

		if (cptr->PPMorphTime > 128)
		{
			cptr->PrevPhase = _Phase;
			cptr->PrevPFTime = _FTime;
			cptr->PPMorphTime = 0;
		}
	}

	cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;



	//========== rotation to tgalpha ===================//

	float rspd, currspeed, tgbend;
	float dalpha = (float)fabs(cptr->tgalpha - cptr->alpha);
	float drspd = dalpha;
	if (drspd > pi) drspd = 2 * pi - drspd;


	for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
		if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) goto SKIPROT;
	}


	if (drspd > 0.02)
		if (cptr->tgalpha > cptr->alpha) currspeed = 0.8f + drspd * 1.4f;
		else currspeed = -0.8f - drspd * 1.4f;
	else currspeed = 0;

	if (cptr->AfraidTime) currspeed *= 1.5;
	if (dalpha > pi) currspeed *= -1;
	if ((cptr->State & csONWATER) || cptr->Phase == DinoInfo[cptr->CType].walkAnim) currspeed /= 1.4f;

	DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 260.f);

	if (cptr->Clone == AI_GALL) {
		tgbend = drspd / 3;
	} else {
		tgbend = drspd / 2;
	}

	if (tgbend > pi / 2) tgbend = pi / 2;

	tgbend *= SGN(currspeed);
	if (fabs(tgbend) > fabs(cptr->bend)) DeltaFunc(cptr->bend, tgbend, (float)TimeDt / 800.f);
	else DeltaFunc(cptr->bend, tgbend, (float)TimeDt / 400.f);


	rspd = cptr->rspeed * TimeDt / 1024.f;
	if (drspd < fabs(rspd)) cptr->alpha = cptr->tgalpha;
	else cptr->alpha += rspd;


	if (cptr->alpha > pi * 2) cptr->alpha -= pi * 2;
	if (cptr->alpha < 0) cptr->alpha += pi * 2;

SKIPROT:

	//========== movement ==============================//
	cptr->lookx = (float)cos(cptr->alpha);
	cptr->lookz = (float)sin(cptr->alpha);

	float curspeed = 0;
	if (cptr->Phase == DinoInfo[cptr->CType].runAnim) curspeed = cptr->speed_run;
	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) curspeed = cptr->speed_walk;

	if (DinoInfo[cptr->CType].canSwim) {
		if (cptr->Phase == DinoInfo[cptr->CType].swimAnim) curspeed = cptr->speed_swim;
	}

	if (drspd > pi / 2.f) curspeed *= 2.f - 2.f*drspd / pi;

	//========== process speed =============//
	curspeed *= cptr->scale;
	DeltaFunc(cptr->vspeed, curspeed, TimeDt / 1024.f);

	MoveCharacter(cptr, cptr->lookx * cptr->vspeed * TimeDt,
		cptr->lookz * cptr->vspeed * TimeDt, !DinoInfo[cptr->CType].canSwim, TRUE);

	//============ Y movement =================//
	if (cptr->StateF & csONWATER && DinoInfo[cptr->CType].canSwim)
	{
		cptr->pos.y = GetLandUpH(cptr->pos.x, cptr->pos.z) - (DinoInfo[cptr->CType].waterLevel + 20) * cptr->scale;
		cptr->beta /= 2;
		cptr->tggamma = 0;
	}
	else {
		ThinkY_Beta_Gamma(cptr, 64, 32, 0.7f, 0.4f);
	}

	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) cptr->tggamma += cptr->rspeed / 12.0f;
	else cptr->tggamma += cptr->rspeed / 8.0f;
	DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 2048.f);
}



void AnimateFish(TCharacter *cptr)
{
	NewPhase = FALSE;
	int _Phase = cptr->Phase;
	int _FTime = cptr->FTime;
	float _tgalpha = cptr->tgalpha;
	Vector3d _pos = cptr->pos;
	float _depth = cptr->depth;
	float _beta = cptr->beta;

TBEGIN:
	float targetx = cptr->tgx;
	float targetz = cptr->tgz;
	float targety = cptr->tdepth;
	float targetdx = targetx - cptr->pos.x;
	float targetdz = targetz - cptr->pos.z;
	float targetdy = targety - cptr->depth;

	float tdist2 = (float)sqrt(targetdx * targetdx + targetdz * targetdz); //non-verticle
	float tdist = (float)sqrt(tdist2 * tdist2 + targetdy * targetdy); //verticle

	float attackDist = 1024.f;

	float playerdx = PlayerX - cptr->pos.x - cptr->lookx * 100 * cptr->scale;
	float playerdz = PlayerZ - cptr->pos.z - cptr->lookz * 100 * cptr->scale;
	float pdist = (float)sqrt(playerdx * playerdx + playerdz * playerdz);

	//REMOVED - turny !!!
	//if (cptr->State == 2)
	//{
	//	NewPhase = TRUE;
	//	cptr->State = 1;
	//}


	if (GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z) > 180 * cptr->scale)
		cptr->StateF |= csONWATER;
	else
		cptr->StateF &= (!csONWATER);

	bool playerInWater = GetLandUpH(PlayerX, PlayerZ) - GetLandH(PlayerX, PlayerZ) > 0;

	//============================================//
	if (!MyHealth) cptr->State = 0;

	if (!cptr->State)
	{
		if (pdist <= attackDist && playerInWater && !DinoInfo[cptr->CType].dontSwimAway) {
			cptr->State = 1;
			cptr->turny = 0;
			cptr->lastTBeta = cptr->beta;
			cptr->AfraidTime = (int)(10.f) * 1024;
			//goto TBEGIN;
		}
		else if (tdist2 < 456) // Ignore vertical
		{
			SetNewTargetPlaceFish(cptr, 1024.f);
			goto TBEGIN;
		}
	}

	if (cptr->State)
	{
		if (pdist > attackDist || !playerInWater)
		{
			cptr->AfraidTime -= TimeDt;
			if (cptr->AfraidTime <= 0)
			{
				cptr->AfraidTime = 0;
				cptr->State = 0;
				SetNewTargetPlaceFish(cptr, 1024.f);
				goto TBEGIN;
			}

		}

		if (cptr->AfraidTime > 0) {

			nv.x = playerdx;
			nv.z = playerdz;
			nv.y = 0;
			NormVector(nv, 2048.f);
			cptr->tgx = cptr->pos.x - nv.x;
			cptr->tgz = cptr->pos.z - nv.z;
			cptr->tgtime = 0;

			cptr->tdepth = GetLandH(cptr->pos.x, cptr->pos.z) +
				((GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z)) / 2);

			cptr->tgtime = 0;
		}

	}

NOTHINK:
	if (pdist < 2048) cptr->NoFindCnt = 0;
	if (cptr->NoFindCnt) cptr->NoFindCnt--;
	else
	{
		cptr->tgalpha = CorrectedAlpha(FindVectorAlpha(targetdx, targetdz), cptr->alpha);//FindVectorAlpha(targetdx, targetdz);
		if (cptr->State && pdist > 1648)
		{
			cptr->tgalpha += (float)sin(RealTime / 824.f) / 2.f;
			if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
			if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;
		}
	}

	LookForAWay(cptr, FALSE, TRUE);
	if (cptr->NoWayCnt > 12)
	{
		cptr->NoWayCnt = 0;
		cptr->NoFindCnt = 16 + rRand(20);
	}


	if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
	if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;

	//===============================================//

	ProcessPrevPhase(cptr);


	//======== select new phase =======================//
	cptr->FTime += TimeDt;

	if (cptr->FTime >= cptr->pinfo->Animation[cptr->Phase].AniTime)
	{
		cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;
		NewPhase = TRUE;

	}

	if (NewPhase)
		if (!cptr->State) cptr->Phase = DinoInfo[cptr->CType].walkAnim;
		else cptr->Phase = DinoInfo[cptr->CType].runAnim;

	if (!cptr->State) cptr->Phase = DinoInfo[cptr->CType].walkAnim;
	else if (fabs(cptr->tgalpha - cptr->alpha) < 1.0 ||
		fabs(cptr->tgalpha - cptr->alpha) > 2 * pi - 1.0)
		cptr->Phase = DinoInfo[cptr->CType].runAnim;
	else cptr->Phase = DinoInfo[cptr->CType].walkAnim;

	//if (cptr->StateF & csONWATER) cptr->Phase = RAP_SWIM;
	//if (cptr->Slide > 40) cptr->Phase = RAP_SLIDE;


ENDPSELECT:

	//====== process phase changing ===========//
	if ((_Phase != cptr->Phase) || NewPhase)
	{
//		if (cptr->Phase == DinoInfo[cptr->CType].walkAnim || cptr->Phase == DinoInfo[cptr->CType].runAnim) {
			ActivateCharacterFxAquatic(cptr);
//		}
//		else {
//			ActivateCharacterFx(cptr);
//		}
	}

	if (_Phase != cptr->Phase)
	{
		//==== set proportional FTime for better morphing =//
		//if (MORPHP)
		//	if (_Phase <= 3 && cptr->Phase <= 3)
		
		
		cptr->FTime = _FTime * cptr->pinfo->Animation[cptr->Phase].AniTime / cptr->pinfo->Animation[_Phase].AniTime + 64;
		

		//	else if (!NewPhase) cptr->FTime = 0;

		if (cptr->PPMorphTime > 128)
		{
			cptr->PrevPhase = _Phase;
			cptr->PrevPFTime = _FTime;
			cptr->PPMorphTime = 0;
		}
	}

	cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;



	//========== rotation to tgalpha ===================//

	float rspd, currspeed, tgbend;
	float dalpha = (float)fabs(cptr->tgalpha - cptr->alpha);
	float drspd = dalpha;
	if (drspd > pi) drspd = 2 * pi - drspd;

	if (drspd > 0.02)
		if (cptr->tgalpha > cptr->alpha) currspeed = 0.6f + drspd * 1.2f;
		else currspeed = -0.6f - drspd * 1.2f;
	else currspeed = 0;
	if (cptr->AfraidTime) currspeed *= 2.5;

	if (dalpha > pi) currspeed *= -1;
	if ((cptr->StateF & csONWATER) || cptr->Phase == DinoInfo[cptr->CType].walkAnim) currspeed /= 1.4f;

	if (cptr->AfraidTime) DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 160.f);
	else DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 180.f);

	tgbend = drspd / 2;
	if (tgbend > pi / 5) tgbend = pi / 5;

	tgbend *= SGN(currspeed);
	if (fabs(tgbend) > fabs(cptr->bend)) DeltaFunc(cptr->bend, tgbend, (float)TimeDt / 800.f);
	else DeltaFunc(cptr->bend, tgbend, (float)TimeDt / 600.f);


	rspd = cptr->rspeed * TimeDt / 1024.f;
	if (drspd < fabs(rspd)) cptr->alpha = cptr->tgalpha;
	else cptr->alpha += rspd;


	if (cptr->alpha > pi * 2) cptr->alpha -= pi * 2;
	if (cptr->alpha < 0) cptr->alpha += pi * 2;

SKIPROT:

	/*
	//======= set slide mode ===========//
	if (!cptr->Slide && cptr->vspeed > 0.6 && cptr->Phase != RAP_JUMP)
		if (AngleDifference(cptr->tgalpha, cptr->alpha) > pi * 2 / 3.f)
		{
			cptr->Slide = (int)(cptr->vspeed*700.f);
			cptr->slidex = cptr->lookx;
			cptr->slidez = cptr->lookz;
			cptr->vspeed = 0;
		}
		*/


		//========== movement ==============================//
	cptr->lookx = (float)cos(cptr->alpha);
	cptr->lookz = (float)sin(cptr->alpha);

	float curspeed = 0;
	if (cptr->Phase == DinoInfo[cptr->CType].runAnim) curspeed = cptr->speed_run;
	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) curspeed = cptr->speed_walk;

	/*
	if (cptr->Phase == RAP_RUN && cptr->Slide)
	{
		curspeed /= 8;
		if (drspd > pi / 2.f) curspeed = 0;
		else if (drspd > pi / 4.f) curspeed *= 2.f - 4.f*drspd / pi;
	}
	else*/ if (drspd > pi / 2.f) curspeed *= 2.f - 2.f*drspd / pi;

	//========== process speed =============//

	DeltaFunc(cptr->vspeed, curspeed, TimeDt / 500.f);

	MoveCharacterFish(cptr, cptr->lookx * cptr->vspeed * TimeDt * cptr->scale,
		cptr->lookz * cptr->vspeed * TimeDt * cptr->scale);

	/*
	//========== slide ==============//
	if (cptr->Slide)
	{
		MoveCharacter(cptr, cptr->slidex * cptr->Slide / 600.f * TimeDt * cptr->scale,
			cptr->slidez * cptr->Slide / 600.f * TimeDt * cptr->scale, FALSE, TRUE);

		cptr->Slide -= TimeDt;
		if (cptr->Slide < 0) cptr->Slide = 0;
	}
	*/

	//============ Y movement =================//


	float tdx2 = cptr->tgx - cptr->pos.x;
	float tdz2 = cptr->tgz - cptr->pos.z;
	float tdist22 = (float)sqrt(tdx2 * tdx2 + tdz2 * tdz2); //need this, it's an updated target dist

	float tbeta = -atan((cptr->tdepth - cptr->depth) / tdist22);

	if (cptr->turny < (pi)) {
		cptr->beta = (((cos(cptr->turny) + 1) / 2) * (cptr->lastTBeta - tbeta)) + tbeta;
		cptr->turny += pi / 100;
	}
	else {
		cptr->beta = tbeta;
	}

	cptr->depth -= cptr->beta * 35 * curspeed;

	float newBend = (_beta - cptr->beta) * 25;
	float max = 0.2;
	float maxIt = max / 6;

	if (fabs(cptr->bdepth - newBend) > maxIt) {
		if (newBend > cptr->bdepth) {
			cptr->bdepth += maxIt;
			if (cptr->bdepth > max) cptr->bdepth = max;
		}
		else {
			cptr->bdepth -= maxIt;
			if (cptr->bdepth < -max) cptr->bdepth = -max;
		}
	}
	else {
		cptr->bdepth = newBend;
	}

	/*
	if (cptr->beta < 0) cptr->beta += 2 * pi;
	if (cptr->beta > 2 * pi) cptr->beta -= 2 * pi;
	*/

	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) {
		cptr->tggamma = cptr->bend;
	}
	else {
		cptr->tggamma = cptr->bend * 2;	//run anim only
	}

	//=== process to tggamma ===//
	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) cptr->tggamma += cptr->rspeed / 10.0f;
	else cptr->tggamma += cptr->rspeed / 8.0f;

	DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 1624.f);


	// Mosa Depth Failsafes
	if (cptr->depth > GetLandUpH(cptr->pos.x, cptr->pos.z) - (DinoInfo[cptr->CType].spacingDepth / 2)) {
		cptr->depth = GetLandUpH(cptr->pos.x, cptr->pos.z) - (DinoInfo[cptr->CType].spacingDepth / 2);
		cptr->tdepth = GetLandH(cptr->pos.x, cptr->pos.z) +
			((GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z)) / 2);
		cptr->lastTBeta = cptr->beta;
	}
	if (cptr->depth < GetLandH(cptr->pos.x, cptr->pos.z) + (DinoInfo[cptr->CType].spacingDepth / 2)) {
		cptr->depth = GetLandH(cptr->pos.x, cptr->pos.z) + (DinoInfo[cptr->CType].spacingDepth / 2);
		cptr->tdepth = GetLandH(cptr->pos.x, cptr->pos.z) +
			((GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z)) / 2);
		cptr->lastTBeta = cptr->beta;
	}

	//==================================================//

	cptr->pos.y = cptr->depth;

}


//DO NOT DELETE, !!! animatemosa put in comments for now, compare to fish ai and update with new code
/*
void AnimateMosa(TCharacter *cptr)
{
	NewPhase = FALSE;
	int _Phase = cptr->Phase;
	int _FTime = cptr->FTime;
	float _tgalpha = cptr->tgalpha;
	Vector3d _pos = cptr->pos;
	float _depth = cptr->depth;
	float _beta = cptr->beta;

TBEGIN:
	float targetx = cptr->tgx;
	float targetz = cptr->tgz;
	float targety = cptr->tdepth;
	float targetdx = targetx - cptr->pos.x;
	float targetdz = targetz - cptr->pos.z;
	float targetdy = targety - cptr->depth;

	float tdist2 = (float)sqrt(targetdx * targetdx + targetdz * targetdz); //non-verticle
	float tdist = (float)sqrt(tdist2 * tdist2 + targetdy * targetdy); //verticle

	float attackDist = ctViewR * DinoInfo[cptr->CType].aggress + OptAgres / 4; //aggress = 128

	float playerdx = PlayerX - cptr->pos.x - cptr->lookx * 100 * cptr->scale;
	float playerdz = PlayerZ - cptr->pos.z - cptr->lookz * 100 * cptr->scale;
	float pdist = (float)sqrt(playerdx * playerdx + playerdz * playerdz);
	if (cptr->State == 2)
	{
		if (cptr->Phase != MOSA_JUMP) NewPhase = TRUE;
		cptr->State = 1;
	}


	if (GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z) > 180 * cptr->scale)
		cptr->StateF |= csONWATER;
	else
		cptr->StateF &= (!csONWATER);

	bool playerInWater = GetLandUpH(PlayerX, PlayerZ) - GetLandH(PlayerX, PlayerZ) > 0;

	if (cptr->Phase == MOSA_EAT) goto NOTHINK;

	//============================================//
	if (!MyHealth) cptr->State = 0;

	if (!cptr->State)
	{
		if (pdist <= attackDist && playerInWater && !ObservMode && DinoInfo[cptr->CType].aggress > 0) {
			cptr->State = 1;
			cptr->AfraidTime = (int)(10.f) * 1024;
			//goto TBEGIN;
		}
		else if (tdist2 < 456) // Ignore vertical
		{
			SetNewTargetPlaceMosasaurus(cptr, 3048.f);
			goto TBEGIN;
		}
	}

	if (cptr->State)
	{
		if (pdist > attackDist || !playerInWater)
		{
			cptr->AfraidTime -= TimeDt;
			if (cptr->AfraidTime <= 0)
			{
				cptr->AfraidTime = 0;
				cptr->State = 0;
				SetNewTargetPlaceMosasaurus(cptr, 3048.f);
				goto TBEGIN;
			}

		}

		if (cptr->AfraidTime > 0) {
			cptr->tgx = PlayerX;
			cptr->tgz = PlayerZ;
			cptr->tdepth = cptr->depth;//IN PROGRESS
			cptr->tgtime = 0;
		}

		//Cant jump for now
		if (!(cptr->StateF & csONWATER))
			if (pdist < 1324 * cptr->scale && pdist>900 * cptr->scale)
				if (AngleDifference(cptr->alpha, FindVectorAlpha(playerdx, playerdz)) < 0.2f)
					cptr->Phase = MOSA_JUMP;

		if (pdist < DinoInfo[cptr->CType].killDist && DinoInfo[cptr->CType].killDist > 0) //killdist=256
			if (fabs(PlayerY - cptr->pos.y - 160) < 256)
			{
				if (!(cptr->StateF & csONWATER))
				{
					cptr->vspeed /= 8.0f;
					cptr->State = 1;
					cptr->Phase = MOSA_EAT;
				}
				AddDeadBody(cptr, DinoInfo[cptr->CType].hunterDeathAnim);
			}
	}

NOTHINK:
	if (pdist < 2048) cptr->NoFindCnt = 0;
	if (cptr->NoFindCnt) cptr->NoFindCnt--;
	else
	{
		cptr->tgalpha = CorrectedAlpha(FindVectorAlpha(targetdx, targetdz), cptr->alpha);//FindVectorAlpha(targetdx, targetdz);
		if (cptr->State && pdist > 1648)
		{
			cptr->tgalpha += (float)sin(RealTime / 824.f) / 2.f;
			if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
			if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;
		}
	}

	LookForAWay(cptr, FALSE, TRUE);
	if (cptr->NoWayCnt > 12)
	{
		cptr->NoWayCnt = 0;
		cptr->NoFindCnt = 16 + rRand(20);
	}


	if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
	if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;

	//===============================================//

	ProcessPrevPhase(cptr);


	//======== select new phase =======================//
	cptr->FTime += TimeDt;

	if (cptr->FTime >= cptr->pinfo->Animation[cptr->Phase].AniTime)
	{
		cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;
		NewPhase = TRUE;

	}

	if (cptr->Phase == MOSA_EAT)  goto ENDPSELECT;
	if (NewPhase && _Phase == MOSA_JUMP)
	{
		cptr->Phase = MOSA_RUN;
		goto ENDPSELECT;
	}


	if (cptr->Phase == MOSA_JUMP) goto ENDPSELECT;

	if (NewPhase)
		if (!cptr->State) cptr->Phase = MOSA_WALK;
		else cptr->Phase = MOSA_RUN;

	if (!cptr->State) cptr->Phase = MOSA_WALK;
	else if (fabs(cptr->tgalpha - cptr->alpha) < 1.0 ||
		fabs(cptr->tgalpha - cptr->alpha) > 2 * pi - 1.0)
		cptr->Phase = MOSA_RUN;
	else cptr->Phase = MOSA_WALK;

	//if (cptr->StateF & csONWATER) cptr->Phase = RAP_SWIM;
	//if (cptr->Slide > 40) cptr->Phase = RAP_SLIDE;


ENDPSELECT:

	//====== process phase changing ===========//
	if ((_Phase != cptr->Phase) || NewPhase)
	{
		if (cptr->Phase == MOSA_WALK || cptr->Phase == MOSA_RUN
			|| cptr->Phase == MOSA_EAT || cptr->Phase == MOSA_DIE) {
			ActivateCharacterFxAquatic(cptr);
		}
		else {
			ActivateCharacterFx(cptr);
		}
	}

	if (_Phase != cptr->Phase)
	{
		//==== set proportional FTime for better morphing =//
		if (MORPHP)
			if (_Phase <= 3 && cptr->Phase <= 3)
				cptr->FTime = _FTime * cptr->pinfo->Animation[cptr->Phase].AniTime / cptr->pinfo->Animation[_Phase].AniTime + 64;
			else if (!NewPhase) cptr->FTime = 0;

		if (cptr->PPMorphTime > 128)
		{
			cptr->PrevPhase = _Phase;
			cptr->PrevPFTime = _FTime;
			cptr->PPMorphTime = 0;
		}
	}

	cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;



	//========== rotation to tgalpha ===================//

	float rspd, currspeed, tgbend;
	float dalpha = (float)fabs(cptr->tgalpha - cptr->alpha);
	float drspd = dalpha;
	if (drspd > pi) drspd = 2 * pi - drspd;

	if (cptr->Phase == MOSA_JUMP || cptr->Phase == MOSA_EAT) goto SKIPROT;

	if (drspd > 0.02)
		if (cptr->tgalpha > cptr->alpha) currspeed = 0.6f + drspd * 1.2f;
		else currspeed = -0.6f - drspd * 1.2f;
	else currspeed = 0;
	if (cptr->AfraidTime) currspeed *= 2.5;

	if (dalpha > pi) currspeed *= -1;
	if ((cptr->StateF & csONWATER) || cptr->Phase == MOSA_WALK) currspeed /= 1.4f;

	if (cptr->AfraidTime) DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 160.f);
	else DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 180.f);

	tgbend = drspd / 2;
	if (tgbend > pi / 5) tgbend = pi / 5;

	tgbend *= SGN(currspeed);
	if (fabs(tgbend) > fabs(cptr->bend)) DeltaFunc(cptr->bend, tgbend, (float)TimeDt / 800.f);
	else DeltaFunc(cptr->bend, tgbend, (float)TimeDt / 600.f);


	rspd = cptr->rspeed * TimeDt / 1024.f;
	if (drspd < fabs(rspd)) cptr->alpha = cptr->tgalpha;
	else cptr->alpha += rspd;


	if (cptr->alpha > pi * 2) cptr->alpha -= pi * 2;
	if (cptr->alpha < 0) cptr->alpha += pi * 2;

SKIPROT:



		//========== movement ==============================//
	cptr->lookx = (float)cos(cptr->alpha);
	cptr->lookz = (float)sin(cptr->alpha);

	float curspeed = 0;
	//if (cptr->Phase == MOSA_RUN) curspeed = cptr->speed_run;
	//if (cptr->Phase == MOSA_JUMP) curspeed = cptr->speed_jump;
	//if (cptr->Phase == MOSA_WALK) curspeed = cptr->speed_walk;
	if (cptr->Phase == MOSA_RUN) curspeed = cptr->speed_run;
	if (cptr->Phase == MOSA_JUMP) curspeed = cptr->speed_jump;
	if (cptr->Phase == MOSA_WALK) curspeed = cptr->speed_walk;


	if (cptr->Phase == MOSA_EAT) curspeed = 0.0f;

	if (drspd > pi / 2.f) curspeed *= 2.f - 2.f*drspd / pi;

	//========== process speed =============//

	DeltaFunc(cptr->vspeed, curspeed, TimeDt / 500.f);

	if (cptr->Phase == MOSA_JUMP) cptr->vspeed = 1.1f;

	MoveCharacterMosasaurus(cptr, cptr->lookx * cptr->vspeed * TimeDt * cptr->scale,
		cptr->lookz * cptr->vspeed * TimeDt * cptr->scale);



	//============ Y movement =================//


	float tdx2 = cptr->tgx - cptr->pos.x;
	float tdz2 = cptr->tgz - cptr->pos.z;
	float tdist22 = (float)sqrt(tdx2 * tdx2 + tdz2 * tdz2); //need this, it's an updated target dist

	float tbeta = -atan((cptr->tdepth - cptr->depth) / tdist22);

	if (cptr->turny < (pi)) { // && cptr->State == 0 ??
		cptr->beta = (((cos(cptr->turny) + 1) / 2) * (cptr->lastTBeta - tbeta)) + tbeta;
		cptr->turny += pi / 100;
	}
	else {
		cptr->beta = tbeta;
	}

	cptr->depth -= cptr->beta * 10;

	float newBend = (_beta - cptr->beta) * 25;
	float max = 0.2;
	float maxIt = max / 6;

	if (fabs(cptr->bdepth - newBend) > maxIt) {
		if (newBend > cptr->bdepth) {
			cptr->bdepth += maxIt;
			if (cptr->bdepth > max) cptr->bdepth = max;
		}
		else {
			cptr->bdepth -= maxIt;
			if (cptr->bdepth < -max) cptr->bdepth = -max;
		}
	}
	else {
		cptr->bdepth = newBend;
	}


	if (cptr->Phase == MOSA_WALK) {
		cptr->tggamma = cptr->bend;
	}
	else {
		cptr->tggamma = cptr->bend * 2;
	}

	//=== process to tggamma ===//
	if (cptr->Phase == MOSA_WALK) cptr->tggamma += cptr->rspeed / 10.0f;
	else cptr->tggamma += cptr->rspeed / 8.0f;
	if (cptr->Phase == MOSA_JUMP) cptr->tggamma = 0;

	DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 1624.f);


	// Mosa Depth Failsafes
	if (cptr->depth > GetLandUpH(cptr->pos.x, cptr->pos.z) - 300) {
		cptr->depth = GetLandUpH(cptr->pos.x, cptr->pos.z) - 300;
		cptr->tdepth = GetLandH(cptr->pos.x, cptr->pos.z) +
			((GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z)) / 2);
		cptr->lastTBeta = cptr->beta;
	}
	if (cptr->depth < GetLandH(cptr->pos.x, cptr->pos.z) + 100) {
		cptr->depth = GetLandH(cptr->pos.x, cptr->pos.z) + 100;
		cptr->tdepth = GetLandH(cptr->pos.x, cptr->pos.z) +
			((GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z)) / 2);
		cptr->lastTBeta = cptr->beta;
	}

	//==================================================//

	cptr->pos.y = cptr->depth;

}
*/










void AnimateIcth(TCharacter *cptr)
{
	NewPhase = FALSE;
	int _Phase = cptr->Phase;
	int _FTime = cptr->FTime;
	float _tgalpha = cptr->tgalpha;
	if (cptr->AfraidTime) cptr->AfraidTime = MAX(0, cptr->AfraidTime - TimeDt);
	if (cptr->State == 2)
	{
		NewPhase = TRUE;
		cptr->State = 1;
	}
	cptr->FTime += TimeDt;

TBEGIN:
	float targetx = cptr->tgx;
	float targetz = cptr->tgz;
	float targetdx = targetx - cptr->pos.x;
	float targetdz = targetz - cptr->pos.z;

	float tdist = (float)sqrt(targetdx * targetdx + targetdz * targetdz);

	float playerdx = PlayerX - cptr->pos.x;
	float playerdz = PlayerZ - cptr->pos.z;
	float pdist = (float)sqrt(playerdx * playerdx + playerdz * playerdz);
	float playerdy = PlayerY - cptr->pos.y;
	float pdistUp = (float)sqrt(pdist * pdist + playerdy * playerdy);

	//	if (cptr->AfraidTime && !(_Phase == ICTH_FLY || _Phase == ICTH_LANDING || _Phase == ICTH_FLY2 || _Phase == ICTH_TAKEOFF || _Phase == ICTH_WINGUP_WATER || _Phase == ICTH_WINGUP_LAND))
	//	{
	//		cptr->wingUp = true;
	//	}

	if (GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z) > 20)
		cptr->StateF |= csONWATER;
	else
		cptr->StateF &= (!csONWATER);

	//=========== run away =================//
	if (cptr->State)
	{

		if (!cptr->AfraidTime)
		{
			if (cptr->pos.y >= GetLandUpH(cptr->pos.x, cptr->pos.z) + 236)
			{
				cptr->gliding = true;
				SetNewTargetPlace_Icth(cptr, 2048.f);
			}
			else
			{
				cptr->Phase = DinoInfo[cptr->CType].landAnim;
				NewPhase = true;
				SetNewTargetPlace_Icth(cptr, 2048.f);
			}
			cptr->State = 0;
			goto TBEGIN;
		}

		/*		nv.x = playerdx;
				nv.z = playerdz;
				nv.y = 0;
				NormVector(nv, 2048.f);
				cptr->tgx = cptr->pos.x - nv.x;
				cptr->tgz = cptr->pos.z - nv.z;
				cptr->tgtime = 0;
				*/
	}


	//======== exploring area ===============//
	if (!cptr->State)
	{
		cptr->AfraidTime = 0;
		if (pdistUp < 1050.f)
		{
			cptr->State = 1;
			SetNewTargetPlace_Icth(cptr, 2048.f);
			cptr->AfraidTime = (50 + rRand(8)) * 1024;
			NewPhase = true;
			goto TBEGIN;
		}

		int targetNear = 456;

		if (cptr->Phase == DinoInfo[cptr->CType].flyAnim || cptr->Phase == DinoInfo[cptr->CType].glideAnim) {
			targetNear = 2024;
		}

		if (tdist < targetNear)
		{
			//if (!cptr->AfraidTime)
			//{
			//	SetNewTargetPlace_Icth(cptr, 2048.f);
			//}
			//else
			//{
			SetNewTargetPlace_Icth(cptr, 2048.f);
			//}
			goto TBEGIN;

		}
	}


	//===============================================//

	ProcessPrevPhase(cptr);



	//======== select new phase =======================//


	if (cptr->FTime >= cptr->pinfo->Animation[cptr->Phase].AniTime)
	{
		cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;
		NewPhase = TRUE;
	}

	float wy = GetLandUpH(cptr->pos.x,
		cptr->pos.z) - GetLandH(cptr->pos.x,
			cptr->pos.z);
	float swimLevel = 40;

	if (NewPhase)
	{
		if (!cptr->State)
		{

			if (cptr->gliding == true)
			{
				cptr->Phase = DinoInfo[cptr->CType].glideAnim;
			}
			else if (cptr->Phase != DinoInfo[cptr->CType].landAnim)
			{
				if (wy >= swimLevel) {
					cptr->Phase = DinoInfo[cptr->CType].swimAnim;
					if (rRand(128) > 110) {

						if (DinoInfo[cptr->CType].idle2Count > 0) {
							cptr->Phase = DinoInfo[cptr->CType].idle2Anim[rRand(DinoInfo[cptr->CType].idle2Count - 1)];
						}

					}
				}
				else
				{
					cptr->Phase = DinoInfo[cptr->CType].walkAnim;
					if (rRand(128) > 110) {
						
						if (DinoInfo[cptr->CType].idleCount > 0) {
							cptr->Phase = DinoInfo[cptr->CType].idleAnim[rRand(DinoInfo[cptr->CType].idleCount - 1)];
						}

					}
				}
			}

		}
		else if (cptr->AfraidTime) {

			if (cptr->Phase == DinoInfo[cptr->CType].flyAnim)
			{
				if (cptr->pos.y > GetLandUpH(cptr->pos.x, cptr->pos.z) + 2100)
				{
					cptr->Phase = DinoInfo[cptr->CType].glideAnim;
					SetNewTargetPlace_Icth(cptr, 2048.f);
				}
			}
			else if (cptr->Phase == DinoInfo[cptr->CType].glideAnim)
			{
				if (cptr->pos.y < GetLandUpH(cptr->pos.x, cptr->pos.z) + 1600)
				{
					cptr->Phase = DinoInfo[cptr->CType].flyAnim;
					SetNewTargetPlace_Icth(cptr, 2048.f);
				}
			}
			else if (cptr->Phase == DinoInfo[cptr->CType].takeoffAnim)
			{
				if (cptr->pos.y > GetLandUpH(cptr->pos.x, cptr->pos.z) + 236)
				{
					cptr->Phase = DinoInfo[cptr->CType].flyAnim;
				}
			}
			else
			{
				cptr->Phase = DinoInfo[cptr->CType].takeoffAnim;
				if (cptr->notFlushed == false)
				{
					ActivateCharacterFx(cptr);
				}
				else
				{
					cptr->notFlushed = false;
				}

				cptr->gamma = 0;
				cptr->beta = 0;
				cptr->bend = 0;//?
			}


		}
		else {
			if (cptr->gliding == true)
			{
				cptr->Phase = DinoInfo[cptr->CType].glideAnim;
			}
			else if (cptr->Phase != DinoInfo[cptr->CType].landAnim)
			{
				if (wy >= swimLevel) {
					cptr->Phase = DinoInfo[cptr->CType].swimAnim;
					if (rRand(128) > 110) {

						if (DinoInfo[cptr->CType].idle2Count > 0) {
							cptr->Phase = DinoInfo[cptr->CType].idle2Anim[rRand(DinoInfo[cptr->CType].idle2Count - 1)];
						}

					}
				}
				else
				{
					cptr->Phase = DinoInfo[cptr->CType].walkAnim;
					if (rRand(128) > 110) {
						
						if (DinoInfo[cptr->CType].idleCount > 0) {
							cptr->Phase = DinoInfo[cptr->CType].idleAnim[rRand(DinoInfo[cptr->CType].idleCount - 1)];
						}

					}
				}
			}
		}

		if (DinoInfo[cptr->CType].idleCount > 0) {
			for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
				if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) {

					if (rRand(24) > 23)
					{
						cptr->State = 1;
						SetNewTargetPlace_Icth(cptr, 2048.f);
						cptr->AfraidTime = (50 + rRand(8)) * 1024;
						cptr->notFlushed = true;
						NewPhase = true;
						goto TBEGIN;
					}

				}
			}
		}

		if (DinoInfo[cptr->CType].idle2Count > 0) {
			for (int i = 0; i < DinoInfo[cptr->CType].idle2Count; i++) {
				if (cptr->Phase == DinoInfo[cptr->CType].idle2Anim[i]) {

					if (rRand(24) > 23)
					{
						cptr->State = 1;
						SetNewTargetPlace_Icth(cptr, 2048.f);
						cptr->AfraidTime = (50 + rRand(8)) * 1024;
						cptr->notFlushed = true;
						NewPhase = true;
						goto TBEGIN;
					}

				}
			}
		}


	}

	if (cptr->gliding == true) {
		if (cptr->pos.y <= GetLandUpH(cptr->pos.x, cptr->pos.z) + 236)
		{
			cptr->gliding = false;
			cptr->Phase = DinoInfo[cptr->CType].landAnim;
			NewPhase = true;
			goto TBEGIN;
		}
	}

	if (cptr->Phase == DinoInfo[cptr->CType].landAnim) {
		if (cptr->pos.y <= GetLandUpH(cptr->pos.x, cptr->pos.z) + 15)
		{
			if (cptr->StateF & csONWATER)
			{
				cptr->Phase = DinoInfo[cptr->CType].shakeWaterAnim;
			}
			else
			{
				cptr->Phase = DinoInfo[cptr->CType].shakeLandAnim;
			}
			//TODO Set beta/gamma and such on land? - might be better to set it further down?
		}
		else if (cptr->pos.y > GetLandUpH(cptr->pos.x, cptr->pos.z) + 256)
		{
			cptr->gliding = true;
			NewPhase = true;
			goto TBEGIN;
		}
	}



	if (wy >= swimLevel)
	{
		if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) {
			NewPhase = true;
			goto TBEGIN;
		}

		if (DinoInfo[cptr->CType].idleCount > 0) {
			for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
				if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) {
					NewPhase = true;
					goto TBEGIN;
				}
			}
		}
	}


	if (!(wy >= swimLevel))
	{
		if (cptr->Phase == DinoInfo[cptr->CType].swimAnim) {
			NewPhase = true;
			goto TBEGIN;
		}

		if (DinoInfo[cptr->CType].idle2Count > 0) {
			for (int i = 0; i < DinoInfo[cptr->CType].idle2Count; i++) {
				if (cptr->Phase == DinoInfo[cptr->CType].idle2Anim[i]) {
					NewPhase = true;
					goto TBEGIN;
				}
			}
		}
	}



	//LAST


	if (NewPhase)
	{

		bool idleWalk = false;

		if (DinoInfo[cptr->CType].idleCount > 0) {
			for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
				if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) {
					idleWalk = true;
				}
			}
		}

		if (cptr->Phase == DinoInfo[cptr->CType].walkAnim || idleWalk)
		{
			if (cptr->shakeTime < 9)
			{
				cptr->shakeTime = cptr->shakeTime + 1;
			}

			if (cptr->shakeTime == 8)
			{
				cptr->Phase = DinoInfo[cptr->CType].shakeLandAnim;
			}
		}
		else
		{
			cptr->shakeTime = 0;
		}
	}



	//============================================//

	if (cptr->Phase == DinoInfo[cptr->CType].flyAnim || cptr->Phase == DinoInfo[cptr->CType].glideAnim
		|| cptr->Phase == DinoInfo[cptr->CType].takeoffAnim || cptr->Phase == DinoInfo[cptr->CType].landAnim) {
		cptr->tgalpha = CorrectedAlpha(FindVectorAlpha(targetdx, targetdz), cptr->alpha);//FindVectorAlpha(targetdx, targetdz);
	}
	else
	{

		if (cptr->NoFindCnt) cptr->NoFindCnt--;
		else
		{
			cptr->tgalpha = CorrectedAlpha(FindVectorAlpha(targetdx, targetdz), cptr->alpha);//FindVectorAlpha(targetdx, targetdz);
			if (cptr->AfraidTime)
			{
				cptr->tgalpha += (float)sin(RealTime / 1024.f) / 3.f;
				if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
				if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;
			}
		}


		LookForAWay(cptr, FALSE, TRUE);
		if (cptr->NoWayCnt > 12)
		{
			cptr->NoWayCnt = 0;
			cptr->NoFindCnt = 32 + rRand(60);
		}

	}

	if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
	if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;

ENDPSELECT:

	//====== process phase changing ===========//

	if ((_Phase != cptr->Phase) || NewPhase)
	{
		if (cptr->Phase == DinoInfo[cptr->CType].flyAnim || cptr->Phase == DinoInfo[cptr->CType].glideAnim)
		{

			if ((rand() & 1023) > 880)
			{
				ActivateCharacterFx(cptr);
			}
		}
		else if (cptr->Phase != DinoInfo[cptr->CType].takeoffAnim)
		{
			ActivateCharacterFx(cptr);
		}

	}

	if (_Phase != cptr->Phase)
	{

		if((_Phase == DinoInfo[cptr->CType].walkAnim || _Phase == DinoInfo[cptr->CType].swimAnim || _Phase == DinoInfo[cptr->CType].flyAnim
			|| _Phase == DinoInfo[cptr->CType].glideAnim || _Phase == DinoInfo[cptr->CType].landAnim || _Phase == DinoInfo[cptr->CType].takeoffAnim)
			&&
			(cptr->Phase == DinoInfo[cptr->CType].walkAnim || cptr->Phase == DinoInfo[cptr->CType].swimAnim || cptr->Phase == DinoInfo[cptr->CType].flyAnim
				|| cptr->Phase == DinoInfo[cptr->CType].glideAnim || cptr->Phase == DinoInfo[cptr->CType].landAnim || cptr->Phase == DinoInfo[cptr->CType].takeoffAnim))
			cptr->FTime = _FTime * cptr->pinfo->Animation[cptr->Phase].AniTime / cptr->pinfo->Animation[_Phase].AniTime + 64;
		else if (!NewPhase) cptr->FTime = 0;

		if (cptr->PPMorphTime > 128)
		{
			cptr->PrevPhase = _Phase;
			cptr->PrevPFTime = _FTime;
			cptr->PPMorphTime = 0;
		}
	}

	cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;




	//========== rotation to tgalpha ===================//

	float rspd, currspeed, tgbend;
	float dalpha = (float)fabs(cptr->tgalpha - cptr->alpha);
	float drspd = dalpha;
	if (drspd > pi) drspd = 2 * pi - drspd;

	if (DinoInfo[cptr->CType].idleCount > 0) {
		for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
			if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) {
				goto SKIPROT;
			}
		}
	}

	if (cptr->Phase == DinoInfo[cptr->CType].shakeLandAnim) goto SKIPROT;

	if (drspd > 0.02)
		if (cptr->Phase == DinoInfo[cptr->CType].flyAnim || cptr->Phase == DinoInfo[cptr->CType].glideAnim
			|| cptr->Phase == DinoInfo[cptr->CType].takeoffAnim || cptr->Phase == DinoInfo[cptr->CType].landAnim)
		{
			if (cptr->tgalpha > cptr->alpha) currspeed = 0.6f + drspd * 1.2f;
			else currspeed = -0.6f - drspd * 1.2f;
		}
		else
		{
			if (cptr->tgalpha > cptr->alpha) currspeed = 0.2f + drspd * 1.0f;
			else currspeed = -0.2f - drspd * 1.0f;
		}
	else currspeed = 0;

	//if (cptr->AfraidTime) currspeed *= 1.5;
	if (dalpha > pi) currspeed *= -1;


	if (DinoInfo[cptr->CType].idle2Count > 0) {
		for (int i = 0; i < DinoInfo[cptr->CType].idle2Count; i++) {
			if (cptr->Phase == DinoInfo[cptr->CType].idle2Anim[i]) {
				currspeed /= 1.4f;
			}
		}
	}

	if (cptr->Phase == DinoInfo[cptr->CType].swimAnim || cptr->Phase == DinoInfo[cptr->CType].shakeWaterAnim) currspeed /= 1.4f;

	if (cptr->Phase == DinoInfo[cptr->CType].flyAnim || cptr->Phase == DinoInfo[cptr->CType].glideAnim
		|| cptr->Phase == DinoInfo[cptr->CType].takeoffAnim || cptr->Phase == DinoInfo[cptr->CType].landAnim)
	{
		DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 460.f);
	}
	else
	{
		DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 400.f);
	}

	tgbend = drspd / 2.f;
	if (cptr->Phase == DinoInfo[cptr->CType].flyAnim || cptr->Phase == DinoInfo[cptr->CType].glideAnim
		|| cptr->Phase == DinoInfo[cptr->CType].takeoffAnim || cptr->Phase == DinoInfo[cptr->CType].landAnim)
	{
		if (tgbend > pi / 2) tgbend = pi / 2;
	}
	else
	{
		if (tgbend > pi / 3.f) tgbend = pi / 3.f;
	}

	tgbend *= SGN(currspeed);
	if (cptr->Phase == DinoInfo[cptr->CType].flyAnim || cptr->Phase == DinoInfo[cptr->CType].glideAnim
		|| cptr->Phase == DinoInfo[cptr->CType].takeoffAnim || cptr->Phase == DinoInfo[cptr->CType].landAnim)
	{
		if (fabs(tgbend) > fabs(cptr->bend)) DeltaFunc(cptr->bend, tgbend, (float)TimeDt / 800.f);
		else DeltaFunc(cptr->bend, tgbend, (float)TimeDt / 400.f);

		rspd = cptr->rspeed * TimeDt / 1024.f;
	}
	else
	{
		DeltaFunc(cptr->bend, tgbend, (float)TimeDt / 2000.f);

		rspd = cptr->rspeed * TimeDt / 612.f;
	}

	if (drspd < fabs(rspd)) cptr->alpha = cptr->tgalpha;
	else cptr->alpha += rspd;


	if (cptr->alpha > pi * 2) cptr->alpha -= pi * 2;
	if (cptr->alpha < 0) cptr->alpha += pi * 2;

SKIPROT:

	//========== movement ==============================//
	cptr->lookx = (float)cos(cptr->alpha);
	cptr->lookz = (float)sin(cptr->alpha);

	float curspeed = 0;
	/*
	if (cptr->Phase == ICTH_FLY) curspeed = cptr->speed_fly;//2.00f;
	if (cptr->Phase == ICTH_FLY2) curspeed = cptr->speed_glide;//1.80f;
	if (cptr->Phase == ICTH_TAKEOFF) curspeed = cptr->speed_takeoff;// 1.50f;
	if (cptr->Phase == ICTH_LANDING) curspeed = cptr->speed_land;// 0.30f;
	if (cptr->Phase == ICTH_WALK) curspeed = cptr->speed_walk;//0.10f;
	if (cptr->Phase == ICTH_SWIM) curspeed = cptr->speed_swim;//0.15f;
	if (cptr->Phase == ICTH_SWIM_IDLE1) curspeed = cptr->speed_swim;//0.15f;
	if (cptr->Phase == ICTH_SWIM_IDLE2) curspeed = cptr->speed_swim;//0.15f;
	if (cptr->Phase == ICTH_WINGDOWN_WATER) curspeed = cptr->speed_swim;//0.15f;
	*/
	if (cptr->Phase == DinoInfo[cptr->CType].flyAnim) curspeed = cptr->speed_fly;
	if (cptr->Phase == DinoInfo[cptr->CType].glideAnim) curspeed = cptr->speed_glide;
	if (cptr->Phase == DinoInfo[cptr->CType].takeoffAnim) curspeed = cptr->speed_takeoff;
	if (cptr->Phase == DinoInfo[cptr->CType].landAnim) curspeed = cptr->speed_land;
	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) curspeed = cptr->speed_walk;
	if (cptr->Phase == DinoInfo[cptr->CType].swimAnim) curspeed = cptr->speed_swim;
	if (cptr->Phase == DinoInfo[cptr->CType].shakeWaterAnim) curspeed = cptr->speed_swim;

	if (DinoInfo[cptr->CType].idle2Count > 0) {
		for (int i = 0; i < DinoInfo[cptr->CType].idle2Count; i++) {
			if (cptr->Phase == DinoInfo[cptr->CType].idle2Anim[i]) {
				curspeed = cptr->speed_swim;
			}
		}
	}

	if (drspd > pi / 2.f) curspeed *= 2.f - 2.f*drspd / pi;



	if (cptr->Phase == DinoInfo[cptr->CType].flyAnim || cptr->Phase == DinoInfo[cptr->CType].glideAnim
		|| cptr->Phase == DinoInfo[cptr->CType].takeoffAnim || cptr->Phase == DinoInfo[cptr->CType].landAnim)
	{
		if (cptr->Phase == DinoInfo[cptr->CType].flyAnim)
			DeltaFunc(cptr->pos.y, GetLandUpH(cptr->pos.x, cptr->pos.z) + 4048, TimeDt / 6.f);

		if (cptr->Phase == DinoInfo[cptr->CType].glideAnim)
		{
			if (cptr->gliding == true)
			{
				DeltaFunc(cptr->pos.y, GetLandUpH(cptr->pos.x, cptr->pos.z), TimeDt / 8.f);
			}
			else
			{
				DeltaFunc(cptr->pos.y, GetLandUpH(cptr->pos.x, cptr->pos.z), TimeDt / 16.f);
			}
		}

		if (cptr->Phase == DinoInfo[cptr->CType].takeoffAnim)
			DeltaFunc(cptr->pos.y, GetLandUpH(cptr->pos.x, cptr->pos.z) + 4048, TimeDt / 5.f);

		if (cptr->Phase == DinoInfo[cptr->CType].landAnim)
			DeltaFunc(cptr->pos.y, GetLandUpH(cptr->pos.x, cptr->pos.z), TimeDt / 4.f);


		if (cptr->gliding == false)
		{
			if (cptr->Phase != DinoInfo[cptr->CType].landAnim && cptr->Phase != DinoInfo[cptr->CType].takeoffAnim) {
				if (cptr->pos.y < GetLandUpH(cptr->pos.x, cptr->pos.z) + 236)
					cptr->pos.y = GetLandUpH(cptr->pos.x, cptr->pos.z) + 256;
			}
			else {
				if (cptr->pos.y < GetLandUpH(cptr->pos.x, cptr->pos.z))
					cptr->pos.y = GetLandUpH(cptr->pos.x, cptr->pos.z);
			}

		}

	}



	//========== process speed =============//

	bool swimmingAnim = FALSE;
	if (cptr->Phase == DinoInfo[cptr->CType].swimAnim || DinoInfo[cptr->CType].shakeWaterAnim) swimmingAnim = TRUE;
	if (DinoInfo[cptr->CType].idle2Count > 0) {
		for (int i = 0; i < DinoInfo[cptr->CType].idle2Count; i++) {
			if (cptr->Phase == DinoInfo[cptr->CType].idle2Anim[i]) {
				swimmingAnim = TRUE;
			}
		}
	}

	curspeed *= cptr->scale;

	if (cptr->Phase == DinoInfo[cptr->CType].flyAnim || cptr->Phase == DinoInfo[cptr->CType].glideAnim
		|| cptr->Phase == DinoInfo[cptr->CType].takeoffAnim || cptr->Phase == DinoInfo[cptr->CType].landAnim)
	{
		DeltaFunc(cptr->vspeed, curspeed, TimeDt / 2024.f);

		cptr->pos.x += cptr->lookx * cptr->vspeed * TimeDt;
		cptr->pos.z += cptr->lookz * cptr->vspeed * TimeDt;

		cptr->tggamma = cptr->rspeed / 4.0f;
		if (cptr->tggamma > pi / 6.f) cptr->tggamma = pi / 6.f;
		if (cptr->tggamma < -pi / 6.f) cptr->tggamma = -pi / 6.f;
	}
	else
	{
		if (curspeed > cptr->vspeed) DeltaFunc(cptr->vspeed, curspeed, TimeDt / 1024.f);
		else DeltaFunc(cptr->vspeed, curspeed, TimeDt / 256.f);

		MoveCharacter(cptr, cptr->lookx * cptr->vspeed * TimeDt,
			cptr->lookz * cptr->vspeed * TimeDt, FALSE, TRUE);

		if (!swimmingAnim)
		{
			ThinkY_Beta_Gamma(cptr, 128, 64, 0.6f, 0.4f);
			if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) cptr->tggamma += cptr->rspeed / 16.0f;
			else cptr->tggamma += cptr->rspeed / 10.0f;
		}
	}

	if (swimmingAnim)
	{
		cptr->gamma = 0;
	}
	else
	{
		DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 2048.f);
	}


	//============ Y movement =================//
	if ((wy >= swimLevel) && cptr->Phase != DinoInfo[cptr->CType].flyAnim && cptr->Phase != DinoInfo[cptr->CType].glideAnim
		&& cptr->Phase != DinoInfo[cptr->CType].takeoffAnim && cptr->Phase != DinoInfo[cptr->CType].landAnim)
	{
		cptr->pos.y = GetLandUpH(cptr->pos.x, cptr->pos.z);
		//cptr->pos.y = GetLandUpH(cptr->pos.x, cptr->pos.z) - 20;
		cptr->beta /= 2;
		cptr->tggamma = 0;
	}

}







void AnimateIcthDead(TCharacter *cptr)
{

	if (cptr->Phase != DinoInfo[cptr->CType].fallAnim && cptr->Phase != DinoInfo[cptr->CType].dieAnim
		&& cptr->Phase != DinoInfo[cptr->CType].waterDieAnim && cptr->Phase != DinoInfo[cptr->CType].sleepAnim)
	{
		cptr->deathPhase = cptr->Phase;
		if (cptr->PPMorphTime > 128)
		{
			cptr->PrevPhase = cptr->Phase;
			cptr->PrevPFTime = cptr->FTime;
			cptr->PPMorphTime = 0;
		}

		cptr->FTime = 0;
		cptr->Phase = DinoInfo[cptr->CType].fallAnim;
		cptr->rspeed = 0;
		ActivateCharacterFx(cptr);
		return;
	}

	ProcessPrevPhase(cptr);

	float wh = GetLandUpH(cptr->pos.x, cptr->pos.z);
	float lh = GetLandH(cptr->pos.x, cptr->pos.z);
	BOOL OnWater = (wh - lh > 10);

	cptr->FTime += TimeDt;
	if (cptr->FTime >= cptr->pinfo->Animation[cptr->Phase].AniTime)
	{
		if (cptr->Phase == DinoInfo[cptr->CType].dieAnim || cptr->Phase == DinoInfo[cptr->CType].waterDieAnim
			|| cptr->Phase == DinoInfo[cptr->CType].sleepAnim)
		{
			if (cptr->canSleep)
			{
				cptr->FTime = 0;
				cptr->Phase = DinoInfo[cptr->CType].sleepAnim;
				ActivateCharacterFx(cptr);
			}
			else
			{
				cptr->FTime = cptr->pinfo->Animation[cptr->Phase].AniTime - 1;
			}
		}


	}
	else
		cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;


	//======= movement ===========//
	if (cptr->Phase == DinoInfo[cptr->CType].dieAnim || cptr->Phase == DinoInfo[cptr->CType].waterDieAnim
		|| cptr->Phase == DinoInfo[cptr->CType].sleepAnim)
		DeltaFunc(cptr->vspeed, 0, TimeDt / 400.f);
	else
		DeltaFunc(cptr->vspeed, 0, TimeDt / 1200.f);

	cptr->pos.x += cptr->lookx * cptr->vspeed * TimeDt;
	cptr->pos.z += cptr->lookz * cptr->vspeed * TimeDt;

	if (cptr->Phase == DinoInfo[cptr->CType].fallAnim)
	{
		if (OnWater)
			if (cptr->pos.y >= wh && (cptr->pos.y + cptr->rspeed * TimeDt / 1024) < wh)
			{
				AddWCircle(cptr->pos.x + siRand(128), cptr->pos.z + siRand(128), 2.0);
				AddWCircle(cptr->pos.x + siRand(128), cptr->pos.z + siRand(128), 2.5);
				AddWCircle(cptr->pos.x + siRand(128), cptr->pos.z + siRand(128), 3.0);
				AddWCircle(cptr->pos.x + siRand(128), cptr->pos.z + siRand(128), 3.5);
				AddWCircle(cptr->pos.x + siRand(128), cptr->pos.z + siRand(128), 3.0);
			}
		cptr->pos.y += cptr->rspeed * TimeDt / 1024;
		cptr->rspeed -= TimeDt * 2.56;

		if (cptr->pos.y <= wh)
		{
			cptr->pos.y = wh;

			if (cptr->PPMorphTime > 128)
			{
				cptr->PrevPhase = cptr->Phase;
				cptr->PrevPFTime = cptr->FTime;
				cptr->PPMorphTime = 0;
			}

			if (OnWater)
			{
				//				AddElements(cptr->pos.x + siRand(128), lh, cptr->pos.z + siRand(128), 4, 10);
				//				AddElements(cptr->pos.x + siRand(128), lh, cptr->pos.z + siRand(128), 4, 10);
				//				AddElements(cptr->pos.x + siRand(128), lh, cptr->pos.z + siRand(128), 4, 10);
				cptr->Phase = DinoInfo[cptr->CType].waterDieAnim;
			}
			else
			{
				cptr->Phase = DinoInfo[cptr->CType].dieAnim;
			}
			cptr->FTime = 0;
			ActivateCharacterFx(cptr);
		}

		bool walkingidle = false;
		if (DinoInfo[cptr->CType].idleCount > 0) {
			for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
				if (cptr->deathPhase == DinoInfo[cptr->CType].idleAnim[i]) {
					walkingidle = TRUE;
				}
			}
		}

		cptr->canSleep = (Tranq && !OnWater &&
			(cptr->deathPhase == DinoInfo[cptr->CType].walkAnim || cptr->deathPhase == DinoInfo[cptr->CType].shakeLandAnim || walkingidle));

	}
	else
	{
		ThinkY_Beta_Gamma(cptr, 140, 126, 0.6f, 0.5f);
		DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 1600.f);
	}

	if (cptr->Phase == DinoInfo[cptr->CType].waterDieAnim)
	{
		cptr->pos.y = wh;
		cptr->gamma = 0;
		cptr->beta = 0;
		cptr->alpha = 0;
	}
}









//NEW BRAHI
void AnimateBrahi(TCharacter *cptr)
{
	NewPhase = FALSE;
	int _Phase = cptr->Phase;
	int _FTime = cptr->FTime;
	float _tgalpha = cptr->tgalpha;

TBEGIN:
	//cptr->tgtime = 0;
	float targetx = cptr->tgx;
	float targetz = cptr->tgz;
	float targetdx = targetx - cptr->pos.x;
	float targetdz = targetz - cptr->pos.z;

	float tdist = (float)sqrt(targetdx * targetdx + targetdz * targetdz);

	float playerdx = PlayerX - cptr->pos.x - cptr->lookx * 108;
	float playerdz = PlayerZ - cptr->pos.z - cptr->lookz * 108;
	float pdist = (float)sqrt(playerdx * playerdx + playerdz * playerdz);

	int attackDist = 128 * DinoInfo[cptr->CType].aggress + OptAgres / 8; //agress = 56

	bool playerAttackable = ((GetLandUpH(PlayerX, PlayerZ) - GetLandH(PlayerX, PlayerZ)) <= 550);
	bool attacking = false;

	if (cptr->State == 2)
	{
		cptr->State = 1;
		cptr->Phase = DinoInfo[cptr->CType].runAnim;
	}

	if (GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z) > 140 * cptr->scale)
		cptr->StateF |= csONWATER;
	else
		cptr->StateF &= (!csONWATER);

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) goto NOTHINK;

	//============================================//
	if (!MyHealth) cptr->State = 0;
	if (cptr->State)
	{
		if (pdist <= attackDist && playerAttackable && DinoInfo[cptr->CType].aggress > 0)
		{
			attacking = true;
			cptr->tgx = PlayerX;
			cptr->tgz = PlayerZ;
			cptr->tgtime = 0;
		}
		else
		{
			attacking = false;
			nv.x = playerdx;
			nv.z = playerdz;
			nv.y = 0;
			NormVector(nv, 2048.f);
			cptr->tgx = cptr->pos.x - nv.x;
			cptr->tgz = cptr->pos.z - nv.z;
			cptr->tgtime = 0;
			cptr->AfraidTime -= TimeDt;
			if (cptr->AfraidTime <= 0)
			{
				cptr->AfraidTime = 0;
				cptr->State = 0;
			}
		}

		if (pdist < DinoInfo[cptr->CType].killDist && DinoInfo[cptr->CType].killDist > 0) //killdist = 600
			if (fabs(PlayerY - cptr->pos.y - 120) < 256)
			{

				if (DinoInfo[cptr->CType].killTypeCount > 0) {

					if (!(cptr->StateF & csONWATER))
					{
						cptr->vspeed /= 8.0f;
						cptr->State = 1;
						cptr->Phase = DinoInfo[cptr->CType].killType[cptr->killType].anim;
						AddDeadBody(cptr, DinoInfo[cptr->CType].killType[cptr->killType].hunteranim);
					}
					else AddDeadBody(cptr, HUNT_EAT);

				}
				else {
					AddDeadBody(cptr, HUNT_EAT);
					cptr->State = 0;
				}


			}
	}

	if (!cptr->State)
	{
		attacking = false;
		cptr->AfraidTime = 0;
		if (tdist < 256)
		{
			SetNewTargetPlace_Brahi(cptr, 2048.f);
			goto TBEGIN;
		}
	}

NOTHINK:
	if (pdist < 2048) cptr->NoFindCnt = 0;
	if (cptr->NoFindCnt) cptr->NoFindCnt--;
	else
	{
		cptr->tgalpha = CorrectedAlpha(FindVectorAlpha(targetdx, targetdz), cptr->alpha);//FindVectorAlpha(targetdx, targetdz);

		/*
		if (cptr->State && pdist > 1648)
		{
			cptr->tgalpha += (float)sin(RealTime / 824.f) / 4.f;
			if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
			if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;
		}
		*/

	}

	if (cptr->Clone == AI_LANDBRACH) {
		LookForAWay(cptr, !DinoInfo[cptr->CType].canSwim, TRUE);
	} else {
		LookForAWay(cptr, !attacking && cptr->State, TRUE);
	}


	if (cptr->NoWayCnt > 12)
	{
		cptr->NoWayCnt = 0;
		cptr->NoFindCnt = 16 + rRand(20);
	}

	if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
	if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;

	//===============================================//

	ProcessPrevPhase(cptr);

	//======== select new phase =======================//
	cptr->FTime += TimeDt;

	if (cptr->FTime >= cptr->pinfo->Animation[cptr->Phase].AniTime)
	{
		cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;
		NewPhase = TRUE;
	}

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount)  goto ENDPSELECT;

	if (NewPhase)
	{
		if (!cptr->State)
		{
			if (cptr->Phase != DinoInfo[cptr->CType].walkAnim)
			{
				if (rRand(128) > 90)
				{
					cptr->Phase = DinoInfo[cptr->CType].walkAnim;
				}
				else
				{
					cptr->Phase = DinoInfo[cptr->CType].idleAnim[rRand(DinoInfo[cptr->CType].idleCount - 1)];
				}
				goto ENDPSELECT;
			}
			if (rRand(128) > 64)
			{
				cptr->Phase = DinoInfo[cptr->CType].idleAnim[0];
			}
			else
			{
				cptr->Phase = DinoInfo[cptr->CType].walkAnim;
			}
		}
		else
		{
			cptr->Phase = DinoInfo[cptr->CType].runAnim;
		}
	}

	/*
	if (cptr->Phase != BRA_IDLE1 && cptr->Phase != BRA_IDLE2 && cptr->Phase != BRA_IDLE3)
		if (!cptr->State) cptr->Phase = BRA_WALK;
		else if (fabs(cptr->tgalpha - cptr->alpha) < 1.0 ||
			fabs(cptr->tgalpha - cptr->alpha) > 2 * pi - 1.0)
			cptr->Phase = BRA_RUN;
		else cptr->Phase = BRA_WALK;
	 */ //001 is this needed?

	 //cptr->Phase=BRA_WALK;

ENDPSELECT:

	//====== process phase changing ===========//
	if ((_Phase != cptr->Phase) || NewPhase)
		ActivateCharacterFx(cptr);

	if (_Phase != cptr->Phase)
	{

		if (cptr->PPMorphTime > 128)
		{
			cptr->PrevPhase = _Phase;
			cptr->PrevPFTime = _FTime;
			cptr->PPMorphTime = 0;
		}
	}

	cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;



	//========== rotation to tgalpha ===================//

	float rspd, currspeed, tgbend;
	float dalpha = (float)fabs(cptr->tgalpha - cptr->alpha);
	float drspd = dalpha;
	if (drspd > pi) drspd = 2 * pi - drspd;

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) goto SKIPROT;

	for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
		if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) goto SKIPROT;
	}

	if (drspd > 0.02)
		if (cptr->tgalpha > cptr->alpha) currspeed = 0.2f + drspd * 0.2f;
		else currspeed = -0.2f - drspd * 0.2f;
	else currspeed = 0;
	if (cptr->AfraidTime) currspeed *= DinoInfo[cptr->CType].rotspdmulti;

	if (dalpha > pi) currspeed *= -1;

	DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 600.f);

	tgbend = drspd / 4;
	if (tgbend > pi / 4) tgbend = pi / 4;

	tgbend *= SGN(currspeed);
	DeltaFunc(cptr->bend, tgbend, (float)TimeDt / 3200.f);

	rspd = cptr->rspeed * TimeDt / 1024.f;
	if (drspd < fabs(rspd)) cptr->alpha = cptr->tgalpha;
	else cptr->alpha += rspd;

	if (cptr->alpha > pi * 2) cptr->alpha -= pi * 2;
	if (cptr->alpha < 0) cptr->alpha += pi * 2;

SKIPROT:

	//========== movement ==============================//
	cptr->lookx = (float)cos(cptr->alpha);
	cptr->lookz = (float)sin(cptr->alpha);

	float curspeed = 0;

	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) curspeed = cptr->speed_walk;
	if (cptr->Phase == DinoInfo[cptr->CType].runAnim) curspeed = cptr->speed_run;

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) curspeed = 0.0f;

	if (drspd > pi / 2.f) curspeed *= 2.f - 2.f*drspd / pi;

	//========== process speed =============//
	curspeed *= cptr->scale;
	DeltaFunc(cptr->vspeed, curspeed, TimeDt / 1024.f);
	cptr->pos.x += cptr->lookx * cptr->vspeed * TimeDt;
	cptr->pos.z += cptr->lookz * cptr->vspeed * TimeDt;
	/*
	MoveCharacter(cptr, cptr->lookx * cptr->vspeed * TimeDt,
					   cptr->lookz * cptr->vspeed * TimeDt, TRUE, TRUE);
	*/

	ThinkY_Beta_Gamma(cptr, 256, 128, 0.1f, 0.2f);
	DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 4048.f);
}







//OLD BRAHI
void AnimateBrahiOld(TCharacter *cptr)
{
	NewPhase = FALSE;
	int _Phase = cptr->Phase;
	int _FTime = cptr->FTime;
	float _tgalpha = cptr->tgalpha;

TBEGIN:
	cptr->tgtime = 0;
	float targetx = cptr->tgx;
	float targetz = cptr->tgz;
	float targetdx = targetx - cptr->pos.x;
	float targetdz = targetz - cptr->pos.z;

	float tdist = (float)sqrt(targetdx * targetdx + targetdz * targetdz);


	if (tdist < 256)
	{
		SetNewTargetPlace_Brahi(cptr, 2048.f);
		goto TBEGIN;
	}

	//============================================//
	cptr->tgalpha = FindVectorAlpha(targetdx, targetdz);

	//============================================//

	if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
	if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;

	//===============================================//

	ProcessPrevPhase(cptr);

	//======== select new phase =======================//
	cptr->FTime += TimeDt;

	if (cptr->FTime >= cptr->pinfo->Animation[cptr->Phase].AniTime)
	{
		cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;
		NewPhase = TRUE;
	}

	if (NewPhase)
	{

		/*
		
							cptr->Phase = DinoInfo[cptr->CType].idleAnim[rRand(DinoInfo[cptr->CType].idleCount - 1)];
				}
				goto ENDPSELECT;
			}
			if (rRand(128) > 64)
			{
				cptr->Phase = DinoInfo[cptr->CType].idleAnim[0];
		
		*/


		if (cptr->Phase > DinoInfo[cptr->CType].walkAnim)
		{
			if (rRand(128) > 90)  cptr->Phase = DinoInfo[cptr->CType].walkAnim;
			else cptr->Phase = DinoInfo[cptr->CType].idleAnim[rRand(DinoInfo[cptr->CType].idleCount - 1)];
			goto ENDPSELECT;
		}
		if (rRand(128) > 64) cptr->Phase = DinoInfo[cptr->CType].idleAnim[0];
		else cptr->Phase = DinoInfo[cptr->CType].walkAnim;
	}


ENDPSELECT:

	//====== process phase changing ===========//
	if ((_Phase != cptr->Phase) || NewPhase)
		ActivateCharacterFx(cptr);

	if (_Phase != cptr->Phase)
	{

		if (cptr->PPMorphTime > 128)
		{
			cptr->PrevPhase = _Phase;
			cptr->PrevPFTime = _FTime;
			cptr->PPMorphTime = 0;
		}
	}

	cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;



	//========== rotation to tgalpha ===================//

	float rspd, currspeed, tgbend;
	float dalpha = (float)fabs(cptr->tgalpha - cptr->alpha);
	float drspd = dalpha;
	if (drspd > pi) drspd = 2 * pi - drspd;

	for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
		if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) goto SKIPROT;
	}

	if (drspd > 0.02)
		if (cptr->tgalpha > cptr->alpha) currspeed = 0.2f + drspd * 0.2f;
		else currspeed = -0.2f - drspd * 0.2f;
	else currspeed = 0;

	if (dalpha > pi) currspeed *= -1;

	DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 600.f);

	tgbend = drspd / 4;
	if (tgbend > pi / 4) tgbend = pi / 4;

	tgbend *= SGN(currspeed);
	DeltaFunc(cptr->bend, tgbend, (float)TimeDt / 3200.f);

	rspd = cptr->rspeed * TimeDt / 1024.f;
	if (drspd < fabs(rspd)) cptr->alpha = cptr->tgalpha;
	else cptr->alpha += rspd;

	if (cptr->alpha > pi * 2) cptr->alpha -= pi * 2;
	if (cptr->alpha < 0) cptr->alpha += pi * 2;

SKIPROT:

	//========== movement ==============================//
	cptr->lookx = (float)cos(cptr->alpha);
	cptr->lookz = (float)sin(cptr->alpha);

	float curspeed = 0;
	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) curspeed = cptr->speed_walk;

	if (drspd > pi / 2.f) curspeed *= 2.f - 2.f*drspd / pi;

	//========== process speed =============//
	curspeed *= cptr->scale;
	DeltaFunc(cptr->vspeed, curspeed, TimeDt / 1024.f);
	cptr->pos.x += cptr->lookx * cptr->vspeed * TimeDt;
	cptr->pos.z += cptr->lookz * cptr->vspeed * TimeDt;
	/*
	MoveCharacter(cptr, cptr->lookx * cptr->vspeed * TimeDt,
					   cptr->lookz * cptr->vspeed * TimeDt, TRUE, TRUE);
	*/

	ThinkY_Beta_Gamma(cptr, 256, 128, 0.1f, 0.2f);
	DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 4048.f);
}






void AnimateDimor(TCharacter *cptr)
{
	NewPhase = FALSE;
	int _Phase = cptr->Phase;
	int _FTime = cptr->FTime;
	float _tgalpha = cptr->tgalpha;


TBEGIN:
	float targetx = cptr->tgx;
	float targetz = cptr->tgz;
	float targetdx = targetx - cptr->pos.x;
	float targetdz = targetz - cptr->pos.z;

	float tdist = (float)sqrt(targetdx * targetdx + targetdz * targetdz);

	float playerdx = PlayerX - cptr->pos.x;
	float playerdz = PlayerZ - cptr->pos.z;
	float pdist = (float)sqrt(playerdx * playerdx + playerdz * playerdz);


	//=========== run away =================//

	if (pdist > (ctViewR + 20) * 256 && cptr->AI > 0)
		if (ReplaceCharacterForward(cptr)) goto TBEGIN;


	//======== exploring area ===============//
	if (tdist < 1024)
	{
		SetNewTargetPlace(cptr, 4048.f);
		goto TBEGIN;
	}


	//============================================//


	cptr->tgalpha = CorrectedAlpha(FindVectorAlpha(targetdx, targetdz), cptr->alpha);//FindVectorAlpha(targetdx, targetdz);
	if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
	if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;

	//===============================================//

	ProcessPrevPhase(cptr);

	//======== select new phase =======================//
	cptr->FTime += TimeDt;

	if (cptr->FTime >= cptr->pinfo->Animation[cptr->Phase].AniTime)
	{
		cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;
		NewPhase = TRUE;
	}

	if (NewPhase)
	{
		if (cptr->Phase == DinoInfo[cptr->CType].flyAnim)
			if (cptr->pos.y > GetLandH(cptr->pos.x, cptr->pos.z) + 2800)
				cptr->Phase = DinoInfo[cptr->CType].glideAnim;
			else;
		else if (cptr->Phase == DinoInfo[cptr->CType].glideAnim)
			if (cptr->pos.y < GetLandH(cptr->pos.x, cptr->pos.z) + 1800)
				cptr->Phase = DinoInfo[cptr->CType].flyAnim;
	}




	//====== process phase changing ===========//
	if ((_Phase != cptr->Phase) || NewPhase)
		if ((rand() & 1023) > 980)
			ActivateCharacterFx(cptr);

	if (_Phase != cptr->Phase)
	{
		if (!NewPhase) cptr->FTime = 0;
		if (cptr->PPMorphTime > 128)
		{
			cptr->PrevPhase = _Phase;
			cptr->PrevPFTime = _FTime;
			cptr->PPMorphTime = 0;
		}
	}


	cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;


	//========== rotation to tgalpha ===================//

	float rspd, currspeed, tgbend;
	float dalpha = (float)fabs(cptr->tgalpha - cptr->alpha);
	float drspd = dalpha;
	if (drspd > pi) drspd = 2 * pi - drspd;


	if (drspd > 0.02)
		if (cptr->tgalpha > cptr->alpha) currspeed = 0.6f + drspd * 1.2f;
		else currspeed = -0.6f - drspd * 1.2f;
	else currspeed = 0;

	if (dalpha > pi) currspeed *= -1;
	DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 460.f);

	tgbend = drspd / 2.f;
	if (tgbend > pi / 2) tgbend = pi / 2;

	tgbend *= SGN(currspeed);
	if (fabs(tgbend) > fabs(cptr->bend)) DeltaFunc(cptr->bend, tgbend, (float)TimeDt / 800.f);
	else DeltaFunc(cptr->bend, tgbend, (float)TimeDt / 400.f);


	rspd = cptr->rspeed * TimeDt / 1024.f;
	if (drspd < fabs(rspd)) cptr->alpha = cptr->tgalpha;
	else cptr->alpha += rspd;


	if (cptr->alpha > pi * 2) cptr->alpha -= pi * 2;
	if (cptr->alpha < 0) cptr->alpha += pi * 2;

	//========== movement ==============================//
	cptr->lookx = (float)cos(cptr->alpha);
	cptr->lookz = (float)sin(cptr->alpha);

	float curspeed = 0;
	if (cptr->Phase == DinoInfo[cptr->CType].flyAnim) curspeed = cptr->speed_fly;
	if (cptr->Phase == DinoInfo[cptr->CType].glideAnim) curspeed = cptr->speed_glide;

	if (drspd > pi / 2.f) curspeed *= 2.f - 2.f*drspd / pi;

	if (cptr->Phase == DinoInfo[cptr->CType].flyAnim)
		DeltaFunc(cptr->pos.y, GetLandH(cptr->pos.x, cptr->pos.z) + 4048, TimeDt / 6.f);
	else
		DeltaFunc(cptr->pos.y, GetLandH(cptr->pos.x, cptr->pos.z), TimeDt / 16.f);


	if (cptr->pos.y < GetLandH(cptr->pos.x, cptr->pos.z) + 236)
		cptr->pos.y = GetLandH(cptr->pos.x, cptr->pos.z) + 256;



	//========== process speed =============//
	curspeed *= cptr->scale;
	DeltaFunc(cptr->vspeed, curspeed, TimeDt / 2024.f);

	cptr->pos.x += cptr->lookx * cptr->vspeed * TimeDt;
	cptr->pos.z += cptr->lookz * cptr->vspeed * TimeDt;

	cptr->tggamma = cptr->rspeed / 4.0f;
	if (cptr->tggamma > pi / 6.f) cptr->tggamma = pi / 6.f;
	if (cptr->tggamma < -pi / 6.f) cptr->tggamma = -pi / 6.f;
	DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 2048.f);
}





void AnimateCharacters()
{
	//if (!RunMode) return;
	TCharacter *cptr;
	if (TrophyMode) return;

	//packs
	for (int packN = 0; packN < PackCount; packN++) {

		//PrintLog("PACKTESTING_");
		Packs[packN].alert = Packs[packN]._alert;
		Packs[packN].attack = Packs[packN]._attack;
		Packs[packN]._alert = FALSE;
		Packs[packN]._attack = FALSE;

	}

	for (CurDino = 0; CurDino < ChCount; CurDino++)
	{
		cptr = &Characters[CurDino];
		if (cptr->StateF == 0xFF) continue;
		cptr->tgtime += TimeDt;

		// replace pack leader
		if (cptr->Health && cptr->pack >= 0) {
			if (!Packs[cptr->pack].leader->Health) Packs[cptr->pack].leader = cptr;
		}



		if (cptr->tgtime > 30 * 1000) {
			if (cptr->Clone < 0) {

				switch (cptr->Clone) {
				case AI_BRACH:
					SetNewTargetPlace_Brahi(cptr, 2048.f);
					break;
				case AI_BRACHDANGER:
					SetNewTargetPlace_Brahi(cptr, 2048.f);
					break;
				case AI_LANDBRACH:
					SetNewTargetPlace_Brahi(cptr, 2048.f);
					break;
				case AI_MOSA:
					SetNewTargetPlaceMosasaurus(cptr, 3048);
					break;
				case AI_FISH:
					SetNewTargetPlaceFish(cptr, 1024);
					break;
				}

			}
			else {
				SetNewTargetPlace(cptr, 2048);
			}

		}

		if (cptr->tgtime > 50 * 1000 && cptr->Clone == AI_ICTH) {
			if (cptr->Phase != DinoInfo[cptr->CType].flyAnim &&
				cptr->Phase != DinoInfo[cptr->CType].glideAnim &&
				cptr->Phase != DinoInfo[cptr->CType].takeoffAnim &&
				cptr->Phase != DinoInfo[cptr->CType].landAnim)
			{
				cptr->State = 2;
				cptr->AfraidTime = (50 + rRand(8)) * 1024;
				cptr->notFlushed = true;
			}
			else {
				SetNewTargetPlace_Icth(cptr, 2048);
			}
		}




		if (cptr->Health)
			if (cptr->BloodTTime)
			{
				cptr->BloodTTime -= TimeDt;
				if (cptr->BloodTTime < 0) cptr->BloodTTime = 0;

				float k = (20000.f + cptr->BloodTTime) / 90000.f;
				if (k > 1.5) k = 1.5;
				cptr->BloodTime += (int)((float)TimeDt * k);
				if (cptr->BloodTime > 600)
				{
					cptr->BloodTime = rRand(228);
					AddBloodTrail(cptr);
					if (rRand(128) > 96) AddBloodTrail(cptr);
				}
			}


		switch (cptr->Clone)
		{
//		case AI_MOSA:
//			AnimateMosa(cptr);
//			break;
		case AI_FISH:
			AnimateFish(cptr);
			break;
		case AI_BRACH:
			if (cptr->Health) AnimateBrahiOld(cptr);
			else AnimateDeadCommon(cptr);
			break;
		case AI_BRACHDANGER:
			if (cptr->Health) AnimateBrahi(cptr);
			else AnimateDeadCommon(cptr);
			break;
		case AI_LANDBRACH:
			if (cptr->Health) AnimateBrahi(cptr);
			else AnimateDeadCommon(cptr);
			break;
		case AI_ICTH:
			if (cptr->Health) AnimateIcth(cptr);
			else AnimateIcthDead(cptr);
			break;
		case AI_MOSH:
			if (cptr->Health) AnimateClassicAmbient(cptr);
			else AnimateDeadCommon(cptr);
			break;
		case AI_GALL:
			if (cptr->Health) AnimateClassicAmbient(cptr);
			else AnimateDeadCommon(cptr);
			break;
		case AI_DIMOR:
			if (cptr->Health) AnimateDimor(cptr);
			else AnimateDimorDead(cptr);
			break;
		case AI_PTERA:
			if (cptr->Health) AnimateDimor(cptr);
			else AnimateDimorDead(cptr);
			break;
		case AI_DIMET:
			if (cptr->Health) AnimateClassicAmbient(cptr);
			else AnimateDeadCommon(cptr);
			break;


		case AI_PARA:
			if (cptr->Health) AnimateHerbivore(cptr);
			else AnimateDeadCommon(cptr);
			break;
		case AI_ANKY:
			if (cptr->Health) AnimateHerbivore(cptr);
			else AnimateDeadCommon(cptr);
			break;
		case AI_PACH:
			if (cptr->Health) AnimateHerbivore(cptr);
			else AnimateDeadCommon(cptr);
			break;
		case AI_STEGO:
			if (cptr->Health) AnimateHerbivore(cptr);
			else AnimateDeadCommon(cptr);
			break;

		case AI_ALLO:
			if (cptr->Health) AnimateCarnivore(cptr);
			else AnimateDeadCommon(cptr);
			break;
		case AI_CHASM:
			if (cptr->Health) AnimateHerbivore(cptr);
			else AnimateDeadCommon(cptr);
			break;

		case AI_VELO:
			if (cptr->Health) AnimateCarnivore(cptr);
			else AnimateDeadCommon(cptr);
			break;
		case AI_SPINO:
			if (cptr->Health) AnimateCarnivore(cptr);
			else AnimateDeadCommon(cptr);
			break;
		case AI_CERAT:
			if (cptr->Health) AnimateCarnivore(cptr);
			else AnimateDeadCommon(cptr);
			break;
		case AI_TREX:
			if (cptr->Health) AnimateTRex(cptr);
			else AnimateTRexDead(cptr);
			break;

		case 0:
			AnimateHuntDead(cptr);
			break;
		}

	}
}




void MakeNoise(Vector3d pos, float range)
{
	for (int c = 0; c < ChCount; c++)
	{
		TCharacter *cptr = &Characters[c];
		if (!cptr->Health) continue;
		float l = VectorLength(SubVectors(cptr->pos, pos));
		if (l > range) continue;

		if (cptr->Clone == AI_TREX)  //===== T-Rex
			if (!cptr->State) cptr->State = 2;

		if (cptr->Clone != AI_TREX && !DinoInfo[cptr->CType].Aquatic)
		{
			cptr->AfraidTime = (int)(10.f + (range - l) / 256.f) * 1024;
			cptr->State = 2;
			cptr->NoFindCnt = 0;
		}
	}
}


void CheckAfraid()
{
	if (!MyHealth) return;
	if (TrophyMode) return;

	Vector3d ppos, plook, clook, wlook, rlook;
	ppos = PlayerPos;

	if (DEBUG || UNDERWATER || ObservMode) return;

	plook.y = 0;
	plook.x = (float)sin(CameraAlpha);
	plook.z = (float)-cos(CameraAlpha);

	wlook = Wind.nv;

	float kR, kwind, klook, kstand;

	float kmask = 1.0f;
	float kskill = 1.0f;
	float kscent = 1.0f;

	if (CamoMode)  kmask *= 1.5;
	if (ScentMode) kscent *= 1.5;

	for (int c = 0; c < ChCount; c++)
	{
		TCharacter *cptr = &Characters[c];
		if (!cptr->Health) continue;
		if (cptr->Clone < 10) continue;
		if (cptr->AfraidTime || cptr->State == 1) continue;

		rlook = SubVectors(ppos, cptr->pos);
		kR = VectorLength(rlook) / 256.f / (32.f + ctViewR / 2);
		NormVector(rlook, 1.0f);

		kR *= 2.5f / (float)(1.5 + OptSens / 128.f);
		if (kR > 3.0f) continue;

		clook.x = cptr->lookx;
		clook.y = 0;
		clook.z = cptr->lookz;

		MulVectorsScal(wlook, rlook, kwind);
		kwind *= Wind.speed / 10;
		MulVectorsScal(clook, rlook, klook);
		klook *= -1.f;

		if (HeadY > 180) kstand = 0.7f;
		else kstand = 1.2f;

		//============= reasons ==============//

		float kALook = kR * ((klook + 3.f) / 3.f) * kstand * kmask;
		if (klook > 0.3) kALook *= 2.0;
		if (klook > 0.8) kALook *= 2.0;
		kALook /= DinoInfo[cptr->CType].LookK;
		if (kALook < 1.0)
			if (TraceLook(cptr->pos.x, cptr->pos.y + 220, cptr->pos.z,
				PlayerX, PlayerY + HeadY / 2.f, PlayerZ)) kALook *= 1.3f;

		if (kALook < 1.0)
			if (TraceLook(cptr->pos.x, cptr->pos.y + 220, cptr->pos.z,
				PlayerX, PlayerY + HeadY, PlayerZ))   kALook = 2.0;
		kALook *= (1.f + (float)ObjectsOnLook / 6.f);

		/*
		  if (kR<1.0f) {
			  char t[32];
		   wsprintf(t,"%d", ObjectsOnLook);
		   AddMessage(t);
		   kALook = 20.f;
		  }
		  */

		float kASmell = kR * ((kwind + 2.0f) / 2.0F) * ((klook + 3.f) / 3.f) * kscent;
		if (kwind > 0) kASmell *= 2.0;
		kASmell /= DinoInfo[cptr->CType].SmellK;

		float kRes = MIN(kALook, kASmell);

		if (kRes < 1.0)
		{
			//MessageBeep(0xFFFFFFFF);
			char t[128];
			if (kALook < kASmell)
				sprintf(t, "LOOK: KR: %f  Tr: %d  K: %f", kR, ObjectsOnLook, kALook);
			else
				sprintf(t, "SMELL: KR: %f  Tr: %d  K: %f", kR, ObjectsOnLook, kASmell);
			//AddMessage(t);

			//MESSAGE REMOVED

			kRes = MIN(kRes, kR);
			cptr->AfraidTime = (int)(1.0 / (kRes + 0.1) * 10.f * 1000.f);
			cptr->State = 2;
			if (cptr->Clone == AI_TREX) //===== T-Rex
				if (kALook > kASmell) cptr->State = 3;
			cptr->NoFindCnt = 0;
		}
	}
}





void PlaceTrophy()
{
	ChCount = 0;

	for (int c = 0; c < 24; c++)
	{
		if (!TrophyRoom.Body[c].ctype) continue;
		//Characters[ChCount].CType = TrophyRoom.Body[c].ctype;
		Characters[ChCount].CType = TrophyIndex[TrophyRoom.Body[c].ctype];

		if (c < 6) Characters[ChCount].alpha = pi / 2;
		else if (c < 12) Characters[ChCount].alpha = pi;
		else if (c < 18) Characters[ChCount].alpha = pi * 3 / 2;
		else
			Characters[ChCount].alpha = 0;

		ResetCharacter(&Characters[ChCount]);

		Characters[ChCount].State = c;
		Characters[ChCount].scale = TrophyRoom.Body[c].scale;
		Characters[ChCount].pos.x = LandingList.list[c].x * 256.f + 128.f;
		Characters[ChCount].pos.z = LandingList.list[c].y * 256.f + 128.f;

		Characters[ChCount].pos.y = GetLandH(Characters[ChCount].pos.x,
			Characters[ChCount].pos.z);
		ChCount++;
	}
}


















void spawnPositionPackLeader(int RegionNo) {
	Characters[ChCount].pos.x = Region[RegionNo].XMin * 256
		+ abs(rRand(Region[RegionNo].XMax - Region[RegionNo].XMin) * 256);

	Characters[ChCount].pos.z = Region[RegionNo].YMin * 256
		+ abs(rRand(Region[RegionNo].YMax - Region[RegionNo].YMin) * 256);
}

void spawnPositionPackFollower(int RegionNo, int leader) {
	//spawn on same spot, so the pack doesn't spawn over a cliff edge or something
	Characters[ChCount].pos.x = Characters[leader].pos.x;
	Characters[ChCount].pos.z = Characters[leader].pos.z;
}

void spawnMapAmbient(int DinoInfoIndex, int RegionNo, int &tr, int leader) {


	Characters[ChCount].CType = DinoInfoIndex; //i9 + 1];
replaceSMA:

	if (leader >= 0) {
		spawnPositionPackFollower(RegionNo, leader);
	}
	else {
		spawnPositionPackLeader(RegionNo);
	}

	Characters[ChCount].pos.y = GetLandH(Characters[ChCount].pos.x,
		Characters[ChCount].pos.z);
	float wy = GetLandUpH(Characters[ChCount].pos.x,
		Characters[ChCount].pos.z) - Characters[ChCount].pos.y;
	tr++;
	if (tr > 10240) return;

	if (DinoInfo[DinoInfoIndex].Clone == AI_BRACH ||
		DinoInfo[DinoInfoIndex].Clone == AI_BRACHDANGER ||
		DinoInfo[DinoInfoIndex].Clone == AI_ICTH) {

		if (wy > 380) goto replaceSMA;
		if (wy < 220) goto replaceSMA;

	}

	if (DinoInfo[DinoInfoIndex].Clone == AI_FISH ||
		DinoInfo[DinoInfoIndex].Clone == AI_MOSA) {
		if (wy < DinoInfo[DinoInfoIndex].minDepth) goto replaceSMA;
		if (wy > DinoInfo[DinoInfoIndex].maxDepth) goto replaceSMA;
	}


	if (DinoInfo[DinoInfoIndex].AvoidCount)
	{
		for (int i = 0; i < DinoInfo[DinoInfoIndex].AvoidCount; i++) {
			if (Characters[ChCount].pos.x < (Avoid[DinoInfo[DinoInfoIndex].Avoidances[i]].XMax + 1) * 256 &&
				Characters[ChCount].pos.z < (Avoid[DinoInfo[DinoInfoIndex].Avoidances[i]].YMax + 1) * 256 &&
				Characters[ChCount].pos.x >(Avoid[DinoInfo[DinoInfoIndex].Avoidances[i]].XMin - 1) * 256 &&
				Characters[ChCount].pos.z >(Avoid[DinoInfo[DinoInfoIndex].Avoidances[i]].YMin - 1) * 256) goto replaceSMA;
		}
	}

	if (DinoInfo[DinoInfoIndex].Clone >= 0 || DinoInfo[DinoInfoIndex].Clone == AI_LANDBRACH) {
		if (fabs(Characters[ChCount].pos.x - PlayerX) +
			fabs(Characters[ChCount].pos.z - PlayerZ) < 256 * 40)
			goto replaceSMA;

		if (CheckPlaceCollisionP(Characters[ChCount].pos)) goto replaceSMA;
	}

	Characters[ChCount].tgx = Characters[ChCount].pos.x;
	Characters[ChCount].tgz = Characters[ChCount].pos.z;
	Characters[ChCount].tgtime = 0;

	if (DinoInfo[DinoInfoIndex].Clone == AI_BRACH ||
		DinoInfo[DinoInfoIndex].Clone == AI_BRACHDANGER ||
		DinoInfo[DinoInfoIndex].Clone == AI_LANDBRACH ||
		DinoInfo[DinoInfoIndex].Clone == AI_ICTH) {

		Characters[ChCount].spawnAlt = GetLandUpH(Characters[ChCount].pos.x,
			Characters[ChCount].pos.z);

	}

	if (DinoInfo[DinoInfoIndex].Clone == AI_FISH ||
		DinoInfo[DinoInfoIndex].Clone == AI_MOSA) {

		Characters[ChCount].depth = GetLandH(Characters[ChCount].tgx, Characters[ChCount].tgz) +
			((GetLandUpH(Characters[ChCount].tgx, Characters[ChCount].tgz) - GetLandH(Characters[ChCount].tgx, Characters[ChCount].tgz) / 2)
				);
		Characters[ChCount].tdepth = Characters[ChCount].depth;

	}

	Characters[ChCount].RType = RegionNo;
	ResetCharacter(&Characters[ChCount]);
	ChCount++;


}



















void PlaceCharacters()
{
	int c, tr;
	ChCount = 0;
	PrintLog("Placing...");

	for (c = 10; c < 30; c++)
		if ((TargetDino & (1 << c)) > 0)
		{
			TargetCall = c;
			break;
		}


	//======== lohs =========//

	int MC = 5 + OptDens / 80;
	if (OptDayNight == 2) MC /= 2;

	tr = 0;

	// NO PACKING HUNTING FOR CLASSIC AMBS
	for (c = 0; c < MC; c++)
	{
		Characters[ChCount].CType = 1 + c % 5; //rRand(3);
	replace1:
		Characters[ChCount].pos.x = PlayerX + siRand(10040);
		Characters[ChCount].pos.z = PlayerZ + siRand(10040);
		Characters[ChCount].pos.y = GetLandH(Characters[ChCount].pos.x,
			Characters[ChCount].pos.z);
		tr++;
		if (tr > 10240) break;

		if (CheckPlaceCollisionP(Characters[ChCount].pos)) goto replace1;

		ResetCharacter(&Characters[ChCount]);

		if (Characters[ChCount].Clone == AI_DIMET ||
			Characters[ChCount].Clone == AI_PTERA)
			Characters[ChCount].pos.y += 2048.f;

		Characters[ChCount].tgx = Characters[ChCount].pos.x;
		Characters[ChCount].tgz = Characters[ChCount].pos.z;
		Characters[ChCount].tgtime = 0;


		ChCount++;
	}


	//MAP AMBIENTS


	for (int i9 = 0; i9 < TotalMA; i9++) {

		int DinoInfoIndex = AI_to_CIndex[i9 + AI_FINAL + 1];

		for (int r = 0; r < DinoInfo[DinoInfoIndex].RegionCount; r++) {

			int RegionNo = DinoInfo[DinoInfoIndex].RType0[r];

			//spawn count
			int spawnNo = Region[RegionNo].SpawnMin;
			for (int i = 0; i < Region[RegionNo].SpawnMax - Region[RegionNo].SpawnMin; i++) {
				if (Region[RegionNo].SpawnRate * 1000 > rRand(1000)) spawnNo++;
			}

			tr = 0;
			for (int spawnN = 0; spawnN < spawnNo; spawnN++)
			{

				int leaderIndex = ChCount;
				// pack leaders
				spawnMapAmbient(DinoInfoIndex, RegionNo, tr, -1);

				//pack size
				int packNo = 1;
				if (DinoInfo[DinoInfoIndex].packMax > 1) {
					packNo = DinoInfo[DinoInfoIndex].packMin;
					for (int i = 0; i < DinoInfo[DinoInfoIndex].packMax - DinoInfo[DinoInfoIndex].packMin; i++) {
						if (1 == rRand(2)) packNo++;
					}
				}

				//pack members
				if (packNo > 1) {
					Packs[PackCount].leader = &Characters[leaderIndex];
					Packs[PackCount].alert = FALSE;
					Packs[PackCount].attack = FALSE;
					Packs[PackCount]._alert = FALSE;
					Packs[PackCount]._attack = FALSE;
					Characters[leaderIndex].pack = PackCount;
					for (int packN = 0; packN < packNo - 1; packN++) {
						Characters[ChCount].pack = PackCount;
						spawnMapAmbient(DinoInfoIndex, RegionNo, tr, leaderIndex);
					}
					PackCount++;
				}
				else Characters[leaderIndex].pack = -1;

				if (tr > 10500) break;

			}


		}


	}




	int TC = 0;
	int TDi[10];
	TDi[0] = 10;
	for (c = 10; c < 20; c++)
		if (TargetDino & (1 << c)) TDi[TC++] = c;


	MC = 8 + OptDens / 30 + rRand(6);
	tr = 0;

	//======== main =========//
	for (c = 0; c < MC; c++)
	{

		if ((c < 4) || (!TargetDino)) Characters[ChCount].CType = AI_to_CIndex[10] + rRand(6);
		else
			//if (c<10)
			Characters[ChCount].CType = AI_to_CIndex[TDi[c % (TC)]];
		//else Characters[ChCount].CType = AI_to_CIndex[ TDi[rRand(TC-1)] ];

		//Characters[ChCount].CType = AI_to_CIndex[10] + 7;//rRand(3);
	replace2:
		Characters[ChCount].pos.x = 512 * 256 + siRand(50 * 256) * 10;
		Characters[ChCount].pos.z = 512 * 256 + siRand(50 * 256) * 10;
		Characters[ChCount].pos.y = GetLandH(Characters[ChCount].pos.x,
			Characters[ChCount].pos.z);
		tr++;
		if (tr > 10240) break;

		if (fabs(Characters[ChCount].pos.x - PlayerX) +
			fabs(Characters[ChCount].pos.z - PlayerZ) < 256 * 40)
			goto replace2;

		if (CheckPlaceCollisionP(Characters[ChCount].pos)) goto replace2;

		Characters[ChCount].tgx = Characters[ChCount].pos.x;
		Characters[ChCount].tgz = Characters[ChCount].pos.z;
		Characters[ChCount].tgtime = 0;

		ResetCharacter(&Characters[ChCount]);
		ChCount++;
	}

	PrintLog("\n");
	DemoPoint.DemoTime = 0;
}






void CreateChMorphedModel(TCharacter *cptr)
{
	TAni *aptr = &cptr->pinfo->Animation[cptr->Phase];
	TAni *paptr = &cptr->pinfo->Animation[cptr->PrevPhase];

	int CurFrame, SplineD, PCurFrame, PSplineD;
	float scale = cptr->scale;

	CurFrame = ((aptr->FramesCount - 1) * cptr->FTime * 256) / aptr->AniTime;
	SplineD = CurFrame & 0xFF;
	CurFrame = (CurFrame >> 8);


	BOOL PMorph = (cptr->Phase != cptr->PrevPhase) && (cptr->PPMorphTime < PMORPHTIME) && (MORPHP);

	if (PMorph)
	{
		PCurFrame = ((paptr->FramesCount - 1) * cptr->PrevPFTime * 256) / paptr->AniTime;
		PSplineD = PCurFrame & 0xFF;
		PCurFrame = (PCurFrame >> 8);
	}



	if (!MORPHA)
	{
		SplineD = 0;
		PSplineD = 0;
	}

	float k1, k2, pk1, pk2, pmk1, pmk2;

	k2 = (float)(SplineD) / 256.f;
	k1 = 1.0f - k2;
	k1 /= 8.f;
	k2 /= 8.f;

	if (PMorph)
	{
		pk2 = (float)(PSplineD) / 256.f;
		pk1 = 1.0f - pk2;
		pk1 /= 8.f;
		pk2 /= 8.f;
		pmk1 = (float)cptr->PPMorphTime / PMORPHTIME;
		pmk2 = 1.f - pmk1;
	}

	int VCount = cptr->pinfo->mptr->VCount;
	short int* adptr = aptr->aniData + CurFrame * VCount * 3;
	short int* padptr = paptr->aniData + PCurFrame * VCount * 3;

	float sb = (float)sin(cptr->beta) * scale;
	float cb = (float)cos(cptr->beta) * scale;
	float sg = (float)sin(cptr->gamma);
	float cg = (float)cos(cptr->gamma);

	for (int v = 0; v < VCount; v++)
	{
		float x = *(adptr + v * 3 + 0) * k1 + *(adptr + (v + VCount) * 3 + 0) * k2;
		float y = *(adptr + v * 3 + 1) * k1 + *(adptr + (v + VCount) * 3 + 1) * k2;
		float z = -(*(adptr + v * 3 + 2) * k1 + *(adptr + (v + VCount) * 3 + 2) * k2);

		if (PMorph)
		{
			float px = *(padptr + v * 3 + 0) * pk1 + *(padptr + (v + VCount) * 3 + 0) * pk2;
			float py = *(padptr + v * 3 + 1) * pk1 + *(padptr + (v + VCount) * 3 + 1) * pk2;
			float pz = -(*(padptr + v * 3 + 2) * pk1 + *(padptr + (v + VCount) * 3 + 2) * pk2);

			x = x * pmk1 + px * pmk2;
			y = y * pmk1 + py * pmk2;
			z = z * pmk1 + pz * pmk2;
		}


		float zz = z;
		float xx = cg * x - sg * y;
		float yy = cg * y + sg * x;


		//float fi = (z / 400) * (cptr->bend / 1.5f);
		float fi;
		if (z > 0)
		{
			fi = z / 240.f;
			if (fi > 1.f) fi = 1.f;
		}
		else
		{
			fi = z / 380.f;
			if (fi < -1.f) fi = -1.f;
		}

		float fiMosa = fi * cptr->bdepth;
		fi *= cptr->bend;

		float bendc = (float)cos(fi);
		float bends = (float)sin(fi);

		float bx;
		float bz;
		float by;

		bx = bendc * xx - bends * zz;
		bz = bendc * zz + bends * xx;
		zz = bz;
		xx = bx;

		if (DinoInfo[cptr->CType].Aquatic) {
			float bendcmosa = (float)cos(fiMosa);
			float bendsmosa = (float)sin(fiMosa);
			bz = bendcmosa * zz + bendsmosa * yy;
			by = bendcmosa * yy + bendsmosa * zz;
			yy = by;
			zz = bz;
		}

		cptr->pinfo->mptr->gVertex[v].x = xx * scale;
		cptr->pinfo->mptr->gVertex[v].y = cb * yy - sb * zz;
		cptr->pinfo->mptr->gVertex[v].z = cb * zz + sb * yy;
	}
}







/*
void CreateChMorphedModel(TCharacter *cptr)
{
  TAni *aptr  = &cptr->pinfo->Animation[cptr->Phase];
  TAni *paptr = &cptr->pinfo->Animation[cptr->PrevPhase];

  int CurFrame, SplineD, PCurFrame, PSplineD;
  float scale = cptr->scale;

  CurFrame = ( (aptr->FramesCount-1) * cptr->FTime * 256) / aptr->AniTime;
  SplineD = CurFrame & 0xFF;
  CurFrame = (CurFrame>>8);


  BOOL PMorph = (cptr->Phase != cptr->PrevPhase) && (cptr->PPMorphTime < PMORPHTIME) && (MORPHP);

  if (PMorph)
  {
	PCurFrame = ( (paptr->FramesCount-1) * cptr->PrevPFTime * 256) / paptr->AniTime;
	PSplineD = PCurFrame & 0xFF;
	PCurFrame = (PCurFrame>>8);
  }



  if (!MORPHA)
  {
	SplineD = 0;
	PSplineD = 0;
  }

  float k1, k2, pk1, pk2, pmk1, pmk2;

  k2 = (float)(SplineD) / 256.f;
  k1 = 1.0f - k2;
  k1/=8.f;
  k2/=8.f;

  if (PMorph)
  {
	pk2 = (float)(PSplineD) / 256.f;
	pk1 = 1.0f - pk2;
	pk1/=8.f;
	pk2/=8.f;
	pmk1 = (float)cptr->PPMorphTime / PMORPHTIME;
	pmk2 = 1.f - pmk1;
  }

  int VCount = cptr->pinfo->mptr->VCount;
  short int* adptr  =  aptr->aniData + CurFrame*VCount*3;
  short int* padptr = paptr->aniData + PCurFrame*VCount*3;

  float sb = (float)sin(cptr->beta) * scale;
  float cb = (float)cos(cptr->beta) * scale;
  float sg = (float)sin(cptr->gamma);
  float cg = (float)cos(cptr->gamma);

  for (int v=0; v<VCount; v++)
  {
	float x = *(adptr+v*3+0) * k1 + *(adptr+(v+VCount)*3+0) * k2;
	float y = *(adptr+v*3+1) * k1 + *(adptr+(v+VCount)*3+1) * k2;
	float z = - (*(adptr+v*3+2) * k1 + *(adptr+(v+VCount)*3+2) * k2);

	if (PMorph)
	{
	  float px = *(padptr+v*3+0) * pk1 + *(padptr+(v+VCount)*3+0) * pk2;
	  float py = *(padptr+v*3+1) * pk1 + *(padptr+(v+VCount)*3+1) * pk2;
	  float pz = - (*(padptr+v*3+2) * pk1 + *(padptr+(v+VCount)*3+2) * pk2);

	  x = x*pmk1 + px* pmk2;
	  y = y*pmk1 + py* pmk2;
	  z = z*pmk1 + pz* pmk2;
	}


	float zz = z;
	float xx = cg * x - sg * y;
	float yy = cg * y + sg * x;


	//float fi = (z / 400) * (cptr->bend / 1.5f);
	float fi;
	if (z>0)
	{
	  fi = z / 240.f;
	  if (fi>1.f) fi=1.f;
	}
	else
	{
	  fi = z / 380.f;
	  if (fi<-1.f) fi=-1.f;
	}

	fi*=cptr->bend;

	float bendc = (float)cos(fi);
	float bends = (float)sin(fi);

	float bx = bendc * xx - bends * zz;
	float bz = bendc * zz + bends * xx;
	zz = bz;
	xx = bx;

	cptr->pinfo->mptr->gVertex[v].x = xx * scale;
	cptr->pinfo->mptr->gVertex[v].y = cb * yy - sb * zz;
	cptr->pinfo->mptr->gVertex[v].z = cb * zz + sb * yy;
  }
}
*/


void CreateMorphedModel(TModel* mptr, TAni *aptr, int FTime, float scale)
{
	int CurFrame = ((aptr->FramesCount - 1) * FTime * 256) / aptr->AniTime;

	int SplineD = CurFrame & 0xFF;
	CurFrame = (CurFrame >> 8);

	float k2 = (float)(SplineD) / 256.f;
	float k1 = 1.0f - k2;
	k1 *= scale / 8.f;
	k2 *= scale / 8.f;

	int VCount = mptr->VCount;
	short int* adptr = &(aptr->aniData[CurFrame*VCount * 3]);
	for (int v = 0; v < VCount; v++)
	{
		mptr->gVertex[v].x = *(adptr + v * 3 + 0) * k1 + *(adptr + (v + VCount) * 3 + 0) * k2;
		mptr->gVertex[v].y = *(adptr + v * 3 + 1) * k1 + *(adptr + (v + VCount) * 3 + 1) * k2;
		mptr->gVertex[v].z = -*(adptr + v * 3 + 2) * k1 - *(adptr + (v + VCount) * 3 + 2) * k2;
	}
}




void CreateMorphedObject(TModel* mptr, TVTL &vtl, int FTime)
{
	int CurFrame = ((vtl.FramesCount - 1) * FTime * 256) / vtl.AniTime;

	int SplineD = CurFrame & 0xFF;
	CurFrame = (CurFrame >> 8);

	float k2 = (float)(SplineD) / 256.f;
	float k1 = 1.0f - k2;
	k1 /= 8.f;
	k2 /= 8.f;

	int VCount = mptr->VCount;
	short int* adptr = &(vtl.aniData[CurFrame*VCount * 3]);
	for (int v = 0; v < VCount; v++)
	{
		mptr->gVertex[v].x = *(adptr + v * 3 + 0) * k1 + *(adptr + (v + VCount) * 3 + 0) * k2;
		mptr->gVertex[v].y = *(adptr + v * 3 + 1) * k1 + *(adptr + (v + VCount) * 3 + 1) * k2;
		mptr->gVertex[v].z = -*(adptr + v * 3 + 2) * k1 - *(adptr + (v + VCount) * 3 + 2) * k2;
	}
}