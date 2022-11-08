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

	cptr->awareHunter = FALSE;

	if (DinoInfo[cptr->CType].killTypeCount > 1) {
		cptr->killType = rRand(DinoInfo[cptr->CType].killTypeCount - 1);
	}

	if (DinoInfo[cptr->CType].roarCount > 0) {
		cptr->roarAnim = DinoInfo[cptr->CType].roarAnim[rRand(DinoInfo[cptr->CType].roarCount - 1)];
	}

	if (DinoInfo[cptr->CType].deathTypeCount > 1) {
		cptr->deathType = rRand(DinoInfo[cptr->CType].deathTypeCount - 1);
	}

	if (DinoInfo[cptr->CType].waterDieCount > 0) {
		cptr->waterDieAnim = DinoInfo[cptr->CType].waterDieAnim[rRand(DinoInfo[cptr->CType].waterDieCount - 1)];
	}

	cptr->speed_run = DinoInfo[cptr->CType].runspd;
	cptr->speed_walk = DinoInfo[cptr->CType].wlkspd;
	cptr->speed_jump = DinoInfo[cptr->CType].jmpspd;
	cptr->speed_swim = DinoInfo[cptr->CType].swmspd;
	cptr->speed_fly = DinoInfo[cptr->CType].flyspd;
	cptr->speed_glide = DinoInfo[cptr->CType].gldspd;
	cptr->speed_takeoff = DinoInfo[cptr->CType].tkfspd;
	cptr->speed_land = DinoInfo[cptr->CType].lndspd;
	cptr->speed_dive = DinoInfo[cptr->CType].divspd;

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

	cptr->aquaticIdle = false;

	cptr->spcDepth = DinoInfo[cptr->CType].spacingDepth + (cptr->scale * 500) - 500;


}


void AddDeadBody(TCharacter *cptr, int phase, bool scream)
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
	if (phase != HUNT_BREATH && scream){
		AddVoicev(fxScream[r].length, fxScream[r].lpData, 256);
	}

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
		/*
		if (DinoInfo[cptr->CType].Aquatic) {
			Characters[ChCount].pos.x = cptr->pos.x + cptr->lookx * pl * cptr->scale * (float)cos(cptr->beta);
			Characters[ChCount].pos.z = cptr->pos.z + cptr->lookz * pl * cptr->scale * (float)cos(cptr->beta);
			Characters[ChCount].pos.y = cptr->pos.y - (float)sin(cptr->beta) * pl * cptr->scale;
			float ply = DinoInfo[cptr->CType].killType[cptr->killType].yoffset;
			Characters[ChCount].pos.y += ply * (float)cos(cptr->beta);
			ply *= (float)sin(cptr->beta);
			Characters[ChCount].pos.z += ply * (float)sin(cptr->alpha);
			Characters[ChCount].pos.x += ply * (float)cos(cptr->alpha);
			Characters[ChCount].alpha = cptr->alpha;
			Characters[ChCount].beta = cptr->beta;
			Characters[ChCount].gamma = cptr->gamma;

		}
		*/
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


//OLD
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

/*
bool jumpCollision(TCharacter *cptr, Vector3d &v, BOOL wc, BOOL mc)
{
	Vector3d p = cptr->pos;
	float lookx = (float)cos(cptr->tgalpha);
	float lookz = (float)sin(cptr->tgalpha);
	for (int i = 0; i < 10; i++) {

		p.x += lookx * 64.f;
		p.z += lookz * 64.f;
		if (CheckPlaceCollision(cptr, p, wc, mc)) {
			return false;
		};
	}
	return true;
}
*/


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
		if ((GetLandUpH(v.x, v.z) - GetLandH(v.x, v.z)) > DinoInfo[cptr->CType].waterLevel ||
			(GetLandUpH(v.x + 256, v.z) - GetLandH(v.x + 256, v.z)) > DinoInfo[cptr->CType].waterLevel ||
			(GetLandUpH(v.x, v.z + 256) - GetLandH(v.x, v.z + 256)) > DinoInfo[cptr->CType].waterLevel ||
			(GetLandUpH(v.x + 256, v.z + 256) - GetLandH(v.x + 256, v.z + 256)) > DinoInfo[cptr->CType].waterLevel ||
			(GetLandUpH(v.x - 256, v.z) - GetLandH(v.x - 256, v.z)) > DinoInfo[cptr->CType].waterLevel ||
			(GetLandUpH(v.x, v.z - 256) - GetLandH(v.x, v.z - 256)) > DinoInfo[cptr->CType].waterLevel ||
			(GetLandUpH(v.x - 256, v.z - 256) - GetLandH(v.x - 256, v.z - 256)) > DinoInfo[cptr->CType].waterLevel ||
			(GetLandUpH(v.x + 256, v.z - 256) - GetLandH(v.x + 256, v.z - 256)) > DinoInfo[cptr->CType].waterLevel ||
			(GetLandUpH(v.x - 256, v.z + 256) - GetLandH(v.x - 256, v.z + 256)) > DinoInfo[cptr->CType].waterLevel) return 1;
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
		/*else if ((cptr->Clone == AI_MOSA)) {
			p.x += lookx * 64.f;
			p.z += lookz * 64.f;
			if (CheckPlaceCollisionMosasaurus(cptr, p, cptr->depth)) c++;

		}*/
		else if (cptr->Clone == AI_FISH ||
			cptr->Clone == AI_MOSA) {
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

	Characters[ChCount].packId = -1;	//Classic ambient- no pack hunting
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


//old
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

	if (cptr->AI < 0) {//STILL NEED THIS FOR CLASSIC AMBIENTS
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

	/*
	p.x = cptr->pos.x + siRand((int)(R/3));
	p.z = cptr->pos.z + siRand((int)(R/3));
	if (p.x > cptr->pos.x) p.x += R * (2 / 3); else p.x -= R * (2 / 3);
	if (p.z > cptr->pos.z) p.z += R * (2 / 3); else p.z -= R * (2 / 3);
	*/

	p.x = cptr->pos.x + siRand((int)(R));
	p.z = cptr->pos.z + siRand((int)(R));

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
		if (fabs(p.x - cptr->pos.x) + fabs(p.z - cptr->pos.z) < R * 0.7) goto replace;

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


	int spcdm = 1;
	if (cptr->aquaticIdle) spcdm = 0.75;

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
		if (wy < cptr->spcDepth * spcdm * 2) {
			if (stayRegion && outsideRegion && tr > 64) stayRegion = false;
			goto replace;
		}

	}

	float targetDepthTemp;

	/*
	if (cptr->aquaticIdle) {
		targetDepthTemp = GetLandUpH(p.x, p.z) - (cptr->spcDepth * 0.75);
		goto skipY;
	}
	*/

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
	//targetDepthTemp = siRand((int)(R/3));

	if (cptr->aquaticIdle) {
		targetDepthTemp = rRand((int)(GetLandUpH(p.x, p.z) - (cptr->spcDepth * 0.68) - cptr->depth)); //target slightly higher so it doesn't take forever - correct to 0.75 later
	} else {
		targetDepthTemp = siRand((int)(tdistTemp));
	}

	tr++;

	/*
	if (cptr->aquaticIdle) {
		if (targetDepthTemp < 0) targetDepthTemp *= -1;
		if (cptr->depth > GetLandUpH(p.x, p.z) - (cptr->spcDepth * 1.1)) {
			targetDepthTemp = GetLandUpH(p.x, p.z) - (cptr->spcDepth * 0.75);
			goto skipY;
		}
	}
	*/

	//PREVENT TOO MUCH TURNING/bending
	if (tr < 1024) {
		float tbeta = -atan((targetDepthTemp) / tdistTemp);
		int dbeta = tbeta - cptr->beta;
		if (dbeta < 0) dbeta *= -1;
		if (dbeta > pi / 16) {
			goto replace2;
		}
	}

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
	

	if (targetDepthTemp < GetLandH(p.x, p.z) + (cptr->spcDepth * spcdm)) {
		if (tr < 3024) {
			goto replace2;
		}
		else {
			targetDepthTemp = GetLandH(p.x, p.z) + (cptr->spcDepth * spcdm);
		}
	}
	if (targetDepthTemp > GetLandUpH(p.x, p.z) - (cptr->spcDepth * spcdm)) {
		if (tr < 3024) {
			goto replace2;
		}
		else {
			targetDepthTemp = GetLandUpH(p.x, p.z) - (cptr->spcDepth * spcdm);
		}
	}

//skipY:

	cptr->tgtime = 0;
	cptr->tgx = p.x;
	cptr->tgz = p.z;
	cptr->tdepth = targetDepthTemp;
	cptr->lastTBeta = cptr->beta;
	cptr->turny = 0;
}


//OLD
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

	//PREVENT TOO MUCH TURNING
	if (tr < 1024) {
		float tbeta = -atan((targetDepthTemp) / tdistTemp);
		int dbeta = tbeta - cptr->beta;
		if (dbeta < 0) dbeta *= -1;
		if (dbeta > pi/8) {
			goto replace2;
		}
	}
	

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
	bool loopDone = FALSE;

	if (killerDino) {
		if (DinoInfo[killerDino->CType].killType[killerDino->killType].carryCorpse &&
			DinoInfo[killerDino->CType].Aquatic) {
			cptr->bend = killerDino->bend;
			cptr->bdepth = killerDino->bdepth;
		}
	}



	cptr->FTime += TimeDt;
	if (cptr->FTime >= cptr->pinfo->Animation[cptr->Phase].AniTime)
	{
		if (killerDino) {
			if (DinoInfo[killerDino->CType].killType[killerDino->killType].dontloop &&
				cptr->Phase == DinoInfo[killerDino->CType].killType[killerDino->killType].hunteranim) {
				loopDone = true;
			}
		}

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

	bool notAq = false;
	if (!killerDino) {
		notAq = true;
	} else if (!DinoInfo[killerDino->CType].Aquatic) notAq = true;

	if (notAq) {
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
	}
	

	if (killerDino) {

		//	if (!(GetLandUpH(killerDino->pos.x, killerDino->pos.z) - GetLandH(killerDino->pos.x, killerDino->pos.z) >
		//		DinoInfo[killerDino->CType].waterLevel * killerDino->scale))
		if (!killedwater || DinoInfo[killerDino->CType].Aquatic) //make exception for aquatic dangerous creatures duh
		{
			if (DinoInfo[killerDino->CType].killTypeCount) {
				if ((DinoInfo[killerDino->CType].killType[killerDino->killType].elevate &&
					killerDino->Phase == DinoInfo[killerDino->CType].killType[killerDino->killType].anim)
					|| DinoInfo[killerDino->CType].killType[killerDino->killType].carryCorpse
					) {

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

		if (loopDone) {
			if (DinoInfo[killerDino->CType].killType[killerDino->killType].carryCorpse
				&& (!killedwater || DinoInfo[killerDino->CType].Aquatic)) {
				cptr->Phase = DinoInfo[killerDino->CType].killType[killerDino->killType].hunterswimanim;
			}
			else {
				cptr->FTime = cptr->pinfo->Animation[cptr->Phase].AniTime - 1;
			}
		}

	}

}



void AnimateTRexDead(TCharacter *cptr)
{

	if (cptr->Phase != DinoInfo[cptr->CType].deathType[cptr->deathType].die)
	{
		if (cptr->PPMorphTime > 128)
		{
			cptr->PrevPhase = cptr->Phase;
			cptr->PrevPFTime = cptr->FTime;
			cptr->PPMorphTime = 0;
		}

		cptr->FTime = 0;
		cptr->Phase = DinoInfo[cptr->CType].deathType[cptr->deathType].die;
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


/*
void AnimateDimorDead(TCharacter *cptr)
{

	if (cptr->Phase != DinoInfo[cptr->CType].fallAnim && cptr->Phase != DinoInfo[cptr->CType].deathType[cptr->deathType].die)
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
		if (cptr->Phase == DinoInfo[cptr->CType].deathType[cptr->deathType].die)
			cptr->FTime = cptr->pinfo->Animation[cptr->Phase].AniTime - 1;
		else
			cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;


	//======= movement ===========//
	if (cptr->Phase == DinoInfo[cptr->CType].deathType[cptr->deathType].die)
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

			cptr->Phase = DinoInfo[cptr->CType].deathType[cptr->deathType].die;
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
*/


//universal animate dead proc
void AnimateDeadCommon(TCharacter *cptr)
{

	if (cptr->Phase != DinoInfo[cptr->CType].deathType[cptr->deathType].die && cptr->Phase != DinoInfo[cptr->CType].deathType[cptr->deathType].sleep)
	{
		if (cptr->PPMorphTime > 128)
		{
			cptr->PrevPhase = cptr->Phase;
			cptr->PrevPFTime = cptr->FTime;
			cptr->PPMorphTime = 0;
		}

		cptr->FTime = 0;
		cptr->Phase = DinoInfo[cptr->CType].deathType[cptr->deathType].die;
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
				cptr->Phase = DinoInfo[cptr->CType].deathType[cptr->deathType].sleep;
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





void AnimateTitan(TCharacter *cptr)
{
	NewPhase = FALSE;
	int _Phase = cptr->Phase;
	int _FTime = cptr->FTime;
	float _tgalpha = cptr->tgalpha;

	bool alertInit = FALSE;
	if (cptr->State == 2) alertInit = TRUE;
	if (cptr->packId >= 0) {
		if (!cptr->State && Packs[cptr->packId]._alert) alertInit = TRUE;
	}



	if (alertInit)
	{
		cptr->State = 1;
		if (cptr->gliding) cptr->Phase = DinoInfo[cptr->CType].flyAnim;
		else cptr->Phase = DinoInfo[cptr->CType].runAnim;
	}

TBEGIN:
	float targetx = cptr->tgx;
	float targetz = cptr->tgz;
	float targetdx = targetx - cptr->pos.x;
	float targetdz = targetz - cptr->pos.z;

	float tdist = (float)sqrt(targetdx * targetdx + targetdz * targetdz);

	float playerdx, playerdz;
	playerdx = PlayerX - cptr->pos.x - cptr->lookx * 108;
	playerdz = PlayerZ - cptr->pos.z - cptr->lookz * 108;
	float pdist = (float)sqrt(playerdx * playerdx + playerdz * playerdz);




	if (GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z) > DinoInfo[cptr->CType].waterLevel * cptr->scale)
		cptr->StateF |= csONWATER;
	else
		cptr->StateF &= (!csONWATER);

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) goto NOTHINK;

	//============================================//			// (run away)
	if (!MyHealth) cptr->State = 0;
	bool fleeMode = FALSE;
	if (cptr->State)
	{
		float aDist;
		aDist = ctViewR * DinoInfo[cptr->CType].aggress + OptAgres / AIInfo[cptr->Clone].agressMulti;


		if (pdist > aDist || ((PlayerY - cptr->pos.y > pdist) && cptr->gliding)  ||
			DinoInfo[cptr->CType].aggress <= 0 || !cptr->awareHunter) {
			fleeMode = TRUE;
		}
		else if (cptr->packId >= 0) Packs[cptr->packId].attack = TRUE;

		if (cptr->packId >= 0) {
			if (Packs[cptr->packId]._attack) fleeMode = FALSE;
		}

		if (fleeMode) {
			nv.x = playerdx;
			nv.z = playerdz;
			nv.y = 0;
			NormVector(nv, 2048.f);
			cptr->tgx = cptr->pos.x - nv.x;
			cptr->tgz = cptr->pos.z - nv.z;
			cptr->tgtime = 0;
			cptr->AfraidTime -= TimeDt;

			if (cptr->packId >= 0) {
				if (cptr->AfraidTime <= 0)
				{
					if (!Packs[cptr->packId]._alert) {
						cptr->AfraidTime = 0;
						cptr->State = 0;
					}
				}
				else Packs[cptr->packId].alert = true;
			}
			else if (cptr->AfraidTime <= 0) {
				cptr->AfraidTime = 0;
				cptr->State = 0;
			}

		}
		else
		{
			cptr->tgx = PlayerX;
			cptr->tgz = PlayerZ;
			cptr->tgtime = 0;
			if (cptr->packId >= 0) {
				Packs[cptr->packId].alert = true;
			}
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
						if (DinoInfo[cptr->CType].killType[cptr->killType].dontloop) cptr->FTime = 0;
						AddDeadBody(cptr,
							DinoInfo[cptr->CType].killType[cptr->killType].hunteranim,
							DinoInfo[cptr->CType].killType[cptr->killType].scream);
					}
					else AddDeadBody(cptr, HUNT_EAT, TRUE);

				}
				else {
					AddDeadBody(cptr, HUNT_EAT, TRUE);
					cptr->State = 0;
				}

			}
		}


	}

	if (!cptr->State)
	{
		cptr->AfraidTime = 0;

		if (cptr->packId >= 0) {
			float leaderdx = Packs[cptr->packId].leader->pos.x - cptr->pos.x;
			float leaderdz = Packs[cptr->packId].leader->pos.z - cptr->pos.z;
			float leaderdist = (float)sqrt(leaderdx * leaderdx + leaderdz * leaderdz);

			if (cptr->followLeader) {
				if (leaderdist < DinoInfo[cptr->CType].packDensity * 128 * 0.6)
				{
					cptr->followLeader = false;
					SetNewTargetPlace(cptr, AIInfo[cptr->Clone].targetDistance);
					goto TBEGIN;
				}
			} else {
				if (leaderdist > DinoInfo[cptr->CType].packDensity * 128 * 1.3)
				{
					cptr->followLeader = true;
				}
			}

		}

		float tdst = 456;
		if (cptr->gliding) tdst = 1024;

		if (cptr->followLeader) {
			cptr->tgx = Packs[cptr->packId].leader->pos.x;
			cptr->tgz = Packs[cptr->packId].leader->pos.z;
		}
		else if (tdist < tdst)
		{
			SetNewTargetPlace(cptr, AIInfo[cptr->Clone].targetDistance);
			goto TBEGIN;
		}



	}

NOTHINK:
	if (pdist < AIInfo[cptr->Clone].pWMin && !cptr->gliding) cptr->NoFindCnt = 0;
	if (cptr->NoFindCnt && !cptr->gliding) cptr->NoFindCnt--;
	else
	{
		cptr->tgalpha = CorrectedAlpha(FindVectorAlpha(targetdx, targetdz), cptr->alpha);//FindVectorAlpha(targetdx, targetdz);

		if (cptr->State && pdist > AIInfo[cptr->Clone].weaveRange)
		{
			float rTD;
			rTD = 824.f;

			cptr->tgalpha += (float)sin(RealTime / rTD) / AIInfo[cptr->Clone].tGAIncrement;
			if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
			if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;
		}
	}

	if (!cptr->gliding) {

		LookForAWay(cptr, !DinoInfo[cptr->CType].canSwim, TRUE);

		if (cptr->NoWayCnt > AIInfo[cptr->Clone].noWayCntMin)
		{
			cptr->NoWayCnt = 0;
			cptr->NoFindCnt = AIInfo[cptr->Clone].noFindWayMed + rRand(AIInfo[cptr->Clone].noFindWayRange);
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

		if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) {
			if (DinoInfo[cptr->CType].killType[cptr->killType].dontloop) {
				cptr->Phase = DinoInfo[cptr->CType].walkAnim;
				cptr->State = 0;
			}
		}


		NewPhase = TRUE;
	}

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount)  goto ENDPSELECT;

	float FlDst = ctViewR * DinoInfo[cptr->CType].flyDist + OptAgres / AIInfo[cptr->Clone].agressMulti;
	if (!alertInit) FlDst *= 1.5;
	if (cptr->State && pdist > FlDst) cptr->gliding = true;

	if (NewPhase){

		if (!cptr->State && rRand(50) == 2) cptr->gliding = true;

		if (cptr->gliding) {

			if (!cptr->State || fleeMode) {
				//WANDER/FLEE

				if (cptr->Phase == DinoInfo[cptr->CType].flyAnim) {
					if (cptr->pos.y > GetLandUpH(cptr->pos.x, cptr->pos.z) + 5800) {
						cptr->Phase = DinoInfo[cptr->CType].glideAnim;
					}
				}
				else if (cptr->Phase == DinoInfo[cptr->CType].glideAnim) {
					if (cptr->pos.y < GetLandUpH(cptr->pos.x, cptr->pos.z) + 3800) {
						cptr->Phase = DinoInfo[cptr->CType].flyAnim;
					}
				}
				else if (cptr->Phase == DinoInfo[cptr->CType].takeoffAnim) {
					if (cptr->pos.y > GetLandUpH(cptr->pos.x, cptr->pos.z) + 1024) {
						cptr->Phase = DinoInfo[cptr->CType].flyAnim;
					}
				}
				else {
					cptr->beta = 0;
					cptr->gamma = 0;
					cptr->Slide = 0;
					cptr->Phase = DinoInfo[cptr->CType].takeoffAnim;
				}
				
			} else {
				if (cptr->Phase != DinoInfo[cptr->CType].takeoffAnim &&
					cptr->Phase != DinoInfo[cptr->CType].glideAnim &&
					cptr->Phase != DinoInfo[cptr->CType].flyAnim) {
					cptr->beta = 0;
					cptr->gamma = 0;
					cptr->Slide = 0;
					cptr->Phase = DinoInfo[cptr->CType].takeoffAnim;
				} else {
					float dalph = cptr->alpha - cptr->tgalpha;
					if (dalph < 0) dalph *= -1;
					if (dalph > pi) dalph -= pi;
					if (dalph > pi/2) {
						if(cptr->pos.y - PlayerY < pdist / 2) cptr->Phase = DinoInfo[cptr->CType].takeoffAnim;
						else if (cptr->pos.y > PlayerY + 600) cptr->Phase = DinoInfo[cptr->CType].glideAnim;
						else cptr->Phase = DinoInfo[cptr->CType].flyAnim;
					} else {
						if (cptr->pos.y < PlayerY + 256 && pdist > 2048) cptr->Phase = DinoInfo[cptr->CType].takeoffAnim;
						else if (cptr->pos.y - PlayerY > pdist / 2)cptr->Phase = DinoInfo[cptr->CType].diveAnim;
						else if (cptr->pos.y > PlayerY + 600) cptr->Phase = DinoInfo[cptr->CType].glideAnim;
						else cptr->Phase = DinoInfo[cptr->CType].flyAnim;
					}

					/*
					if (256 + cptr->pos.y - PlayerY < pdist/2) {
						cptr->Phase = DinoInfo[cptr->CType].takeoffAnim;
					} else if (cptr->pos.y - PlayerY > pdist / 2) {
						cptr->Phase = DinoInfo[cptr->CType].diveAnim;
					} else if (cptr->pos.y > PlayerY + 600) {
						cptr->Phase = DinoInfo[cptr->CType].glideAnim;
					} else cptr->Phase = DinoInfo[cptr->CType].flyAnim;
					*/
				}
				
			}
			
		} else {

			if (!cptr->State) {

				if (DinoInfo[cptr->CType].idleCount
					&& (MyHealth || !DinoInfo[cptr->CType].killType[cptr->killType].carryCorpse)
					&& !(cptr->StateF & csONWATER)) {

					if (rRand(AIInfo[cptr->Clone].idleStartD) > 110) {
						cptr->Phase = DinoInfo[cptr->CType].idleAnim[rRand(DinoInfo[cptr->CType].idleCount - 1)];
						goto ENDPSELECT;
					}
					else {
						cptr->Phase = DinoInfo[cptr->CType].walkAnim;
					}

				} else {
					cptr->Phase = DinoInfo[cptr->CType].walkAnim;
				}

			}
			else cptr->Phase = DinoInfo[cptr->CType].runAnim;

		}
	}

	bool idlePhase = false;
	for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
		if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) idlePhase = true;
	}

	if (!idlePhase && !cptr->gliding) {
		if (!cptr->State) cptr->Phase = DinoInfo[cptr->CType].walkAnim;
		else if (fabs(cptr->tgalpha - cptr->alpha) < 1.0 ||
			fabs(cptr->tgalpha - cptr->alpha) > 2 * pi - 1.0)
			cptr->Phase = DinoInfo[cptr->CType].runAnim;
		else cptr->Phase = DinoInfo[cptr->CType].walkAnim;

	}

	if (DinoInfo[cptr->CType].canSwim) {
		if (cptr->StateF & csONWATER) cptr->Phase = DinoInfo[cptr->CType].swimAnim;
	}

	if (cptr->gliding) {
		if (cptr->Slide > 40) cptr->Phase = DinoInfo[cptr->CType].slideAnim;
	}


ENDPSELECT:

	//====== process phase changing ===========//
	if ((_Phase != cptr->Phase) || NewPhase)
		ActivateCharacterFx(cptr);

	if (_Phase != cptr->Phase)
	{
		//==== set proportional FTime for better morphing =//

		if (cptr->gliding) {
			if (!NewPhase) cptr->FTime = 0;
		} else if (MORPHP) {
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
	float dalpha = fabs(cptr->tgalpha - cptr->alpha);
	float drspd = dalpha;
	if (drspd > pi) drspd = 2 * pi - drspd;
	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) goto SKIPROT;
	for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
		if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) goto SKIPROT;
	}




	if (drspd > 0.02)
		if (cptr->tgalpha > cptr->alpha) currspeed = 0.6f + drspd * 1.2f;
		else currspeed = -0.6f - drspd * 1.2f;
	else currspeed = 0;
	if (cptr->AfraidTime && !cptr->gliding) currspeed *= 2.5;
	if (cptr->gliding) currspeed /= 2;

	if (dalpha > pi) currspeed *= -1;
	if (((cptr->StateF & csONWATER) || cptr->Phase == DinoInfo[cptr->CType].walkAnim ) && !cptr->gliding) currspeed /= 1.4f;

	if (cptr->gliding) DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 460.f);
	else if (cptr->AfraidTime) DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 160.f);
	else DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 180.f);

	if (cptr->gliding) {
		tgbend = drspd / 2.f;
		if (tgbend > pi / 10) tgbend = pi / 10;
	} else {
		tgbend = drspd / AIInfo[cptr->Clone].targetBendRotSpd;
		if (tgbend > pi / 5) tgbend = pi / 5;
	}

	tgbend *= SGN(currspeed);
	if (fabs(tgbend) > fabs(cptr->bend)) DeltaFunc(cptr->bend, tgbend, (float)TimeDt / 800.f);
	else if (cptr->gliding) DeltaFunc(cptr->bend, tgbend, (float)TimeDt / 400.f); 
	else DeltaFunc(cptr->bend, tgbend, (float)TimeDt / 600.f);


	rspd = cptr->rspeed * TimeDt / 1024.f;




	if (drspd < fabs(rspd)) cptr->alpha = cptr->tgalpha;
	else cptr->alpha += rspd;


	if (cptr->alpha > pi * 2) cptr->alpha -= pi * 2;
	if (cptr->alpha < 0) cptr->alpha += pi * 2;

SKIPROT:


	if (cptr->gliding) {
		//======= set slide mode ===========//
		if (!cptr->Slide && cptr->vspeed > 0.6)
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
	if (cptr->Phase == DinoInfo[cptr->CType].flyAnim) curspeed = cptr->speed_fly;
	if (cptr->Phase == DinoInfo[cptr->CType].glideAnim) curspeed = cptr->speed_glide;
	if (cptr->Phase == DinoInfo[cptr->CType].takeoffAnim) curspeed = cptr->speed_takeoff;
	if (cptr->Phase == DinoInfo[cptr->CType].diveAnim) curspeed = cptr->speed_dive;
	if (DinoInfo[cptr->CType].canSwim) {
		if (cptr->Phase == DinoInfo[cptr->CType].swimAnim) curspeed = cptr->speed_swim;
	}

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) curspeed = 0.0f;

	if (cptr->gliding && cptr->Slide)
	{
		curspeed /= 8;
		if (drspd > pi / 2.f) curspeed = 0;
		else if (drspd > pi / 4.f) curspeed *= 2.f - 4.f*drspd / pi;
	}
	else if (drspd > pi / 2.f) curspeed *= 2.f - 2.f*drspd / pi;

	if (cptr->Phase == DinoInfo[cptr->CType].flyAnim) cptr->pos.y += TimeDt / 5.f;
	if (cptr->Phase == DinoInfo[cptr->CType].takeoffAnim) cptr->pos.y += TimeDt / 4.f;
	if (cptr->Phase == DinoInfo[cptr->CType].glideAnim) cptr->pos.y -= TimeDt / 10.f;
	if (cptr->Phase == DinoInfo[cptr->CType].diveAnim) cptr->pos.y -= TimeDt;

	//if (cptr->pos.y < GetLandH(cptr->pos.x, cptr->pos.z) + 236) cptr->pos.y = GetLandH(cptr->pos.x, cptr->pos.z) + 256;

	//========== process speed =============//

	if (cptr->gliding) {
		curspeed *= cptr->scale;
		DeltaFunc(cptr->vspeed, curspeed, TimeDt / 2024.f);

		cptr->pos.x += cptr->lookx * cptr->vspeed * TimeDt;
		cptr->pos.z += cptr->lookz * cptr->vspeed * TimeDt;

		cptr->tggamma = cptr->rspeed / 1.5f;
		if (cptr->tggamma > pi / 3.f) cptr->tggamma = pi / 3.f;
		if (cptr->tggamma < -pi / 3.f) cptr->tggamma = -pi / 3.f;
		DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 3048.f);

	} else {

		DeltaFunc(cptr->vspeed, curspeed, TimeDt / 500.f);

		MoveCharacter(cptr, cptr->lookx * cptr->vspeed * TimeDt * cptr->scale,
			cptr->lookz * cptr->vspeed * TimeDt * cptr->scale, !DinoInfo[cptr->CType].canSwim, TRUE);

		//========== slide ==============//
		if (cptr->Slide && cptr->gliding)
		{
			MoveCharacter(cptr, cptr->slidex * cptr->Slide / 600.f * TimeDt * cptr->scale,
				cptr->slidez * cptr->Slide / 600.f * TimeDt * cptr->scale, !DinoInfo[cptr->CType].canSwim, TRUE);

			cptr->Slide -= TimeDt;
			if (cptr->Slide < 0) cptr->Slide = 0;
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

		DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 1624.f);

		//==================================================//


	}

}

void AnimateHuntable(TCharacter *cptr)
{
	NewPhase = FALSE;
	int _Phase = cptr->Phase;
	int _FTime = cptr->FTime;
	float _tgalpha = cptr->tgalpha;
	if ((!AIInfo[cptr->Clone].carnivore || AIInfo[cptr->Clone].iceAge) && cptr->AfraidTime) cptr->AfraidTime = MAX(0, cptr->AfraidTime - TimeDt);

	bool alertInit = FALSE;
	if (cptr->State == 2) alertInit = TRUE;
	if (cptr->packId >= 0) {
		if (!cptr->State && Packs[cptr->packId]._alert) alertInit = TRUE;
	}

	if (alertInit && (MyHealth || AIInfo[cptr->Clone].carnivore))
	{
		if (!AIInfo[cptr->Clone].carnivore) NewPhase = TRUE;

		if (AIInfo[cptr->Clone].jumper) {
			if (cptr->Phase != DinoInfo[cptr->CType].jumpAnim) NewPhase = TRUE;
		}
		cptr->State = 1;

		if (cptr->Clone == AI_SPINO || cptr->Clone == AI_CERAT) cptr->Phase = DinoInfo[cptr->CType].runAnim;
	}

TBEGIN:
	float targetx = cptr->tgx;
	float targetz = cptr->tgz;
	float targetdx = targetx - cptr->pos.x;
	float targetdz = targetz - cptr->pos.z;

	float tdist = (float)sqrt(targetdx * targetdx + targetdz * targetdz);

	float playerdx, playerdz;
	if (cptr->Clone == AI_ALLO) {
		playerdx = PlayerX - cptr->pos.x - cptr->lookx * 100 * cptr->scale;
		playerdz = PlayerZ - cptr->pos.z - cptr->lookz * 100 * cptr->scale;
	} else if (cptr->Clone == AI_CHASM || cptr->Clone == AI_HOG || cptr->Clone == AI_BRONT || cptr->Clone == AI_BEAR ||
		cptr->Clone == AI_WOLF || cptr->Clone == AI_RHINO || cptr->Clone == AI_SMILO) {
		playerdx = PlayerX - cptr->pos.x - cptr->lookx * 300 * cptr->scale;
		playerdz = PlayerZ - cptr->pos.z - cptr->lookz * 300 * cptr->scale;
	} else if (AIInfo[cptr->Clone].carnivore) {
		playerdx = PlayerX - cptr->pos.x - cptr->lookx * 108;
		playerdz = PlayerZ - cptr->pos.z - cptr->lookz * 108;
	} else {
		playerdx = PlayerX - cptr->pos.x;
		playerdz = PlayerZ - cptr->pos.z;
	}

	float pdist = (float)sqrt(playerdx * playerdx + playerdz * playerdz);

	


	if (GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z) > DinoInfo[cptr->CType].waterLevel * cptr->scale)
		cptr->StateF |= csONWATER;
	else
		cptr->StateF &= (!csONWATER);

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) goto NOTHINK;

	//============================================//			// (run away)
	if (!MyHealth) cptr->State = 0;
	if (cptr->State)
	{
		float aDist;
		if (AIInfo[cptr->Clone].carnivore && (!AIInfo[cptr->Clone].iceAge || cptr->Clone == AI_WOLF)) {
			aDist = ctViewR * DinoInfo[cptr->CType].aggress + OptAgres / AIInfo[cptr->Clone].agressMulti;
		} else {
			aDist = AIInfo[cptr->Clone].agressMulti * DinoInfo[cptr->CType].aggress + OptAgres / 8;
			if (pdist < 6000 && cptr->Clone != AI_DEER) cptr->AfraidTime = 8000;
		}

		bool fleeMode = FALSE;
		if (pdist > aDist ||
			DinoInfo[cptr->CType].aggress <= 0 || !cptr->awareHunter) {
			fleeMode = TRUE;
		}
		else if (cptr->packId >= 0) Packs[cptr->packId].attack = TRUE;

		if (cptr->packId >= 0) {
			if (Packs[cptr->packId]._attack) fleeMode = FALSE;
		}

		if (fleeMode) {
			nv.x = playerdx;
			nv.z = playerdz;
			nv.y = 0;
			NormVector(nv, 2048.f);
			cptr->tgx = cptr->pos.x - nv.x;
			cptr->tgz = cptr->pos.z - nv.z;
			cptr->tgtime = 0;
			if (AIInfo[cptr->Clone].carnivore) cptr->AfraidTime -= TimeDt;

			if (cptr->packId >= 0) {
				if (cptr->AfraidTime <= 0)
				{
					if (!Packs[cptr->packId]._alert) {
						if (AIInfo[cptr->Clone].carnivore)cptr->AfraidTime = 0;
						else SetNewTargetPlace(cptr, AIInfo[cptr->Clone].targetDistance);
						cptr->State = 0;
					}
				}
				else Packs[cptr->packId].alert = true;
			}
			else if (cptr->AfraidTime <= 0) {
				if (AIInfo[cptr->Clone].carnivore)cptr->AfraidTime = 0;
				else SetNewTargetPlace(cptr, AIInfo[cptr->Clone].targetDistance);
				cptr->State = 0;
			}

		}
		else
		{
			cptr->tgx = PlayerX;
			cptr->tgz = PlayerZ;
			cptr->tgtime = 0;
			if (cptr->packId >= 0 && AIInfo[cptr->Clone].carnivore) {
				Packs[cptr->packId].alert = true;
			}
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
						if (DinoInfo[cptr->CType].killType[cptr->killType].dontloop) cptr->FTime = 0;
						AddDeadBody(cptr,
							DinoInfo[cptr->CType].killType[cptr->killType].hunteranim,
							DinoInfo[cptr->CType].killType[cptr->killType].scream);
					}
					else AddDeadBody(cptr, HUNT_EAT, TRUE);

				}
				else {
					AddDeadBody(cptr, HUNT_EAT, TRUE);
					cptr->State = 0;
				}

			}
		}


	}

	if (!cptr->State)
	{
		if (cptr->Clone == AI_VELO || cptr->Clone == AI_CERAT || !AIInfo[cptr->Clone].carnivore) cptr->AfraidTime = 0;

		if (pdist < 1024.f && cptr->Clone == AI_DEER && !ObservMode && !DEBUG) {
			cptr->State = 1;
			cptr->AfraidTime = (6 + rRand(8)) * 1024;
			cptr->Phase = cptr->Phase = DinoInfo[cptr->CType].runAnim;
			goto TBEGIN;
		}


		if (cptr->packId >= 0) {
			float leaderdx = Packs[cptr->packId].leader->pos.x - cptr->pos.x;
			float leaderdz = Packs[cptr->packId].leader->pos.z - cptr->pos.z;
			float leaderdist = (float)sqrt(leaderdx * leaderdx + leaderdz * leaderdz);

			if (cptr->followLeader) {
				if (leaderdist < DinoInfo[cptr->CType].packDensity * 128 * 0.6)
				{
					cptr->followLeader = false;
					SetNewTargetPlace(cptr, AIInfo[cptr->Clone].targetDistance);
					goto TBEGIN;
				}
			}
			else {
				if (leaderdist > DinoInfo[cptr->CType].packDensity * 128 * 1.3)
				{
					cptr->followLeader = true;
				}
			}

		}

		if (cptr->followLeader) {
			cptr->tgx = Packs[cptr->packId].leader->pos.x;
			cptr->tgz = Packs[cptr->packId].leader->pos.z;
		}
		else if (tdist < 456)
		{
			SetNewTargetPlace(cptr, AIInfo[cptr->Clone].targetDistance);
			goto TBEGIN;
		}



	}

NOTHINK:
	if (pdist < AIInfo[cptr->Clone].pWMin && (AIInfo[cptr->Clone].carnivore || AIInfo[cptr->Clone].iceAge)) cptr->NoFindCnt = 0;
	if (cptr->NoFindCnt) cptr->NoFindCnt--;
	else
	{
		cptr->tgalpha = CorrectedAlpha(FindVectorAlpha(targetdx, targetdz), cptr->alpha);//FindVectorAlpha(targetdx, targetdz);

		//bool weaveCondition = pdist > AIInfo[cptr->Clone].weaveRange;

		/*
		if (AIInfo[cptr->Clone].iceAge) {
			weaveCondition = pdist > 3072 && cptr->AfraidTime; //12*256
		} else {
			if (AIInfo[cptr->Clone].carnivore) {
				weaveCondition = cptr->State && pdist > 1648;
			}
			else {
				weaveCondition = cptr->AfraidTime;
			}
		}
		*/

		//if (!AIInfo[cptr->Clone].carnivore || AIInfo[cptr->Clone].iceAge) weaveCondition = weaveCondition && cptr->AfraidTime;

		if (cptr->State && pdist > AIInfo[cptr->Clone].weaveRange)
		{
			float rTD;
			if (AIInfo[cptr->Clone].carnivore && !AIInfo[cptr->Clone].iceAge) {
				rTD = 824.f;
			} else {
				rTD = 1024.f;
			}
			cptr->tgalpha += (float)sin(RealTime / rTD) / AIInfo[cptr->Clone].tGAIncrement;
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

		if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) {
			if (DinoInfo[cptr->CType].killType[cptr->killType].dontloop) {
				cptr->Phase = DinoInfo[cptr->CType].walkAnim;
				cptr->State = 0;
			}
		}


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




			if (DinoInfo[cptr->CType].idleCount
				&& (MyHealth || !DinoInfo[cptr->CType].killType[cptr->killType].carryCorpse)
				&& !(cptr->StateF & csONWATER)) {

				if (AIInfo[cptr->Clone].carnivore) {

					if (AIInfo[cptr->Clone].iceAge) {


						for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
							if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) {
								
								if (cptr->Clone == AI_WOLF) {
									if (rRand(128) > 64) cptr->Phase = DinoInfo[cptr->CType].walkAnim;
									else cptr->Phase = DinoInfo[cptr->CType].idleAnim[rRand(DinoInfo[cptr->CType].idleCount - 1)];//Wolf AI has this extra bit, don't feckin ask, I dunno
								} else cptr->Phase = DinoInfo[cptr->CType].walkAnim;
								
								goto ENDPSELECT;
							}
						}

						if (rRand(128) > AIInfo[cptr->Clone].idleStartD) {
							cptr->Phase = DinoInfo[cptr->CType].idleAnim[rRand(DinoInfo[cptr->CType].idleCount - 1)]; //yeah, wolf has this bit too... I know
						}


					} else {

						if (rRand(AIInfo[cptr->Clone].idleStartD) > 110) {
							cptr->Phase = DinoInfo[cptr->CType].idleAnim[rRand(DinoInfo[cptr->CType].idleCount - 1)];
							goto ENDPSELECT;
						}
						else {
							cptr->Phase = DinoInfo[cptr->CType].walkAnim;
						}



					}



				} else {

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

	if (!idlePhase){
	if (AIInfo[cptr->Clone].carnivore && !AIInfo[cptr->Clone].iceAge) {
		if (!cptr->State) cptr->Phase = DinoInfo[cptr->CType].walkAnim;
		else if (fabs(cptr->tgalpha - cptr->alpha) < 1.0 ||
			fabs(cptr->tgalpha - cptr->alpha) > 2 * pi - 1.0)
			cptr->Phase = DinoInfo[cptr->CType].runAnim;
		else cptr->Phase = DinoInfo[cptr->CType].walkAnim;
	} else {
		//NEEDED FOR SWIMMING STUFF
		if (!cptr->State) cptr->Phase = DinoInfo[cptr->CType].walkAnim;
		else cptr->Phase = DinoInfo[cptr->CType].runAnim;
	}
	}

	if (DinoInfo[cptr->CType].canSwim) {
		if (cptr->StateF & csONWATER) cptr->Phase = DinoInfo[cptr->CType].swimAnim;
	}

	if (cptr->Clone != AI_CERAT && AIInfo[cptr->Clone].carnivore && !AIInfo[cptr->Clone].iceAge) {
		if (cptr->Slide > 40) cptr->Phase = DinoInfo[cptr->CType].slideAnim;
	}


ENDPSELECT:

	//====== process phase changing ===========//
	if ((_Phase != cptr->Phase) || NewPhase)
		ActivateCharacterFx(cptr);

	if (_Phase != cptr->Phase)
	{
		//==== set proportional FTime for better morphing =//

		if (MORPHP || !AIInfo[cptr->Clone].carnivore || AIInfo[cptr->Clone].iceAge) {
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
    float dalpha = fabs(cptr->tgalpha - cptr->alpha);
	float drspd = dalpha;
	if (drspd > pi) drspd = 2 * pi - drspd;

	if (AIInfo[cptr->Clone].jumper) {
		if (cptr->Phase == DinoInfo[cptr->CType].jumpAnim) goto SKIPROT;
	}
	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) goto SKIPROT;
	for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
		if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) goto SKIPROT;
	}


	if (AIInfo[cptr->Clone].carnivore && !AIInfo[cptr->Clone].iceAge) {

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

	} else {

		if (drspd > 0.02){
			if (cptr->tgalpha > cptr->alpha) currspeed = AIInfo[cptr->Clone].rot1 + drspd * AIInfo[cptr->Clone].rot2;
			else currspeed = -AIInfo[cptr->Clone].rot1 - drspd * AIInfo[cptr->Clone].rot2;
		} else currspeed = 0;

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

	}


	if (drspd < fabs(rspd)) cptr->alpha = cptr->tgalpha;
	else cptr->alpha += rspd;


	if (cptr->alpha > pi * 2) cptr->alpha -= pi * 2;
	if (cptr->alpha < 0) cptr->alpha += pi * 2;

SKIPROT:

	if (cptr->Clone != AI_CERAT && AIInfo[cptr->Clone].carnivore && !AIInfo[cptr->Clone].iceAge) {
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

	if (cptr->Phase == DinoInfo[cptr->CType].runAnim && cptr->Slide && cptr->Clone != AI_CERAT && AIInfo[cptr->Clone].carnivore && !AIInfo[cptr->Clone].iceAge)
	{
		curspeed /= 8;
		if (drspd > pi / 2.f) curspeed = 0;
		else if (drspd > pi / 4.f) curspeed *= 2.f - 4.f*drspd / pi;
	}
	else if (drspd > pi / 2.f) curspeed *= 2.f - 2.f*drspd / pi;

	//========== process speed =============//

	if (AIInfo[cptr->Clone].carnivore && !AIInfo[cptr->Clone].iceAge) {

		DeltaFunc(cptr->vspeed, curspeed, TimeDt / 500.f);

		if (AIInfo[cptr->Clone].jumper) {
			if (cptr->Phase == DinoInfo[cptr->CType].jumpAnim) cptr->vspeed = cptr->speed_jump;
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

	} else {

		curspeed *= cptr->scale;
		if (curspeed > cptr->vspeed) DeltaFunc(cptr->vspeed, curspeed, TimeDt / 1024.f);
		else DeltaFunc(cptr->vspeed, curspeed, TimeDt / 256.f);

		MoveCharacter(cptr, cptr->lookx * cptr->vspeed * TimeDt,
			cptr->lookz * cptr->vspeed * TimeDt, !DinoInfo[cptr->CType].canSwim, TRUE);

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

	if (AIInfo[cptr->Clone].carnivore && !AIInfo[cptr->Clone].iceAge){
		DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 1624.f);
	} else {
		DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 2048.f);
	}

	//==================================================//

}




//carn animate
/*
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

	bool alertInit = FALSE;
	if (cptr->State == 2) alertInit = TRUE;
	if (cptr->packId >= 0) {
		if (!cptr->State && Packs[cptr->packId]._alert) alertInit = TRUE;
	}

	if (alertInit)
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

		bool fleeMode = FALSE;
		if (pdist > ctViewR * DinoInfo[cptr->CType].aggress + OptAgres / AIInfo[cptr->Clone].agressMulti ||
			DinoInfo[cptr->CType].aggress <= 0 || !cptr->awareHunter ) {
			fleeMode = TRUE;
		} else if (cptr->packId >= 0) Packs[cptr->packId].attack = TRUE;

		if (cptr->packId >= 0) {
			if (Packs[cptr->packId]._attack) fleeMode = FALSE;
		}

		if (fleeMode) {
			nv.x = playerdx;
			nv.z = playerdz;
			nv.y = 0;
			NormVector(nv, 2048.f);
			cptr->tgx = cptr->pos.x - nv.x;
			cptr->tgz = cptr->pos.z - nv.z;
			cptr->tgtime = 0;
			cptr->AfraidTime -= TimeDt;

			if (cptr->packId >= 0) {
				if (cptr->AfraidTime <= 0)
				{
					if (!Packs[cptr->packId]._alert) {
						cptr->AfraidTime = 0;
						cptr->State = 0;
					}
				} else Packs[cptr->packId].alert = true;
			} else if (cptr->AfraidTime <= 0) {
				cptr->AfraidTime = 0;
				cptr->State = 0;
			}

		}
		else
		{
			cptr->tgx = PlayerX;
			cptr->tgz = PlayerZ;
			cptr->tgtime = 0;
			if (cptr->packId >= 0) {
				Packs[cptr->packId].alert = true;
			}
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
						if (DinoInfo[cptr->CType].killType[cptr->killType].dontloop) cptr->FTime = 0;
						AddDeadBody(cptr,
									DinoInfo[cptr->CType].killType[cptr->killType].hunteranim,
									DinoInfo[cptr->CType].killType[cptr->killType].scream);
					}
					else AddDeadBody(cptr, HUNT_EAT, TRUE);

				} else {
					AddDeadBody(cptr, HUNT_EAT, TRUE);
					cptr->State = 0;
				}

			}
		}


	}

	if (!cptr->State)
	{
		if (cptr->Clone == AI_VELO || cptr->Clone == AI_CERAT) cptr->AfraidTime = 0;



		if (cptr->packId >= 0) {
			float leaderdx = Packs[cptr->packId].leader->pos.x - cptr->pos.x;
			float leaderdz = Packs[cptr->packId].leader->pos.z - cptr->pos.z;
			float leaderdist = (float)sqrt(leaderdx * leaderdx + leaderdz * leaderdz);

			if (cptr->followLeader) {
				if (leaderdist < DinoInfo[cptr->CType].packDensity * 128 * 0.6)
				{
					cptr->followLeader = false;
					SetNewTargetPlace(cptr, 8048.f);
					goto TBEGIN;
				}
			}
			else {
				if (leaderdist > DinoInfo[cptr->CType].packDensity * 128 * 1.3)
				{
					cptr->followLeader = true;
				}
			}

		}

		if (cptr->followLeader) {
			cptr->tgx = Packs[cptr->packId].leader->pos.x;
			cptr->tgz = Packs[cptr->packId].leader->pos.z;
		}
		else if (tdist < 456)
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

		if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) {
			if (DinoInfo[cptr->CType].killType[cptr->killType].dontloop) {
				cptr->Phase = DinoInfo[cptr->CType].walkAnim;
				cptr->State = 0;
			}
		}


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




			if (DinoInfo[cptr->CType].idleCount
				&& (MyHealth || !DinoInfo[cptr->CType].killType[cptr->killType].carryCorpse)
				&& !(cptr->StateF & csONWATER)) {

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
		if (cptr->Phase == DinoInfo[cptr->CType].jumpAnim) cptr->vspeed = cptr->speed_jump;
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
*/






boolean huntDogSearch(TCharacter *cptr)
{
	bool preyFound = FALSE;
	Vector3d preyPos;
	float preyDist;
	int preyNo;

	//if (!MyHealth) return false;
	if (TrophyMode) return false;

	float kR, kwind, klook, kstand;

	float kmask = 1.0f;
	float kscent = 1.5f;

	for (int c = 0; c < ChCount; c++)
	{
		TCharacter *dino = &Characters[c];

		Vector3d ppos, plook, clook, wlook, rlook;
		ppos = dino->pos;

		wlook = Wind.nv;

		plook.y = 0;
		plook.x = (float)sin(dino->alpha);
		plook.z = (float)-cos(dino->alpha);

		if (!dino->Health) continue;
		if (!DinoInfo[dino->CType].dogSmell) continue;

		rlook = SubVectors(dino->pos, cptr->pos);
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
				dino->pos.x, dino->pos.y, dino->pos.z)) kALook *= 1.3f;

		if (kALook < 1.0)
			if (TraceLook(cptr->pos.x, cptr->pos.y + 220, cptr->pos.z,
				dino->pos.x, dino->pos.y, dino->pos.z))   kALook = 2.0;
		kALook *= (1.f + (float)ObjectsOnLook / 6.f);

		float kASmell = kR * ((kwind + 2.0f) / 2.0F) * ((klook + 3.f) / 3.f) * kscent;
		if (kwind > 0) kASmell *= 2.0;
		kASmell /= DinoInfo[cptr->CType].SmellK;

		float kRes = MIN(kALook, kASmell);

		if (kRes < 1.0)
		{
			kRes = MIN(kRes, kR);

			if (preyFound) {
				float dx = dino->pos.x - cptr->pos.x;
				float dz = dino->pos.z - cptr->pos.z;
				float tempDist = (float)sqrt(dx * dx + dz * dz);
				if (tempDist < preyDist) {
					preyDist = tempDist;
					preyPos = dino->pos;
					preyNo = c;
				}
			} else {
				float dx = dino->pos.x - cptr->pos.x;
				float dz = dino->pos.z - cptr->pos.z;
				preyDist = (float)sqrt(dx * dx + dz * dz);
				preyPos = dino->pos;
				preyNo = c;
				preyFound = true;
			}

		}
	}

	if (preyFound) {
		cptr->tgx = preyPos.x;
		cptr->tgz = preyPos.z;
		cptr->tgtime = 0;
		cptr->dogPrey = preyNo;
		return true;
	}

	return false;
}









void AnimateHuntdog(TCharacter *cptr)
{
	NewPhase = FALSE;
	int _Phase = cptr->Phase;
	int _FTime = cptr->FTime;
	float _tgalpha = cptr->tgalpha;
//	if (cptr->AfraidTime) {
//		cptr->AfraidTime = MAX(0, cptr->AfraidTime - TimeDt);
//	}

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

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) goto NOTHINK;


	//======== exploring area ===============//

	if (cptr->State == 1){
		cptr->tgx = PlayerX;
		cptr->tgz = PlayerZ;
		cptr->tgtime = 0;

		if (pdist < 3 * 256) {
			cptr->State = 0;
			SetNewTargetPlace(cptr, 8048.f);
		}

	}

	if (cptr->State == 0)
	{

		if (huntDogSearch(cptr)) {
			cptr->State = 2;
		} else {

			if (tdist < 456)
			{
				SetNewTargetPlace(cptr, 8048.f);
				goto TBEGIN;
			}

			if (pdist > 4 * 256) {
				cptr->tgx = PlayerX;
				cptr->tgz = PlayerZ;
				cptr->tgtime = 0;
			}

			if (pdist > 8 * 256) {
				cptr->State = 1;
			}

		}

	}

	if (cptr->State == 2) {

		if (pdist > 17 * 256) {
			cptr->State = 1;
		}

		if (tdist < 456 || !Characters[cptr->dogPrey].Health)
		{
			if (!huntDogSearch(cptr)) {
				cptr->State = 0;
				SetNewTargetPlace(cptr, 8048.f);
				goto TBEGIN;
			}
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

	//even if not newphase...
	if (cptr->Phase == 2 && _Phase != 2) {

		if (DinoInfo[cptr->CType].idle2Count > 0) {
			cptr->Phase = DinoInfo[cptr->CType].idle2Anim[rRand(DinoInfo[cptr->CType].idle2Count - 1)];
		}

		//cptr->Phase == cptr->roarAnim;
		cptr->FTime = 0;
		goto ENDPSELECT;
	}

	if (NewPhase){

		if (cptr->State == 1) {
			cptr->Phase = DinoInfo[cptr->CType].runAnim;
		
		} else if (cptr->State == 0){
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

		} else if (cptr->State == 2) {

			if (rRand(AIInfo[cptr->Clone].idleStartD) > 120) {

				if (DinoInfo[cptr->CType].idle2Count > 0) {
					cptr->Phase = DinoInfo[cptr->CType].idle2Anim[rRand(DinoInfo[cptr->CType].idle2Count - 1)];
				}

				//cptr->Phase = cptr->roarAnim;
				goto ENDPSELECT;
			}
			else {
				cptr->Phase = DinoInfo[cptr->CType].runAnim;
			}

		}

	}

	/*
	if (DinoInfo[cptr->CType].canSwim) {
		if (cptr->StateF & csONWATER) cptr->Phase = DinoInfo[cptr->CType].swimAnim;
	}
	*/

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

	//if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) goto SKIPROT;

	//if (cptr->Phase == cptr->roarAnim) goto SKIPROT;

	for (int i = 0; i < DinoInfo[cptr->CType].idle2Count; i++) {
		if (cptr->Phase == DinoInfo[cptr->CType].idle2Anim[i]) goto SKIPROT;
	}

	for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
		if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) goto SKIPROT;
	}

	if (drspd > 0.02)
		if (cptr->tgalpha > cptr->alpha) currspeed = 0.2f + drspd * 1.0f;
		else currspeed = -0.2f - drspd * 1.0f;
	else currspeed = 0;

	//only if fleeing?
	//if (cptr->AfraidTime) currspeed *= 1.5;
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










//universal animate proc herbivore
/*
void AnimateHerbivore(TCharacter *cptr)
{
	NewPhase = FALSE;
	int _Phase = cptr->Phase;
	int _FTime = cptr->FTime;
	float _tgalpha = cptr->tgalpha;
	if (cptr->AfraidTime) cptr->AfraidTime = MAX(0, cptr->AfraidTime - TimeDt);

	bool alertInit = FALSE;
	if (cptr->State == 2) alertInit = TRUE;
	if (cptr->packId >= 0) {
		if (!cptr->State && Packs[cptr->packId]._alert) alertInit = TRUE;
	}

	if (alertInit && MyHealth) {
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

	if (!MyHealth) cptr->State = 0;

	if (cptr->State)
	{
		
		if (pdist < 6000) cptr->AfraidTime = 8000;

		if (cptr->packId >= 0) {
			if (cptr->AfraidTime <= 0) {
				if (!Packs[cptr->packId]._alert) {
					cptr->State = 0;
					SetNewTargetPlace(cptr, AIInfo[cptr->Clone].targetDistance);
					//goto TBEGIN;
				}
			} else Packs[cptr->packId].alert = TRUE;
		} else if (cptr->AfraidTime <= 0) {
			cptr->State = 0;
			SetNewTargetPlace(cptr, AIInfo[cptr->Clone].targetDistance);
			//goto TBEGIN;
		}

		bool fleeMode = FALSE;
		if (pdist > AIInfo[cptr->Clone].agressMulti * DinoInfo[cptr->CType].aggress + OptAgres / 8 || DinoInfo[cptr->CType].aggress <= 0 || !cptr->awareHunter) {
			fleeMode = TRUE;
		} else if (cptr->packId >= 0) Packs[cptr->packId].attack = TRUE;

		if (cptr->packId >= 0) {
			if (Packs[cptr->packId]._attack) fleeMode = FALSE;
		}

		if (fleeMode)
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
							if (DinoInfo[cptr->CType].killType[cptr->killType].dontloop) cptr->FTime = 0;
							AddDeadBody(cptr,
								DinoInfo[cptr->CType].killType[cptr->killType].hunteranim,
								DinoInfo[cptr->CType].killType[cptr->killType].scream);
						}
						else AddDeadBody(cptr, HUNT_EAT, TRUE);

					}
					else {
						AddDeadBody(cptr, HUNT_EAT, TRUE);
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

		if (cptr->packId >=0) {
			float leaderdx = Packs[cptr->packId].leader->pos.x - cptr->pos.x;
			float leaderdz = Packs[cptr->packId].leader->pos.z - cptr->pos.z;
			float leaderdist = (float)sqrt(leaderdx * leaderdx + leaderdz * leaderdz);

			if (cptr->followLeader) {
				if (leaderdist < DinoInfo[cptr->CType].packDensity * 128 * 0.6)
				{
					cptr->followLeader = false;
					SetNewTargetPlace(cptr, 8048.f);
					goto TBEGIN;
				}
			} else{
				if (leaderdist > DinoInfo[cptr->CType].packDensity * 128 * 1.3)
				{
					cptr->followLeader = true;
				}
			}

		}

		if (cptr->followLeader) {
			cptr->tgx = Packs[cptr->packId].leader->pos.x;
			cptr->tgz = Packs[cptr->packId].leader->pos.z;
		} else if (tdist < 456)
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
		if (cptr->AfraidTime && (!AIInfo[cptr->Clone].iceAge || pdist > 12 * 256))
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

		if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) {
			if (DinoInfo[cptr->CType].killType[cptr->killType].dontloop) {
				cptr->Phase = DinoInfo[cptr->CType].walkAnim;
				cptr->State = 0;
			}
		}


		NewPhase = TRUE;
	}

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount)  goto ENDPSELECT;


	if (NewPhase)
		if (!cptr->State)
		{
			if (DinoInfo[cptr->CType].idleCount
				&& (MyHealth || !DinoInfo[cptr->CType].killType[cptr->killType].carryCorpse)
				&& !(cptr->StateF & csONWATER)) {

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

		} else cptr->Phase = DinoInfo[cptr->CType].runAnim;

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
*/

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


	bool alertInit = FALSE;
	if (cptr->State == 5) alertInit = TRUE;
	if (cptr->packId >= 0) {
		if (!cptr->State && Packs[cptr->packId]._alert) alertInit = TRUE;
	}

	if (alertInit)
	{
		NewPhase = TRUE;
		cptr->State = 1;
		cptr->Phase = DinoInfo[cptr->CType].walkAnim;
		cptr->FTime = 0;
		cptr->tgx = PlayerX;
		cptr->tgz = PlayerZ;
		goto TBEGIN;
	}

	if (cptr->State) Packs[cptr->packId].alert = TRUE;



	if (GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z) > DinoInfo[cptr->CType].waterLevel * cptr->scale)
		cptr->StateF |= csONWATER;
	else
		cptr->StateF &= (!csONWATER);

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) goto NOTHINK;

	//============================================//
	//if (!MyHealth) cptr->State = 0; //TREX cannot return to state 0!!!

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
						cptr->rspeed = 0;
					}
					else if (DinoInfo[cptr->CType].roarCount) {
						cptr->Phase = cptr->roarAnim;
						cptr->rspeed = 0;
					} else {
						cptr->Phase = DinoInfo[cptr->CType].runAnim;
					}
					
				}
				else {
					if (DinoInfo[cptr->CType].idle2Count) {
						cptr->Phase = DinoInfo[cptr->CType].idle2Anim[rRand(DinoInfo[cptr->CType].idle2Count - 1)];
						cptr->rspeed = 0;
					}
					else if (DinoInfo[cptr->CType].roarCount) {
						cptr->Phase = cptr->roarAnim;
						cptr->rspeed = 0;
					} else {
						cptr->Phase = DinoInfo[cptr->CType].runAnim;
					}

				}
				cptr->State = 1;
			}




		if (pdist < DinoInfo[cptr->CType].killDist && DinoInfo[cptr->CType].killDist > 0 && MyHealth)
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
						if (DinoInfo[cptr->CType].killType[cptr->killType].dontloop) cptr->FTime = 0;
						AddDeadBody(cptr,
							DinoInfo[cptr->CType].killType[cptr->killType].hunteranim,
							DinoInfo[cptr->CType].killType[cptr->killType].scream);
					}
					else AddDeadBody(cptr, HUNT_EAT, TRUE);

				}
				else {
					AddDeadBody(cptr, HUNT_EAT, TRUE);
					cptr->State = 0;
				}

			}
		}



	}

	if (!cptr->State) {


		if (cptr->packId >= 0) {
			float leaderdx = Packs[cptr->packId].leader->pos.x - cptr->pos.x;
			float leaderdz = Packs[cptr->packId].leader->pos.z - cptr->pos.z;
			float leaderdist = (float)sqrt(leaderdx * leaderdx + leaderdz * leaderdz);

			if (cptr->followLeader) {
				if (leaderdist < DinoInfo[cptr->CType].packDensity * 128 * 0.6)
				{
					cptr->followLeader = false;
					SetNewTargetPlace(cptr, 8048.f);
					goto TBEGIN;
				}
			}
			else {
				if (leaderdist > DinoInfo[cptr->CType].packDensity * 128 * 1.3)
				{
					cptr->followLeader = true;
				}
			}

		}

		if (cptr->followLeader) {
			cptr->tgx = Packs[cptr->packId].leader->pos.x;
			cptr->tgz = Packs[cptr->packId].leader->pos.z;
		}
		else if (tdist < 1224)
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
		if (cptr->State && pdist > 5648)
		{
			cptr->tgalpha += (float)sin(RealTime / 824.f) / 6.f;
			if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
			if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;
		}
	}

	LookForAWay(cptr, !DinoInfo[cptr->CType].canSwim, !cptr->State || DinoInfo[cptr->CType].TRexObjCollide);
	//LookForAWay(cptr, !DinoInfo[cptr->CType].canSwim, TRUE);
	
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

		if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) {
			if (DinoInfo[cptr->CType].killType[cptr->killType].dontloop) {
				cptr->Phase = DinoInfo[cptr->CType].walkAnim;
				cptr->State = 0;
			}
		}


		NewPhase = TRUE;
	}

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount)    goto ENDPSELECT;

	if (!NewPhase)
		if (DinoInfo[cptr->CType].roarCount > 0 && cptr->Phase == cptr->roarAnim) goto ENDPSELECT;
		
	if (!cptr->State)
		if (NewPhase)
			if (rRand(128) > 110
				&& (MyHealth || !DinoInfo[cptr->CType].killType[cptr->killType].carryCorpse)
				&& !(cptr->StateF & csONWATER)
				) {
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

//multiplayer
void AnimateMClientCharacter(TCharacter *cptr)
{
	NewPhase = FALSE;
	int _FTime = cptr->FTime;

	if (cptr->_PhaseM != cptr->Phase) cptr->FTime = 0;

	ProcessPrevPhase(cptr);

	//======== select new phase =======================//
	cptr->FTime += TimeDt;

	if (cptr->FTime >= cptr->pinfo->Animation[cptr->Phase].AniTime)
	{
		if (cptr->Phase == DinoInfo[cptr->CType].deathType[cptr->deathType].die) cptr->FTime = cptr->pinfo->Animation[cptr->Phase].AniTime - 1;
		else {
			cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;
			NewPhase = TRUE;
		}
	}

	//====== process phase changing ===========//
	if ((cptr->_PhaseM != cptr->Phase) || NewPhase) {
		if (cptr->Clone == AI_DIMOR || cptr->Clone == AI_PTERA) {
			if (cptr->Phase == DinoInfo[cptr->CType].flyAnim || cptr->Phase == DinoInfo[cptr->CType].glideAnim) {
				if ((rand() & 1023) > 980) ActivateCharacterFx(cptr);
			} else if (!NewPhase) ActivateCharacterFx(cptr);
		} else ActivateCharacterFx(cptr);
	}

	if (cptr->Phase != DinoInfo[cptr->CType].deathType[cptr->deathType].die) {

		if (cptr->_PhaseM != cptr->Phase)
		{
			if ((cptr->_PhaseM == DinoInfo[cptr->CType].runAnim ||
				cptr->_PhaseM == DinoInfo[cptr->CType].walkAnim) &&
				(cptr->Phase == DinoInfo[cptr->CType].runAnim ||
					cptr->Phase == DinoInfo[cptr->CType].walkAnim))
				cptr->FTime = _FTime * cptr->pinfo->Animation[cptr->Phase].AniTime / cptr->pinfo->Animation[cptr->_PhaseM].AniTime + 64;
			else if (!NewPhase) cptr->FTime = 0;

			if (cptr->PPMorphTime > 128)
			{
				cptr->PrevPhase = cptr->_PhaseM;
				cptr->PrevPFTime = _FTime;
				cptr->PPMorphTime = 0;
			}
		}

		cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;

	}

	//========== rotation to tgalpha ===================//

	//========== movement ==============================//
	cptr->lookx = (float)cos(cptr->alpha);
	cptr->lookz = (float)sin(cptr->alpha);

	//============ Y movement =================//
	if (cptr->Clone != AI_DIMOR && cptr->Clone != AI_PTERA) {
		if (cptr->StateF & csONWATER && DinoInfo[cptr->CType].canSwim)
		{
			cptr->pos.y = GetLandUpH(cptr->pos.x, cptr->pos.z) - (DinoInfo[cptr->CType].waterLevel + 20) * cptr->scale;
			cptr->beta /= 2;
			cptr->tggamma = 0;
		}
		else {
			ThinkY_Beta_Gamma(cptr, 64, 32, 0.7f, 0.4f);
		}

		//if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) cptr->tggamma += cptr->rspeed / 12.0f;
		//else cptr->tggamma += cptr->rspeed / 8.0f;
		DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 2048.f);
	} else cptr->gamma = cptr->bend;

	cptr->_PhaseM = cptr->Phase;
	if (cptr->Phase == DinoInfo[cptr->CType].deathType[cptr->deathType].die) cptr->Health = 0;
	if (cptr->Clone == AI_DIMOR || cptr->Clone == AI_PTERA)
		if (cptr->Phase == DinoInfo[cptr->CType].deathType[cptr->deathType].fall) cptr->Health = 0;
	if (DinoInfo[cptr->CType].waterDieCount > 0)
		if (cptr->Phase == cptr->waterDieAnim) cptr->Health = 0;
}













void AnimateClassicAmbient(TCharacter *cptr)
{
	NewPhase = FALSE;
	int _Phase = cptr->Phase;
	int _FTime = cptr->FTime;
	float _tgalpha = cptr->tgalpha;
	if (cptr->AfraidTime) cptr->AfraidTime = MAX(0, cptr->AfraidTime - TimeDt);

	bool alertInit = FALSE;
	if (cptr->State == 2) alertInit = TRUE;
	if (cptr->packId >= 0) {
		if (!cptr->State && Packs[cptr->packId]._alert) alertInit = TRUE;
	}

	if (alertInit) {
		NewPhase = TRUE;
		cptr->State = 1;
	}

TBEGIN:
	float targetx = cptr->tgx;
	float targetz = cptr->tgz;
	float targetdx = targetx - cptr->pos.x;
	float targetdz = targetz - cptr->pos.z;

	float tdist = (float)sqrt(targetdx * targetdx + targetdz * targetdz);

	bool pdistMulti = FALSE;
	int pCh = 1;
	float pdist[4];
	float playerdx[4];
	float playerdz[4];
	playerdx[0] = PlayerX - cptr->pos.x;
	playerdz[0] = PlayerZ - cptr->pos.z;
	pdist[0] = (float)sqrt(playerdx[0] * playerdx[0] + playerdz[0] * playerdz[0]);
	if (Multiplayer) {
		//for loop 1 to hunter count
		playerdx[pCh] = MPlayers[pCh].pos.x - cptr->pos.x;
		playerdz[pCh] = MPlayers[pCh].pos.z - cptr->pos.z;
		pdist[pCh] = (float)sqrt(playerdx[pCh] * playerdx[pCh] + playerdz[pCh] * playerdz[pCh]);
		pCh += 1;
		//
	}

	if (GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z) > DinoInfo[cptr->CType].waterLevel * cptr->scale)
		cptr->StateF |= csONWATER;
	else
		cptr->StateF &= (!csONWATER);


	//=========== run away =================//

	if (cptr->State)
	{

		if (!cptr->AfraidTime)
		{
			pdistMulti = FALSE;
			for (int pNo = 0; pNo < pCh; pNo++) {
				if (pdist[pNo] < 2048.f) pdistMulti = TRUE;
			}
			if (pdistMulti) {
				if (cptr->Clone == AI_GALL) cptr->State = 1;
				cptr->AfraidTime = (5 + rRand(5)) * 1024;
				if (cptr->packId >= 0) {
					Packs[cptr->packId].alert = TRUE;
				}
			}

			pdistMulti = TRUE;
			for (int pNo = 0; pNo < pCh; pNo++) {
				if (!(pdist[pNo] > 4096.f)) pdistMulti = FALSE;
			}
			if (pdistMulti)
			{
				if (cptr->packId >= 0) {
					if (!Packs[cptr->packId]._alert) {
						cptr->State = 0;
						SetNewTargetPlace(cptr, 2048.f);
						goto TBEGIN;
					}
				} else {
					cptr->State = 0;
					SetNewTargetPlace(cptr, 2048.f);
					goto TBEGIN;
				}
			}
		} else if (cptr->packId >= 0) Packs[cptr->packId].alert = TRUE;


		nv.x = playerdx[0];
		nv.z = playerdz[0];
		nv.y = 0;
		NormVector(nv, 2048.f);
		cptr->tgx = cptr->pos.x - nv.x;
		cptr->tgz = cptr->pos.z - nv.z;
		cptr->tgtime = 0;
	}

	if (pdist[0] > (ctViewR + 20) * 256 && cptr->AI > 0)
		if (ReplaceCharacterForward(cptr)) goto TBEGIN;


	//======== exploring area ===============//
	if (!cptr->State)
	{
		cptr->AfraidTime = 0;
		pdistMulti = FALSE;
		for (int pNo = 0; pNo < pCh; pNo++) {
			if (pdist[pNo] < 812.f) pdistMulti = TRUE;
		}
		if (pdistMulti)
		{
			cptr->State = 1;
			cptr->AfraidTime = (5 + rRand(5)) * 1024;
			cptr->Phase = DinoInfo[cptr->CType].runAnim;
			goto TBEGIN;
		}

		if (cptr->packId >= 0) {
			float leaderdx = Packs[cptr->packId].leader->pos.x - cptr->pos.x;
			float leaderdz = Packs[cptr->packId].leader->pos.z - cptr->pos.z;
			float leaderdist = (float)sqrt(leaderdx * leaderdx + leaderdz * leaderdz);

			if (cptr->followLeader) {
				if (leaderdist < DinoInfo[cptr->CType].packDensity * 128 * 0.6)
				{
					cptr->followLeader = false;
					SetNewTargetPlace(cptr, 2048.f);
					goto TBEGIN;
				}
			}
			else {
				if (leaderdist > DinoInfo[cptr->CType].packDensity * 128 * 1.3)
				{
					cptr->followLeader = true;
				}
			}

		}

		if (cptr->followLeader) {
			cptr->tgx = Packs[cptr->packId].leader->pos.x;
			cptr->tgz = Packs[cptr->packId].leader->pos.z;
		}
		else if (tdist < 456)
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

			if (DinoInfo[cptr->CType].idleCount)
			{

				bool idlePhase = false;
				for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
					if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) idlePhase = true;
				}


				if (idlePhase) {
					if (rRand(128) > AIInfo[cptr->Clone].idleStart && cptr->Phase == DinoInfo[cptr->CType].idleAnim[DinoInfo[cptr->CType].idleCount - 1])
						cptr->Phase = DinoInfo[cptr->CType].walkAnim;
					else cptr->Phase = DinoInfo[cptr->CType].idleAnim[rRand(DinoInfo[cptr->CType].idleCount - 1)];
					goto ENDPSELECT;
				}
				if (rRand(128) > 120) cptr->Phase = DinoInfo[cptr->CType].idleAnim[0];
				else cptr->Phase = DinoInfo[cptr->CType].walkAnim;

			} else cptr->Phase = DinoInfo[cptr->CType].walkAnim;

		}
		else cptr->Phase = DinoInfo[cptr->CType].runAnim;

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

	//float attackDist = 1024.f;
	//if (DinoInfo[cptr->CType].DangerFish) {
	//	attackDist = DinoInfo[cptr->CType].aggress;
	//}

	float playerdx = PlayerX - cptr->pos.x - cptr->lookx * 100 *cptr->scale;
	float playerdz = PlayerZ - cptr->pos.z - cptr->lookz * 100 *cptr->scale;
	float pdist = (float)sqrt(playerdx * playerdx + playerdz * playerdz);

	//REMOVED - turny !!!
	//if (cptr->State == 2)
	//{
	//	NewPhase = TRUE;
	//	cptr->State = 1;
	//}

	float tv;
	switch (cptr->Clone) {
	 case AI_FISH: tv = 1024.f;
	 case AI_MOSA: tv = 5024.f;
	}

	// JUMP & IDLE PARTICLES

	//int Scal = ((cptr->scale * 2) - 1);
	if (pdist < (ctViewR + 20) * 256) {	//Only create particles within player render distance
		if (DinoInfo[cptr->CType].partCnt[cptr->Phase]) {
			if (cptr->FTime > DinoInfo[cptr->CType].partFrame1[cptr->Phase] / cptr->pinfo->Animation[cptr->Phase].aniKPS
				&& cptr->FTime < DinoInfo[cptr->CType].partFrame2[cptr->Phase] / cptr->pinfo->Animation[cptr->Phase].aniKPS) {
				for (int i = 0; i < (int)sqrt(DinoInfo[cptr->CType].partCnt[cptr->Phase]* ((cptr->scale * 3) - 2)); i++) {
					float xo = (int)siRand((int)DinoInfo[cptr->CType].partDist[cptr->Phase]* cptr->scale) + cptr->pos.x +
						((cos(cptr->alpha)  * ((cptr->scale * 1.5) - 0.5) * DinoInfo[cptr->CType].partOffset[cptr->Phase]));
					float zo = (int)siRand((int)DinoInfo[cptr->CType].partDist[cptr->Phase] * cptr->scale) + cptr->pos.z +
						((sin(cptr->alpha)  * ((cptr->scale * 1.5) - 0.5) * DinoInfo[cptr->CType].partOffset[cptr->Phase]));
					AddElementsA(xo,
						GetLandUpH(xo, zo),
						zo,
						2,
						5,
						DinoInfo[cptr->CType].partMag[cptr->Phase],
						DinoInfo[cptr->CType].partAngled[cptr->Phase],
						cptr->alpha);
					if (DinoInfo[cptr->CType].partCircle[cptr->Phase]) AddWCircle(xo, zo, 1.2);
				}
			}
		}
	}


	if (GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z) > 180 * cptr->scale)
		cptr->StateF |= csONWATER;
	else
		cptr->StateF &= (!csONWATER);

	bool playerInWater = GetLandUpH(PlayerX, PlayerZ) - GetLandH(PlayerX, PlayerZ) > 0;


	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) goto NOTHINK;

	//============================================//
	if (!MyHealth) cptr->State = 0;

	int ao = 0;
	if (DinoInfo[cptr->CType].DangerFish)ao = OptAgres;

	if (!cptr->State)
	{

		bool attackmode = pdist <= ctViewR * DinoInfo[cptr->CType].aggress + ao / AIInfo[cptr->Clone].agressMulti && playerInWater && !DinoInfo[cptr->CType].dontSwimAway
			&& MyHealth && !ObservMode && !DEBUG;
		if (attackmode)	cptr->AfraidTime = (int)(10.f) * 1024;
		if (cptr->packId >= 0 && MyHealth) {
			if (attackmode) Packs[cptr->packId].alert = TRUE;
			if (Packs[cptr->packId]._alert) attackmode = TRUE;
		}

		if (attackmode) {
			cptr->State = 1;
			cptr->turny = 0;
			cptr->lastTBeta = cptr->beta;
			//cptr->AfraidTime = (int)(10.f) * 1024;
			//goto TBEGIN;
		} else {

			if (cptr->packId >= 0) {
				float leaderdx = Packs[cptr->packId].leader->pos.x - cptr->pos.x;
				float leaderdz = Packs[cptr->packId].leader->pos.z - cptr->pos.z;
				float leaderdy = Packs[cptr->packId].leader->pos.y - cptr->depth;
				float leaderdist = (float)sqrt(leaderdx * leaderdx + leaderdz * leaderdz);
				float leaderdisty = (float)sqrt(leaderdist * leaderdist + leaderdy * leaderdy);


				if (cptr->followLeader) {
					if (leaderdist < DinoInfo[cptr->CType].packDensity * 128 * 0.6)
					{
						cptr->followLeader = false;
						SetNewTargetPlaceFish(cptr, tv);
						goto TBEGIN;
					}
				}
				else {
					if (leaderdist > DinoInfo[cptr->CType].packDensity * 128 * 1.3)
					{
						cptr->followLeader = true;
						cptr->turny = 0;
						cptr->lastTBeta = cptr->beta;
					}
				}

			}

			if (cptr->followLeader) {
				cptr->tgx = Packs[cptr->packId].leader->pos.x;
				cptr->tgz = Packs[cptr->packId].leader->pos.z;
				cptr->tdepth = Packs[cptr->packId].leader->depth;

			} else if (tdist2 < 456) // Ignore vertical
			{
				SetNewTargetPlaceFish(cptr, tv);
				goto TBEGIN;
			}
		}
	}

	if (cptr->State)
	{
		if (pdist > ctViewR * DinoInfo[cptr->CType].aggress + ao / AIInfo[cptr->Clone].agressMulti || !playerInWater)
		{
			cptr->AfraidTime -= TimeDt;

			if (cptr->packId >= 0) {
				if (cptr->AfraidTime <= 0) {

					if (!Packs[cptr->packId]._alert) {
						cptr->AfraidTime = 0;
						cptr->State = 0;
						SetNewTargetPlaceFish(cptr, tv);
						goto TBEGIN;
					}

				} else Packs[cptr->packId].alert = TRUE;
			} else if (cptr->AfraidTime <= 0) {
				cptr->AfraidTime = 0;
				cptr->State = 0;
				SetNewTargetPlaceFish(cptr, tv);
				goto TBEGIN;
			}




		}

		if (DinoInfo[cptr->CType].DangerFish) {
			cptr->tgx = PlayerX;
			cptr->tgz = PlayerZ;
			cptr->tgtime = 0;
			cptr->tdepth = PlayerY;


			// Mosa Target Depth Failsafes
			if (cptr->tdepth > GetLandUpH(cptr->tgx, cptr->tgz) - (cptr->spcDepth * 0.75)) {
				cptr->tdepth = GetLandUpH(cptr->pos.x, cptr->pos.z) - (cptr->spcDepth * 0.75);
			}

			//Target above the player so it can get to jumping depth in time.
			if (AIInfo[cptr->Clone].jumper) {
				if (cptr->depth < cptr->tdepth) {
					cptr->tdepth += (cptr->tdepth - cptr->depth) * 3;
					//float haw = (GetLandUpH(cptr->tgx, cptr->tgz) - GetLandH(cptr->tgx, cptr->tgz));
					//if (haw) cptr->tdepth *= (cptr->tdepth - GetLandH(cptr->tgx, cptr->tgz)) / haw;
				}
			}

			if (cptr->packId >= 0) {
				Packs[cptr->packId].alert = true;
			}

		}
		else
		{
			nv.x = playerdx;
			nv.z = playerdz;
			nv.y = 0;
			NormVector(nv, 2048.f);
			cptr->tgx = cptr->pos.x - nv.x;
			cptr->tgz = cptr->pos.z - nv.z;

			cptr->tdepth = GetLandH(cptr->pos.x, cptr->pos.z) +
				((GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z)) / 2);
		}

		cptr->tgtime = 0;

		if (cptr->Phase != DinoInfo[cptr->CType].jumpAnim){
			if (AIInfo[cptr->Clone].jumper && DinoInfo[cptr->CType].DangerFish) {
				if (cptr->depth > GetLandUpH(cptr->pos.x, cptr->pos.z) - (cptr->spcDepth * 0.95)){
					float pUp = PlayerY - GetLandUpH(PlayerX, PlayerZ); //jump later if the player is on a low bridge, not at all if too high
					if (pUp < 0) pUp = 0;
					float md = ((DinoInfo[cptr->CType].jumpRange * cptr->speed_jump) - (pUp * 1.3)) * cptr->scale;
					if (pdist < md && pdist > md - 200)//1200
						if (AngleDifference(cptr->alpha, FindVectorAlpha(playerdx, playerdz)) < 0.2f) {

							Vector3d pv;
							pv.x = PlayerX;
							pv.z = PlayerZ;

							if (!CheckPlaceCollisionFish(cptr, pv, cptr->depth,
								DinoInfo[cptr->CType].maxDepth,
								DinoInfo[cptr->CType].minDepth)) {

								cptr->Phase = DinoInfo[cptr->CType].jumpAnim;
								NewPhase = TRUE;
								cptr->FTime = 0;
								cptr->bend = 0;
								cptr->bdepth = 0;

							}

						}
				}
			}
		}

		if (pdist < DinoInfo[cptr->CType].killDist * cptr->scale && DinoInfo[cptr->CType].killDist > 0) {
			float killAlt = cptr->spcDepth;
			if (killAlt < 256) killAlt = 256;
			if (AIInfo[cptr->Clone].jumper && cptr->Phase == DinoInfo[cptr->CType].jumpAnim) killAlt += 80;
			if (fabs(PlayerY - cptr->pos.y) < killAlt + 20 * cptr->scale)
			{

				if (DinoInfo[cptr->CType].killTypeCount > 0) {

					cptr->vspeed /= 8.0f;
					cptr->State = 1;
					cptr->Phase = DinoInfo[cptr->CType].killType[cptr->killType].anim;
					if (DinoInfo[cptr->CType].killType[cptr->killType].dontloop) cptr->FTime = 0;
					//cptr->FTime = 0;
					AddDeadBody(cptr,
						DinoInfo[cptr->CType].killType[cptr->killType].hunteranim,
						DinoInfo[cptr->CType].killType[cptr->killType].scream);
				}
				else {
					AddDeadBody(cptr, HUNT_EAT, TRUE);
					cptr->State = 0;
				}

				cptr->aquaticIdle = false;

			}
		}
		

	}

NOTHINK:
	if (pdist < 2048) cptr->NoFindCnt = 0;
	if (cptr->NoFindCnt) cptr->NoFindCnt--;
	else
	{
		cptr->tgalpha = CorrectedAlpha(FindVectorAlpha(targetdx, targetdz), cptr->alpha);//FindVectorAlpha(targetdx, targetdz);
		if (cptr->State && pdist > 20000)
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

		if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) {
			if (DinoInfo[cptr->CType].killType[cptr->killType].dontloop) {
				cptr->Phase = DinoInfo[cptr->CType].walkAnim;
				cptr->State = 0;
			}
		}


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

	/*
	if (cptr->FTime >= cptr->pinfo->Animation[cptr->Phase].AniTime)
	{
		cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;
		NewPhase = TRUE;

	}
	*/

	if (cptr->State) cptr->aquaticIdle = false;
	else if (DinoInfo[cptr->CType].idleCount > 0) {
		for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
			if (NewPhase && _Phase == DinoInfo[cptr->CType].idleAnim[i]) {
				cptr->aquaticIdle = false;
			}
		}
	}

	if (NewPhase) {
		if (!cptr->State) {
			cptr->Phase = DinoInfo[cptr->CType].walkAnim;
			if (DinoInfo[cptr->CType].idleCount){
				if (!cptr->aquaticIdle && rRand(128) > AIInfo[cptr->Clone].idleStart
					&& (MyHealth || !DinoInfo[cptr->CType].killType[cptr->killType].carryCorpse)
					) { // Don't play idles when carrying hunters corpse
					cptr->aquaticIdle = true;
				}

				if (cptr->aquaticIdle &&
					MyHealth && // Don't play idles when carrying hunters corpse
					cptr->depth > GetLandUpH(cptr->pos.x, cptr->pos.z) - (cptr->spcDepth * 0.8) &&
					fabs(cptr->beta) < pi / 32 &&
					fabs(cptr->gamma) < pi / 32 &&
					fabs(cptr->bend) < pi / 32) {

					cptr->Phase = DinoInfo[cptr->CType].idleAnim[rRand(DinoInfo[cptr->CType].idleCount - 1)];
					NewPhase = TRUE;
					cptr->FTime = 0;
					goto ENDPSELECT;
				}

			}
		} else cptr->Phase = DinoInfo[cptr->CType].runAnim;

	}

	/*
	if (!cptr->State) cptr->Phase = DinoInfo[cptr->CType].walkAnim;
	else if (fabs(cptr->tgalpha - cptr->alpha) < 1.0 ||
		fabs(cptr->tgalpha - cptr->alpha) > 2 * pi - 1.0)
		cptr->Phase = DinoInfo[cptr->CType].runAnim;
	else cptr->Phase = DinoInfo[cptr->CType].walkAnim;
	*/

	//if (cptr->StateF & csONWATER) cptr->Phase = RAP_SWIM;
	//if (cptr->Slide > 40) cptr->Phase = RAP_SLIDE;


ENDPSELECT:

	//====== process phase changing ===========//
	if ((_Phase != cptr->Phase) || NewPhase)
	{

		/*
		bool idp = false;

		for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
			if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) idp = true;
		}
		
		if (cptr->Phase == DinoInfo[cptr->CType].jumpAnim || idp) {
			ActivateCharacterFx(cptr);
		} else {
			ActivateCharacterFxAquatic(cptr);
		}
		*/
		ActivateCharacterFxAquatic(cptr);
		if (cptr->Phase != DinoInfo[cptr->CType].walkAnim && cptr->Phase != DinoInfo[cptr->CType].runAnim) {
			ActivateCharacterFx(cptr);
		}



	}

	if (_Phase != cptr->Phase)
	{
		//==== set proportional FTime for better morphing =//
		//if (MORPHP)
		//	if (_Phase <= 3 && cptr->Phase <= 3)
		
		if ((_Phase == DinoInfo[cptr->CType].runAnim ||
			_Phase == DinoInfo[cptr->CType].walkAnim) &&
			(cptr->Phase == DinoInfo[cptr->CType].runAnim ||
				cptr->Phase == DinoInfo[cptr->CType].walkAnim)) {
			cptr->FTime = _FTime * cptr->pinfo->Animation[cptr->Phase].AniTime / cptr->pinfo->Animation[_Phase].AniTime + 64;
		}
		//else if (!NewPhase) cptr->FTime = 0;

		if (cptr->PPMorphTime > 128)
		{
			cptr->PrevPhase = _Phase;
			cptr->PrevPFTime = _FTime;
			cptr->PPMorphTime = 0;
		}
	}

	cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;



	//========== rotation to tgalpha ===================//

	//OLD BACKUP
	/*
		float rspd, currspeed, tgbend;
	float dalpha = (float)fabs(cptr->tgalpha - cptr->alpha);
	float drspd = dalpha;
	if (drspd > pi) drspd = 2 * pi - drspd;


	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) goto SKIPROT;

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
	*/

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
	//if (cptr->AfraidTime) currspeed *= 2.5;

	if (dalpha > pi) currspeed *= -1;
	/*if ((cptr->StateF & csONWATER) || cptr->Phase == DinoInfo[cptr->CType].walkAnim) */currspeed /= 1.4f;

	if (cptr->AfraidTime) DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 250.f);
	else DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 460.f);

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
	if (AIInfo[cptr->Clone].jumper) {
		if (cptr->Phase == DinoInfo[cptr->CType].jumpAnim) curspeed = cptr->speed_jump;
	}

	if (DinoInfo[cptr->CType].idleCount > 0) {
		for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
			if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) {
				curspeed = cptr->speed_walk;
			}
		}
	}


	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) curspeed = 0.0f;

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

	if (AIInfo[cptr->Clone].jumper) {
		if (cptr->Phase == DinoInfo[cptr->CType].jumpAnim) cptr->vspeed = cptr->speed_jump;
	}

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

	if (cptr->Clone == AI_MOSA && cptr->Phase == DinoInfo[cptr->CType].walkAnim) {
		//cptr->depth -= cptr->beta * 10;
		cptr->depth -= cptr->beta * 25 * curspeed;

	} else {
		cptr->depth -= cptr->beta * 35 * curspeed;
	}

	float newBend = (_beta - cptr->beta) * 25;
	float max = 0.2;
	float maxIt = max / 6;

	if (fabs(cptr->bdepth - newBend) > maxIt) {
		if (newBend > cptr->bdepth) {
			cptr->bdepth += maxIt;
			//if (cptr->bdepth > max) cptr->bdepth = max; - see below
		}
		else {
			cptr->bdepth -= maxIt;
			//if (cptr->bdepth < -max) cptr->bdepth = -max; - see below
		}
	}
	else {
		cptr->bdepth = newBend;
	}
	if (cptr->bdepth > max) cptr->bdepth = max;
	if (cptr->bdepth < -max) cptr->bdepth = -max;

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

	if (AIInfo[cptr->Clone].jumper) {
		if (cptr->Phase == DinoInfo[cptr->CType].jumpAnim) cptr->tggamma = 0;
	}

	DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 1624.f);


	// Mosa Depth Failsafes
	if (cptr->depth > GetLandUpH(cptr->pos.x, cptr->pos.z) - (cptr->spcDepth / 2)) {
		cptr->depth = GetLandUpH(cptr->pos.x, cptr->pos.z) - (cptr->spcDepth / 2);
		cptr->tdepth = GetLandH(cptr->pos.x, cptr->pos.z) +
			((GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z)) / 2);
		cptr->lastTBeta = cptr->beta;
	}
	if (cptr->depth < GetLandH(cptr->pos.x, cptr->pos.z) + (cptr->spcDepth / 2)) {
		cptr->depth = GetLandH(cptr->pos.x, cptr->pos.z) + (cptr->spcDepth / 2);
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


	bool alertInit = FALSE;
	if (cptr->State == 2) alertInit = TRUE;
	if (cptr->packId >= 0) {
		if (!cptr->State && Packs[cptr->packId]._alert) alertInit = TRUE;
	}

	if (alertInit) {
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

		bool relax = FALSE;
		if (cptr->packId >= 0) {
			if (!cptr->AfraidTime) {
				if (!Packs[cptr->packId]._alert) relax = TRUE;
			} else Packs[cptr->packId].alert = TRUE;
		} else if (!cptr->AfraidTime) relax = TRUE;

		if (relax)
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
			if (cptr->packId >= 0) Packs[cptr->packId].alert = TRUE;
			goto TBEGIN;
		}

	}

	int targetNear = 456;

	if (cptr->Phase == DinoInfo[cptr->CType].flyAnim || cptr->Phase == DinoInfo[cptr->CType].glideAnim) {
		targetNear = 2024;
	}

	
	if (cptr->packId >= 0) {
		float leaderdx = Packs[cptr->packId].leader->pos.x - cptr->pos.x;
		float leaderdz = Packs[cptr->packId].leader->pos.z - cptr->pos.z;
		float leaderdist = (float)sqrt(leaderdx * leaderdx + leaderdz * leaderdz);

		if (cptr->followLeader) {
			if (leaderdist < DinoInfo[cptr->CType].packDensity * 128 * 0.6)
			{
				cptr->followLeader = false;
				SetNewTargetPlace_Icth(cptr, 4048.f);
				goto TBEGIN;
			}
		}
		else {
			if (leaderdist > DinoInfo[cptr->CType].packDensity * 128 * 1.3)
			{
				cptr->followLeader = true;
			}
		}

	}

	if (cptr->followLeader) {
		cptr->tgx = Packs[cptr->packId].leader->pos.x;
		cptr->tgz = Packs[cptr->packId].leader->pos.z;
	}
	else if (tdist < targetNear)
	{
		SetNewTargetPlace_Icth(cptr, 2048.f);
		goto TBEGIN;
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
	float swimLevel = DinoInfo[cptr->CType].waterLevel * cptr->scale;// 40;

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
		
		if (cptr->State) {

			bool afraid = FALSE;
			if (cptr->AfraidTime) afraid = TRUE;

			if (cptr->packId >= 0) {
				if (Packs[cptr->packId]._alert) afraid = TRUE;
			}


			if (afraid) {

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
		DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 2048.f);
		
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

			DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 2048.f);
		}
		else {
			cptr->gamma = 0;
		}
	}


	/*
	if (swimmingAnim)
	{
		cptr->gamma = 0;
	}
	else
	{
		DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 2048.f);
	}
	*/



	//============ Y movement =================//
	if ((wy >= swimLevel) && cptr->Phase != DinoInfo[cptr->CType].flyAnim && cptr->Phase != DinoInfo[cptr->CType].glideAnim
		&& cptr->Phase != DinoInfo[cptr->CType].takeoffAnim && cptr->Phase != DinoInfo[cptr->CType].landAnim)
	{
		cptr->pos.y = GetLandUpH(cptr->pos.x, cptr->pos.z) - (DinoInfo[cptr->CType].waterLevel * cptr->scale);
		//cptr->pos.y = GetLandUpH(cptr->pos.x, cptr->pos.z) - 20;
		cptr->beta /= 2;
		cptr->tggamma = 0;
	}

}







void AnimateIcthDead(TCharacter *cptr)
{
	cptr->bend = 0;

	if (cptr->Phase != DinoInfo[cptr->CType].deathType[cptr->deathType].fall && cptr->Phase != DinoInfo[cptr->CType].deathType[cptr->deathType].die
		&& !(cptr->Phase == DinoInfo[cptr->CType].deathType[cptr->deathType].sleep && cptr->Clone == AI_ICTH)
		&& !(cptr->Phase == cptr->waterDieAnim && DinoInfo[cptr->CType].waterDieCount))
	{
		cptr->deathPhase = cptr->Phase;
		if (cptr->PPMorphTime > 128)
		{
			cptr->PrevPhase = cptr->Phase;
			cptr->PrevPFTime = cptr->FTime;
			cptr->PPMorphTime = 0;
		}

		cptr->FTime = 0;
		cptr->Phase = DinoInfo[cptr->CType].deathType[cptr->deathType].fall;
		cptr->rspeed = 0;
		ActivateCharacterFx(cptr);
		return;
	}

	ProcessPrevPhase(cptr);

	float wh = GetLandUpH(cptr->pos.x, cptr->pos.z);
	float lh = GetLandH(cptr->pos.x, cptr->pos.z);
	BOOL OnWaterQ = (wh > lh);
	if (!DinoInfo[cptr->CType].waterDieCount) OnWaterQ = false;

	cptr->FTime += TimeDt;
	if (cptr->FTime >= cptr->pinfo->Animation[cptr->Phase].AniTime)
	{
		if (cptr->Phase == DinoInfo[cptr->CType].deathType[cptr->deathType].die ||
			(cptr->Phase == cptr->waterDieAnim && DinoInfo[cptr->CType].waterDieCount) ||
			(cptr->Phase == DinoInfo[cptr->CType].deathType[cptr->deathType].sleep && cptr->Clone == AI_ICTH))
		{
			if (cptr->canSleep)
			{
				cptr->FTime = 0;
				cptr->Phase = DinoInfo[cptr->CType].deathType[cptr->deathType].sleep;
				ActivateCharacterFx(cptr);
			}
			else
			{
				cptr->FTime = cptr->pinfo->Animation[cptr->Phase].AniTime - 1;
			}
		}
		else
			cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;


	}


	//======= movement ===========//
	if (cptr->Phase == DinoInfo[cptr->CType].deathType[cptr->deathType].die || 
		(cptr->Phase == cptr->waterDieAnim && DinoInfo[cptr->CType].waterDieCount) || 
		(cptr->Phase == DinoInfo[cptr->CType].deathType[cptr->deathType].sleep && cptr->Clone == AI_ICTH))
		DeltaFunc(cptr->vspeed, 0, TimeDt / 400.f);
	else
		DeltaFunc(cptr->vspeed, 0, TimeDt / 1200.f);

	cptr->pos.x += cptr->lookx * cptr->vspeed * TimeDt;
	cptr->pos.z += cptr->lookz * cptr->vspeed * TimeDt;

	if (cptr->Phase == DinoInfo[cptr->CType].deathType[cptr->deathType].fall)
	{
		if (OnWaterQ)
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

			if (OnWaterQ)
			{
				//				AddElements(cptr->pos.x + siRand(128), lh, cptr->pos.z + siRand(128), 4, 10);
				//				AddElements(cptr->pos.x + siRand(128), lh, cptr->pos.z + siRand(128), 4, 10);
				//				AddElements(cptr->pos.x + siRand(128), lh, cptr->pos.z + siRand(128), 4, 10);
				cptr->Phase = cptr->waterDieAnim;
			}
			else
			{
				cptr->Phase = DinoInfo[cptr->CType].deathType[cptr->deathType].die;
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

		cptr->canSleep = (Tranq && !OnWaterQ && cptr->Clone == AI_ICTH &&
			(cptr->deathPhase == DinoInfo[cptr->CType].walkAnim || cptr->deathPhase == DinoInfo[cptr->CType].shakeLandAnim || walkingidle));

	}
	else
	{
		ThinkY_Beta_Gamma(cptr, 140, 126, 0.6f, 0.5f);
		DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 1600.f);
	}

	if (DinoInfo[cptr->CType].waterDieCount && cptr->Phase == cptr->waterDieAnim)
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

	bool alertInit = FALSE;
	if (cptr->State == 2) alertInit = TRUE;
	if (cptr->packId >= 0) {
		if (!cptr->State && Packs[cptr->packId]._alert) alertInit = TRUE;
	}

	if (alertInit) {
		NewPhase = TRUE;
		cptr->State = 1;
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

		bool fleeMode = FALSE;
		if (pdist > attackDist || !playerAttackable || DinoInfo[cptr->CType].aggress <= 0 || !cptr->awareHunter) {
			fleeMode = TRUE;
		}
		else if (cptr->packId >= 0) Packs[cptr->packId].attack = TRUE;

		if (cptr->packId >= 0) {
			if (Packs[cptr->packId]._attack) fleeMode = FALSE;
		}

		if (!fleeMode)
		{
			attacking = true;
			cptr->tgx = PlayerX;
			cptr->tgz = PlayerZ;
			cptr->tgtime = 0;
			if (cptr->packId >= 0) {
				Packs[cptr->packId].alert = true;
			}
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


			if (cptr->packId >= 0) {
				if (cptr->AfraidTime <= 0)
				{
					if (!Packs[cptr->packId]._alert) {
						cptr->AfraidTime = 0;
						cptr->State = 0;
					}
				}
				else Packs[cptr->packId].alert = true;
			}
			else if (cptr->AfraidTime <= 0) {
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
						if (DinoInfo[cptr->CType].killType[cptr->killType].dontloop) cptr->FTime = 0;
						AddDeadBody(cptr,
							DinoInfo[cptr->CType].killType[cptr->killType].hunteranim,
							DinoInfo[cptr->CType].killType[cptr->killType].scream);
					}
					else AddDeadBody(cptr, HUNT_EAT, TRUE);

				}
				else {
					AddDeadBody(cptr, HUNT_EAT, TRUE);
					cptr->State = 0;
				}


			}
	}

	if (!cptr->State)
	{
		attacking = false;
		cptr->AfraidTime = 0;


		if (cptr->packId >= 0) {
			float leaderdx = Packs[cptr->packId].leader->pos.x - cptr->pos.x;
			float leaderdz = Packs[cptr->packId].leader->pos.z - cptr->pos.z;
			float leaderdist = (float)sqrt(leaderdx * leaderdx + leaderdz * leaderdz);

			if (cptr->followLeader) {
				if (leaderdist < DinoInfo[cptr->CType].packDensity * 128 * 0.6)
				{
					cptr->followLeader = false;
					SetNewTargetPlace_Brahi(cptr, 2048.f);
					goto TBEGIN;
				}
			}
			else {
				if (leaderdist > DinoInfo[cptr->CType].packDensity * 128 * 1.3)
				{
					cptr->followLeader = true;
				}
			}

		}

		if (cptr->followLeader) {
			cptr->tgx = Packs[cptr->packId].leader->pos.x;
			cptr->tgz = Packs[cptr->packId].leader->pos.z;
		}
		else if (tdist < 256)
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

		if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) {
			if (DinoInfo[cptr->CType].killType[cptr->killType].dontloop) {
				cptr->Phase = DinoInfo[cptr->CType].walkAnim;
				cptr->State = 0;
			}
		}

		NewPhase = TRUE;
	}

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount)  goto ENDPSELECT;

	if (NewPhase)
	{
		if (!cptr->State)
		{
			if (DinoInfo[cptr->CType].idleCount) {


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
				if (rRand(128) > 0
					&& (MyHealth || !DinoInfo[cptr->CType].killType[cptr->killType].carryCorpse)
					)
				{
					cptr->Phase = DinoInfo[cptr->CType].idleAnim[0];
				}
				else
				{
					cptr->Phase = DinoInfo[cptr->CType].walkAnim;
				}


			} else cptr->Phase = DinoInfo[cptr->CType].walkAnim;

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


	if (cptr->packId >= 0) {
		float leaderdx = Packs[cptr->packId].leader->pos.x - cptr->pos.x;
		float leaderdz = Packs[cptr->packId].leader->pos.z - cptr->pos.z;
		float leaderdist = (float)sqrt(leaderdx * leaderdx + leaderdz * leaderdz);

		if (cptr->followLeader) {
			if (leaderdist < DinoInfo[cptr->CType].packDensity * 128 * 0.6)
			{
				cptr->followLeader = false;
				SetNewTargetPlace_Brahi(cptr, 2048.f);
				goto TBEGIN;
			}
		}
		else {
			if (leaderdist > DinoInfo[cptr->CType].packDensity * 128 * 1.3)
			{
				cptr->followLeader = true;
			}
		}

	}

	if (cptr->followLeader) {
		cptr->tgx = Packs[cptr->packId].leader->pos.x;
		cptr->tgz = Packs[cptr->packId].leader->pos.z;
	}
	else if (tdist < 256)
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
		
		if (DinoInfo[cptr->CType].idleCount) {


			bool idlePhase2 = false;
			for (int i = 0; i < DinoInfo[cptr->CType].idleCount; i++) {
				if (cptr->Phase == DinoInfo[cptr->CType].idleAnim[i]) idlePhase2 = true;
			}

			if (idlePhase2)
			{

				if (rRand(128) > 90)  cptr->Phase = DinoInfo[cptr->CType].walkAnim;
				else cptr->Phase = DinoInfo[cptr->CType].idleAnim[rRand(DinoInfo[cptr->CType].idleCount - 1)];
				goto ENDPSELECT;
			}
			if (rRand(128) > 64) cptr->Phase = DinoInfo[cptr->CType].idleAnim[0];
			else cptr->Phase = DinoInfo[cptr->CType].walkAnim;


		}
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




	if (cptr->packId >= 0) {
		float leaderdx = Packs[cptr->packId].leader->pos.x - cptr->pos.x;
		float leaderdz = Packs[cptr->packId].leader->pos.z - cptr->pos.z;
		float leaderdist = (float)sqrt(leaderdx * leaderdx + leaderdz * leaderdz);

		if (cptr->followLeader) {
			if (leaderdist < DinoInfo[cptr->CType].packDensity * 128 * 0.6)
			{
				cptr->followLeader = false;
				SetNewTargetPlace(cptr, 4048.f);
				goto TBEGIN;
			}
		}
		else {
			if (leaderdist > DinoInfo[cptr->CType].packDensity * 128 * 1.3)
			{
				cptr->followLeader = true;
			}
		}

	}

	if (cptr->followLeader) {
		cptr->tgx = Packs[cptr->packId].leader->pos.x;
		cptr->tgz = Packs[cptr->packId].leader->pos.z;
	}
	else if (tdist < 1024)
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



//multiplayer
void AnimateMHunters() {

	//loop through hunters

	for (int c = 0; c < 1; c++) {//temp 1 player

		Vector3d *pos = &MPlayers[c].pos;

		if (mGunShot[c] != -1) {
			int weapon = mGunShot[c];
			mGunShot[c] = -1;
			if (WeapInfo[weapon].MGSSound) {
				TSFX *shotFx = &fxGunShot[WeapInfo[weapon].SFXIndex];
				AddVoice3d(shotFx->length, shotFx->lpData, pos->x, pos->y, pos->z);
			}
			MakeNoise(*pos, ctViewR * 200 * WeapInfo[weapon].Loud);
		}

		if (mHunterCall[c] != -1) {
			int targetCreature = mHunterCall[c];
			mHunterCall[c] = -1;
			int callType = mHunterCallType[c];
			mHunterCallType[c] = -1;
			TSFX *callFx = &fxCall[targetCreature][callType];
			AddVoice3d(callFx->length, callFx->lpData, pos->x, pos->y, pos->z);
		}

	}
	
}



void AnimateCharacters()
{
	//if (!RunMode) return;
	TCharacter *cptr;
	if (TrophyMode) {

		for (CurDino = 0; CurDino < ChCount; CurDino++)
		{

			//LandingList.list[DinoInfo[Characters[ChCount].CType].trophyType[DinoInfo[Characters[ChCount].CType].tCounter].trophyPos].x

			cptr = &Characters[CurDino];

			if (cptr->animateTrophy) {
				cptr->FTime += TimeDt;
				if (cptr->FTime >= cptr->pinfo->Animation[cptr->Phase].AniTime)
				{
					cptr->FTime %= cptr->pinfo->Animation[cptr->Phase].AniTime;
				}
			}
		}

		return;
	}

	if (Multiplayer && !Host) {
		
		for (CurDino = 0; CurDino < 6/*ChCount*/; CurDino++)
		{
			cptr = &Characters[CurDino];

			AnimateMClientCharacter(cptr);
		}
		

		return;
	}

	//packs
	for (int packN = 0; packN < PackCount; packN++) {

		Packs[packN]._alert = Packs[packN].alert;
		Packs[packN]._attack = Packs[packN].attack;
		Packs[packN].alert = FALSE;
		Packs[packN].attack = FALSE;

	}

	for (CurDino = 0; CurDino < ChCount; CurDino++)
	{
		cptr = &Characters[CurDino];
		if (cptr->StateF == 0xFF) continue;
		cptr->tgtime += TimeDt;

		// replace pack leader
		if (cptr->Health && cptr->packId >= 0) {
			if (!Packs[cptr->packId].leader->Health) Packs[cptr->packId].leader = cptr;
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
					SetNewTargetPlaceFish(cptr, 5048.f);
					break;
				case AI_FISH:
					SetNewTargetPlaceFish(cptr, 1024.f);
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

		if (cptr->AfraidTime <= 0) { cptr->awareHunter = FALSE; }

		switch (cptr->Clone)
		{
		case AI_MOSA:
		case AI_FISH:
			AnimateFish(cptr);
			break;
		case AI_BRACH:
			if (cptr->Health) AnimateBrahiOld(cptr);
			else AnimateDeadCommon(cptr);
			break;
		case AI_BRACHDANGER:
		case AI_LANDBRACH:
			if (cptr->Health) AnimateBrahi(cptr);
			else AnimateDeadCommon(cptr);
			break;
		case AI_ICTH:
			if (cptr->Health) AnimateIcth(cptr);
			else AnimateIcthDead(cptr);
			break;
		case AI_MOSH:
		case AI_PIG:
		case AI_GALL:
		case AI_DIMET:
			if (cptr->Health) AnimateClassicAmbient(cptr);
			else AnimateDeadCommon(cptr);
			break;
		case AI_DIMOR:
		case AI_PTERA:
			if (cptr->Health) AnimateDimor(cptr);
			else AnimateIcthDead(cptr);
			break;
		case AI_HUNTDOG:
			//	HUNTDOG TEMPP DISABLED
			/* if (cptr->Health) AnimateHuntdog(cptr);
			else AnimateDeadCommon(cptr);*/
			break;


		case AI_PARA:
		case AI_ANKY:
		case AI_PACH:
		case AI_STEGO:
		case AI_ALLO:
		case AI_CHASM:
		case AI_VELO:
		case AI_SPINO:
		case AI_CERAT:
		case AI_BRONT:
		case AI_HOG:
		case AI_WOLF:
		case AI_RHINO:
		case AI_DEER:
		case AI_SMILO:
		case AI_MAMM:
		case AI_BEAR:
			if (cptr->Health) AnimateHuntable(cptr);
			else AnimateDeadCommon(cptr);
			break;
		case AI_TREX:
			if (cptr->Health) AnimateTRex(cptr);
			else AnimateTRexDead(cptr);
			break;
		case AI_TITAN:
			if (cptr->Health) AnimateTitan(cptr);
			else AnimateIcthDead(cptr);
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


		if (cptr->Clone == AI_TREX) {  //===== T-Rex
			if (!cptr->State) {
				cptr->State = 2;
				cptr->awareHunter = TRUE;
			}
		}

		if (cptr->Clone != AI_TREX && !DinoInfo[cptr->CType].Aquatic && cptr->Clone != AI_HUNTDOG)
		{
			cptr->AfraidTime = (int)(10.f + (range - l) / 256.f) * 1024;
			if (cptr->State == 0) {
				cptr->State = 2;
			}
			cptr->NoFindCnt = 0;

			cptr->awareHunter = TRUE;
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
		//if (cptr->AfraidTime || cptr->State == 1) continue;

		if (cptr->Clone == AI_TREX && (cptr->AfraidTime || cptr->State == 1)) continue; //here to check if hunter detected once it starts running from fear call, trex doesn't like it tho

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
			if (cptr->State==0) {
				cptr->State = 2;
			}
			cptr->awareHunter = TRUE;
			if (cptr->Clone == AI_TREX) //===== T-Rex
				if (kALook > kASmell) cptr->State = 3;
			cptr->NoFindCnt = 0;
		}
	}
}





void PlaceTrophy()
{
	ChCount = 0;
	PrintLog("Placing Trophies...");

	for (int c = 0; c < TROPHY_COUNT; c++)
	{

		//MessageBox(hwndMain, c.tostring(), "boring", IDOK);

		if (!TrophyRoom.Body[c].ctype) continue;

			//do this in addshiptask too

		Characters[ChCount].CType = TrophyIndex[TrophyRoom.Body[c].ctype];

		if (DinoInfo[Characters[ChCount].CType].tCounter >=
			DinoInfo[TrophyIndex[TrophyRoom.Body[c].ctype]].trophyTypeCount) continue;
		

		//Characters[ChCount].CType = TrophyRoom.Body[c].ctype;

		/*
		if (c < 6) Characters[ChCount].alpha = pi / 2;
		else if (c < 12) Characters[ChCount].alpha = pi;
		else if (c < 18) Characters[ChCount].alpha = pi * 3 / 2;
		else			*/



		ResetCharacter(&Characters[ChCount]);

		
		Characters[ChCount].State = c;
		Characters[ChCount].scale = TrophyRoom.Body[c].scale;
		
		float scaleDif = Characters[ChCount].scale - (DinoInfo[Characters[ChCount].CType].Scale0 / 1000.f);
		if (scaleDif != 0) {
			if (DinoInfo[Characters[ChCount].CType].ScaleA != 0) scaleDif /= (DinoInfo[Characters[ChCount].CType].ScaleA / 1000.f);
			else scaleDif = 0;
		}
		int scaleDifx= (int)(scaleDif * DinoInfo[Characters[ChCount].CType].trophyType[DinoInfo[Characters[ChCount].CType].tCounter].xoffsetScale);
		int scaleDifz= (int)(scaleDif * DinoInfo[Characters[ChCount].CType].trophyType[DinoInfo[Characters[ChCount].CType].tCounter].zoffsetScale);
		int scaleDify = (int)(scaleDif * DinoInfo[Characters[ChCount].CType].trophyType[DinoInfo[Characters[ChCount].CType].tCounter].yoffsetScale);

		Characters[ChCount].pos.x = LandingList.list[DinoInfo[Characters[ChCount].CType].trophyType[DinoInfo[Characters[ChCount].CType].tCounter].trophyPos].x
			* 256 + 128 + DinoInfo[Characters[ChCount].CType].trophyType[DinoInfo[Characters[ChCount].CType].tCounter].xoffset +scaleDifx;
		Characters[ChCount].pos.z = LandingList.list[DinoInfo[Characters[ChCount].CType].trophyType[DinoInfo[Characters[ChCount].CType].tCounter].trophyPos].y
			* 256 + 128 + DinoInfo[Characters[ChCount].CType].trophyType[DinoInfo[Characters[ChCount].CType].tCounter].zoffset +scaleDifz;

		Characters[ChCount].Phase = DinoInfo[Characters[ChCount].CType].trophyType[DinoInfo[Characters[ChCount].CType].tCounter].anim;
		Characters[ChCount].PrevPhase = Characters[ChCount].Phase;


		Characters[ChCount].pos.y = GetLandH(Characters[ChCount].pos.x,
			Characters[ChCount].pos.z);

		Characters[ChCount].pos.y += DinoInfo[Characters[ChCount].CType].trophyType[DinoInfo[Characters[ChCount].CType].tCounter].yoffset +scaleDify;
		
		float a = (float)DinoInfo[Characters[ChCount].CType].trophyType[DinoInfo[Characters[ChCount].CType].tCounter].alpha;
		float b = (float)DinoInfo[Characters[ChCount].CType].trophyType[DinoInfo[Characters[ChCount].CType].tCounter].beta;
		float g = (float)DinoInfo[Characters[ChCount].CType].trophyType[DinoInfo[Characters[ChCount].CType].tCounter].gamma;
		Characters[ChCount].alpha = pi * 2 * a / 360.f;
		Characters[ChCount].beta = pi * 2 * b / 360.f;
		Characters[ChCount].gamma = pi * 2 * g / 360.f;

		Characters[ChCount].xdata = (int)(LandingList.list[DinoInfo[Characters[ChCount].CType].trophyType[DinoInfo[Characters[ChCount].CType].tCounter].trophyPos].x
			* 256 + 128 + DinoInfo[Characters[ChCount].CType].trophyType[DinoInfo[Characters[ChCount].CType].tCounter].xdata);
		Characters[ChCount].zdata = (int)(LandingList.list[DinoInfo[Characters[ChCount].CType].trophyType[DinoInfo[Characters[ChCount].CType].tCounter].trophyPos].y
			* 256 + 128 + DinoInfo[Characters[ChCount].CType].trophyType[DinoInfo[Characters[ChCount].CType].tCounter].zdata);
		Characters[ChCount].ydata = DinoInfo[Characters[ChCount].CType].trophyType[DinoInfo[Characters[ChCount].CType].tCounter].ydata;

		Characters[ChCount].animateTrophy = DinoInfo[Characters[ChCount].CType].trophyType[DinoInfo[Characters[ChCount].CType].tCounter].playAnim;

		DinoInfo[Characters[ChCount].CType].tCounter++;
		ChCount++;
	}
}


















void spawnPositionPackLeader(int RegionNo) {
	Characters[ChCount].pos.x = Region[RegionNo].XMin * 256
		+ abs(rRand(Region[RegionNo].XMax - Region[RegionNo].XMin) * 256);

	Characters[ChCount].pos.z = Region[RegionNo].YMin * 256
		+ abs(rRand(Region[RegionNo].YMax - Region[RegionNo].YMin) * 256);
}


void spawnPositionPackFollower(int leader) {
	//spawn on same spot, so the pack doesn't spawn over a cliff edge or something
	Characters[ChCount].pos.x = Characters[leader].pos.x;
	Characters[ChCount].pos.z = Characters[leader].pos.z;
}


void spawnHuntable(int &tr, int leader) {

replace2:
	if (leader >= 0) {
		spawnPositionPackFollower(leader);
	}
	else { //scrap this if we use huntable regions?
		Characters[ChCount].pos.x = 512 * 256 + siRand(50 * 256) * 10;
		Characters[ChCount].pos.z = 512 * 256 + siRand(50 * 256) * 10;
	}

	Characters[ChCount].pos.y = GetLandH(Characters[ChCount].pos.x,
		Characters[ChCount].pos.z);
	tr++;
	if (tr > 10240) return;

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


void spawnMapAmbient(int DinoInfoIndex, int RegionNo, int &tr, int leader) {


	Characters[ChCount].CType = DinoInfoIndex; //i9 + 1];
replaceSMA:

	if (leader >= 0) {
		spawnPositionPackFollower(leader);
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


	if (fabs(Characters[ChCount].pos.x - PlayerX) +
		fabs(Characters[ChCount].pos.z - PlayerZ) < 256 * 40)
		goto replaceSMA;

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






//multiplayer
void PlaceMHunters() {

	//1 player test
	MPlayers[0].pos.x = PlayerX;
	MPlayers[0].pos.z = PlayerZ;
	MPlayers[0].pos.y = PlayerY;

	MPlayers[0].pinfo = &MPlayerInfo[0];

	MPlayers[0].alpha = 0;

	MPlayers[0].State = 0;
	MPlayers[0].StateF = 0;
	MPlayers[0].Phase = 0;
	MPlayers[0].FTime = 0;
	MPlayers[0].PrevPhase = 0;
	MPlayers[0].PrevPFTime = 0;
	MPlayers[0].PPMorphTime = 0;
	MPlayers[0].beta = 0;
	MPlayers[0].gamma = 0;
	MPlayers[0].tggamma = 0;
	MPlayers[0].bend = 0;
	MPlayers[0].rspeed = 0;
	MPlayers[0].AfraidTime = 0;
	MPlayers[0].BloodTTime = 0;
	MPlayers[0].BloodTime = 0;
	MPlayers[0].lookx = (float)cos(MPlayers[0].alpha);
	MPlayers[0].lookz = (float)sin(MPlayers[0].alpha);
	MPlayers[0].scale = 1;

	HunterCount = 1;

}







void PlaceCharacters()
{

	//TEST
/*
if (Tranq) MessageBox(NULL, "TRANQ", "Carnivores Termination", IDOK | MB_SYSTEMMODAL | MB_ICONEXCLAMATION);
if (ObservMode) MessageBox(NULL, "OBSERV", "Carnivores Termination", IDOK | MB_SYSTEMMODAL | MB_ICONEXCLAMATION);
if (ScentMode) MessageBox(NULL, "SCENT", "Carnivores Termination", IDOK | MB_SYSTEMMODAL | MB_ICONEXCLAMATION);
if (CamoMode) MessageBox(NULL, "CAMO", "Carnivores Termination", IDOK | MB_SYSTEMMODAL | MB_ICONEXCLAMATION);
if (DoubleAmmo) MessageBox(NULL, "DOUBLE", "Carnivores Termination", IDOK | MB_SYSTEMMODAL | MB_ICONEXCLAMATION);
if (RadarMode) MessageBox(NULL, "RADAR", "Carnivores Termination", IDOK | MB_SYSTEMMODAL | MB_ICONEXCLAMATION);
*/


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

		Characters[ChCount].packId = -1;
		ResetCharacter(&Characters[ChCount]);

		if (Characters[ChCount].Clone == AI_DIMET ||
			Characters[ChCount].Clone == AI_PTERA)
			Characters[ChCount].pos.y += 2048.f;

		Characters[ChCount].tgx = Characters[ChCount].pos.x;
		Characters[ChCount].tgz = Characters[ChCount].pos.z;
		Characters[ChCount].tgtime = 0;


		ChCount++;
	}

	//place hunting dog
	DogMode = false;
	if (DogMode) {
		tr = 0;
		Characters[ChCount].CType = AI_to_CIndex[AI_HUNTDOG];
		replacehuntDog:
		Characters[ChCount].pos.x = PlayerX + siRand(5) * 256;
		Characters[ChCount].pos.z = PlayerZ + siRand(5) * 256;
		Characters[ChCount].pos.y = GetLandH(Characters[ChCount].pos.x,
			Characters[ChCount].pos.z);
		tr++;
		if (tr < 10240 && CheckPlaceCollisionP(Characters[ChCount].pos)) goto replacehuntDog;
		Characters[ChCount].tgx = Characters[ChCount].pos.x;
		Characters[ChCount].tgz = Characters[ChCount].pos.z;
		Characters[ChCount].tgtime = 0;
		
		Characters[ChCount].packId = -1;
		ResetCharacter(&Characters[ChCount]);
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
					if (DinoInfo[DinoInfoIndex].packMax != DinoInfo[DinoInfoIndex].packMin) {
						for (int i = 0; i < DinoInfo[DinoInfoIndex].packMax - DinoInfo[DinoInfoIndex].packMin; i++) {
							if (1 == rRand(2)) packNo++;
						}
					}
				}

				//pack members
				if (packNo > 1) {
					Packs[PackCount].leader = &Characters[leaderIndex];
					Packs[PackCount].alert = FALSE;
					Packs[PackCount].attack = FALSE;
					Packs[PackCount]._alert = FALSE;
					Packs[PackCount]._attack = FALSE;
					Characters[leaderIndex].packId = PackCount;
					for (int packN = 0; packN < packNo - 1; packN++) {
						Characters[ChCount].packId = PackCount;
						spawnMapAmbient(DinoInfoIndex, RegionNo, tr, leaderIndex);
					}
					PackCount++;
				}
				else Characters[leaderIndex].packId = -1;

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

		


		int DinoInfoIndex = Characters[ChCount].CType;

		int leaderIndex = ChCount;
		// pack leaders
		spawnHuntable(tr, -1);

		//pack size
		int packNo = 1;
		if (DinoInfo[DinoInfoIndex].packMax > 1) {
			packNo = DinoInfo[DinoInfoIndex].packMin;
			if(DinoInfo[DinoInfoIndex].packMax != DinoInfo[DinoInfoIndex].packMin){
				for (int i = 0; i < DinoInfo[DinoInfoIndex].packMax - DinoInfo[DinoInfoIndex].packMin; i++) {
					if (1 == rRand(2)) packNo++;
				}
			}
		}

		//pack members
		if (packNo > 1) {
			Packs[PackCount].leader = &Characters[leaderIndex];
			Packs[PackCount].alert = FALSE;
			Packs[PackCount].attack = FALSE;
			Packs[PackCount]._alert = FALSE;
			Packs[PackCount]._attack = FALSE;
			Characters[leaderIndex].packId = PackCount;
			for (int packN = 0; packN < packNo - 1; packN++) {
				Characters[ChCount].packId = PackCount;
				Characters[ChCount].CType = DinoInfoIndex;
				spawnHuntable(tr, leaderIndex);
			}
			PackCount++;
		}
		else Characters[leaderIndex].packId = -1;

		if (tr > 10500) break;

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

		//if (DinoInfo[cptr->CType].Aquatic) {	//Also hunter corpse when killed by aquatic creature
			float bendcmosa = (float)cos(fiMosa);
			float bendsmosa = (float)sin(fiMosa);
			bz = bendcmosa * zz + bendsmosa * yy;
			by = bendcmosa * yy + bendsmosa * zz;
			yy = by;
			zz = bz;
		//}

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