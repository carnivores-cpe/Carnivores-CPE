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
/*	char buff[100];
	sprintf(buff, "\n AI= %i", DinoInfo[cptr->CType].Clone);
	PrintLog(buff);

	char buff2[100];
	sprintf(buff2, " Ph= %i", cptr->Phase);
	PrintLog(buff2);*/

	cptr->PPMorphTime += TimeDt;
	if (cptr->PPMorphTime > PMORPHTIME) cptr->PrevPhase = cptr->Phase;

	cptr->PrevPFTime += TimeDt;
	cptr->PrevPFTime %= cptr->pinfo->Animation[cptr->PrevPhase].AniTime;
	cptr->PrevPFTime %= cptr->pinfo->Animation[cptr->PrevPhase].AniTime;
}


void ActivateCharacterFxAquatic(TCharacter *cptr)
{
	if (cptr->CType) //== not hunter ==//
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
	
	//char buff[100];
	//sprintf(buff, "\n AI= %i", DinoInfo[cptr->CType].Clone);
	//PrintLog(buff);

	//char buff2[100];
	//sprintf(buff2, " Ph= %i", cptr->Phase);
	//PrintLog(buff2);

	if (cptr->CType) //== not hunter ==//
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
	//cptr->AI = DinoInfo[cptr->CType].AI;
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

	cptr->claimed = false;

	cptr->tracker = -1;
	cptr->RTime = 0;

	if (cptr->Clone == AI_BRACH ||
		cptr->Clone == AI_BRACHDANGER ||
		cptr->Clone == AI_ICTH ||
		cptr->Clone == AI_FISH ||
		cptr->Clone == AI_MOSA) {
		cptr->cpcpAquatic = TRUE;
	}
	else cptr->cpcpAquatic = FALSE;

	cptr->currentIdleGroup = -1;
	cptr->currentIdle2Group = -1;

	cptr->awareHunter = FALSE;
	cptr->heardShot = FALSE;

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

	cptr->lastTBeta = 0;
	cptr->turny = 0;
	cptr->bdepth = (float)0;

	cptr->lookx = (float)cos(cptr->alpha);
	cptr->lookz = (float)sin(cptr->alpha);

	cptr->Health = DinoInfo[cptr->CType].Health0;
	if (OptAgres > 128) cptr->Health = (cptr->Health*OptAgres) / 128;

	cptr->scale = (float)(DinoInfo[cptr->CType].Scale0 + rRand(DinoInfo[cptr->CType].ScaleA)) / 1000.f;

	//When does need to get set? not here huh?
	//cptr->RType = spawnGroup[cptr->SpawnGroupType].spawnRegionCh;

	cptr->followLeader = false;

	cptr->aquaticIdle = false;

	cptr->spcDepth = DinoInfo[cptr->CType].spacingDepth + (cptr->scale * 500) - 500;

	cptr->showSonar = FALSE;

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




int CheckPlaceCollisionP(Vector3d &v, bool aquatic)
{
	int ccx = (int)v.x / 256;
	int ccz = (int)v.z / 256;

	if (ccx < 4 || ccz < 4 || ccx>1008 || ccz>1008) return 1;

	int F = (FMap[ccz][ccx - 1] | FMap[ccz - 1][ccx] | FMap[ccz - 1][ccx - 1] |
		FMap[ccz][ccx] |
		FMap[ccz + 1][ccx] | FMap[ccz][ccx + 1] | FMap[ccz + 1][ccx + 1]);

	if (aquatic) {
		if (F & fmNOWAY) return 1;
	}
	else if (F & (fmWater + fmNOWAY)) return 1;


	


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


int CheckPlaceCollisionFishP(Vector3d &v, int minDepth, int maxDepth)
{
	int ccx = (int)v.x / 256;
	int ccz = (int)v.z / 256;

	if (ccx < 4 || ccz < 4 || ccx>1008 || ccz>1008) return 1;
	
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

	if (spawnGroup[cptr->SpawnGroupType].avoidRegionCh) {
		for (int ar = 0; ar < spawnGroup[cptr->SpawnGroupType].avoidRegionCh; ar++) {
			if (ccx > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMin &&
				ccx < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMax &&
				ccz > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMin &&
				ccz < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMax) return 1;
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

	if (spawnGroup[cptr->SpawnGroupType].avoidRegionCh) {
		for (int ar = 0; ar < spawnGroup[cptr->SpawnGroupType].avoidRegionCh; ar++) {
			if (ccx > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMin &&
				ccx < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMax &&
				ccz > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMin &&
				ccz < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMax) return 1;
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

	if (spawnGroup[cptr->SpawnGroupType].avoidRegionCh) {
		for (int ar = 0; ar < spawnGroup[cptr->SpawnGroupType].avoidRegionCh; ar++) {
			if (ccx > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMin &&
				ccx < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMax &&
				ccz > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMin &&
				ccz < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMax) return 1;
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

int CheckPlaceCollisionMicro(TCharacter *cptr, Vector3d &v, BOOL wc, BOOL mc)
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

	if (spawnGroup[cptr->SpawnGroupType].avoidRegionCh) {
		for (int ar = 0; ar < spawnGroup[cptr->SpawnGroupType].avoidRegionCh; ar++) {
			if (ccx > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMin &&
				ccx < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMax &&
				ccz > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMin &&
				ccz < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMax) return 1;
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
					if (r < CR && (!TreeTable[ob] || !cptr->gottaClimb)) return 1;
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

	if (spawnGroup[cptr->SpawnGroupType].avoidRegionCh) {
		for (int ar = 0; ar < spawnGroup[cptr->SpawnGroupType].avoidRegionCh; ar++) {
			if (ccx > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMin &&
				ccx < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMax &&
				ccz > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMin &&
				ccz < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMax) return 1;
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

	if (spawnGroup[cptr->SpawnGroupType].avoidRegionCh) {
		for (int ar = 0; ar < spawnGroup[cptr->SpawnGroupType].avoidRegionCh; ar++) {
			if (ccx > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMin &&
				ccx < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMax &&
				ccz > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMin &&
				ccz < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMax) return 1;
		}
	}

	int limit = 550;//550
	int range = 256;//256bluz
	if (wc) {
		if ((GetLandUpH(v.x, v.z) - GetLandH(v.x, v.z)) > limit ||
			(GetLandUpH(v.x + range, v.z) - GetLandH(v.x + range, v.z)) > limit ||
			(GetLandUpH(v.x, v.z + range) - GetLandH(v.x, v.z + range)) > limit ||
			(GetLandUpH(v.x + range, v.z + range) - GetLandH(v.x + range, v.z + range)) > limit ||
			(GetLandUpH(v.x - range, v.z) - GetLandH(v.x - range, v.z)) > limit ||
			(GetLandUpH(v.x, v.z - range) - GetLandH(v.x, v.z - range)) > limit ||
			(GetLandUpH(v.x - range, v.z - range) - GetLandH(v.x - range, v.z - range)) > limit ||
			(GetLandUpH(v.x + range, v.z - range) - GetLandH(v.x + range, v.z - range)) > limit ||
			(GetLandUpH(v.x - range, v.z + range) - GetLandH(v.x - range, v.z + range)) > limit) return 1;
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

int CheckPlaceCollisionBrahiP(Vector3d &v)
{
	int ccx = (int)v.x / 256;
	int ccz = (int)v.z / 256;

	if (ccx < 4 || ccz < 4 || ccx>1008 || ccz>1008) return 1;

	int F = (FMap[ccz][ccx - 1] | FMap[ccz - 1][ccx] | FMap[ccz - 1][ccx - 1] |
		FMap[ccz][ccx] |
		FMap[ccz + 1][ccx] | FMap[ccz][ccx + 1] | FMap[ccz + 1][ccx + 1]);

	if (!(GetLandUpH(v.x, v.z) > GetLandH(v.x, v.z))) return 1;

	int limit = 550;//550
	int range = 256;//256bluz
	if ((GetLandUpH(v.x, v.z) - GetLandH(v.x, v.z)) > limit ||
		(GetLandUpH(v.x + range, v.z) - GetLandH(v.x + range, v.z)) > limit ||
		(GetLandUpH(v.x, v.z + range) - GetLandH(v.x, v.z + range)) > limit ||
		(GetLandUpH(v.x + range, v.z + range) - GetLandH(v.x + range, v.z + range)) > limit ||
		(GetLandUpH(v.x - range, v.z) - GetLandH(v.x - range, v.z)) > limit ||
		(GetLandUpH(v.x, v.z - range) - GetLandH(v.x, v.z - range)) > limit ||
		(GetLandUpH(v.x - range, v.z - range) - GetLandH(v.x - range, v.z - range)) > limit ||
		(GetLandUpH(v.x + range, v.z - range) - GetLandH(v.x + range, v.z - range)) > limit ||
		(GetLandUpH(v.x - range, v.z + range) - GetLandH(v.x - range, v.z + range)) > limit) return 1;

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

	if (spawnGroup[cptr->SpawnGroupType].avoidRegionCh) {
		for (int ar = 0; ar < spawnGroup[cptr->SpawnGroupType].avoidRegionCh; ar++) {
			if (ccx > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMin &&
				ccx < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMax &&
				ccz > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMin &&
				ccz < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMax) return 1;
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
		else if (cptr->Clone == AI_FISH ||
			cptr->Clone == AI_MOSA) {
			p.x += lookx * 64.f;
			p.z += lookz * 64.f;
			if (CheckPlaceCollisionFish(cptr, p, cptr->depth,
				DinoInfo[cptr->CType].maxDepth,
				DinoInfo[cptr->CType].minDepth)) c++;

		}
		else if (cptr->Clone == AI_MICRO) {
			p.x += lookx * 64.f;
			p.z += lookz * 64.f;
			if (CheckPlaceCollisionMicro(cptr, p, wc, mc)) c++;
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


void SetNewTargetPlace_Icth(TCharacter *cptr, float R)
{
	Vector3d p;
	int tr = 0;


	//PrintLog("iT");//TEST20200412
replace:
	//PrintLog("-");//TEST20200412
	p.x = cptr->pos.x + siRand((int)R);
	p.z = cptr->pos.z + siRand((int)R);

	if (p.x < 512) p.x = 512;
	if (p.x > 1018 * 256) p.x = 1018 * 256;
	if (p.z < 512) p.z = 512;
	if (p.z > 1018 * 256) p.z = 1018 * 256;
	tr++;
	if (tr < 16)
		if (fabs(p.x - cptr->pos.x) + fabs(p.z - cptr->pos.z) < R / 2.f) goto replace;

	if (tr < 1024) {
		if (spawnGroup[cptr->SpawnGroupType].stayInRegion) {
			BOOL outside = true;
			for (int sr = 0; sr < spawnGroup[cptr->SpawnGroupType].spawnRegionCh; sr++) {
				if (p.x > spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].XMin * 256 &&
					p.x < spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].XMax * 256 &&
					p.z > spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].YMin * 256 &&
					p.z < spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].YMax * 256) outside = false;
			}
			if (outside) goto replace;
		}
		if (spawnGroup[cptr->SpawnGroupType].avoidRegionCh) {
			for (int ar = 0; ar < spawnGroup[cptr->SpawnGroupType].avoidRegionCh; ar++) {
				if (p.x > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMin * 256 &&
					p.x < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMax * 256 &&
					p.z > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMin * 256 &&
					p.z < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMax * 256) goto replace;
			}
		}
	}
	/*
	if (stayRegion && outsideRegion && tr > 64) {
		if (fabs(p.x - cptr->pos.x) + fabs(p.z - cptr->pos.z) > R * 25.f) {
			stayRegion = false;
			goto replace;
		}
	}
	*/

	//if (tr < 128)
	if (tr < 100)
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
		if (CheckPlaceCollisionP(p, true)) goto replace;

	cptr->tgtime = 0;
	cptr->tgx = p.x;
	cptr->tgz = p.z;
}

void SetNewTargetPlace_IcthOld(TCharacter *cptr, float R)
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
	if (tr < 16)
		if (fabs(p.x - cptr->pos.x) + fabs(p.z - cptr->pos.z) < R / 2.f) goto replace;

	if (tr < 128)
	{
		if (!waterNear(p.x, p.z, 50)) goto replace;
	}

	if (tr < 256)
		if (CheckPlaceCollisionP(p, true)) goto replace;

	cptr->tgtime = 0;
	cptr->tgx = p.x;
	cptr->tgz = p.z;
}

void SetNewTargetPlace(TCharacter *cptr, float R)
{

	//if (cptr->AI < 0) {//STILL NEED THIS FOR CLASSIC AMBIENTS
	SetNewTargetPlaceRegion(cptr, R);
	//}
	//else {
	//	SetNewTargetPlaceVanilla(cptr, R);
	//}


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
		if (CheckPlaceCollisionP(p, cptr->cpcpAquatic)) goto replace;

	cptr->tgtime = 0;
	cptr->tgx = p.x;
	cptr->tgz = p.z;
}

void SetNewTargetPlaceRegion(TCharacter *cptr, float R)
{
	Vector3d p;
	int tr = 0;
replace:
	//PrintLog("-");//TEST20200415
	p.x = cptr->pos.x + siRand((int)R);
	p.z = cptr->pos.z + siRand((int)R);

	if (p.x < 512) p.x = 512;
	if (p.x > 1018 * 256) p.x = 1018 * 256;
	if (p.z < 512) p.z = 512;
	if (p.z > 1018 * 256) p.z = 1018 * 256;
	p.y = GetLandH(p.x, p.z);
	tr++;
	if (tr < 128) {
		if (fabs(p.x - cptr->pos.x) + fabs(p.z - cptr->pos.z) < R / 2.f) goto replace;

		if (spawnGroup[cptr->SpawnGroupType].stayInRegion) {
			BOOL outside = true;
			for (int sr = 0; sr < spawnGroup[cptr->SpawnGroupType].spawnRegionCh; sr++) {
				if (p.x > spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].XMin * 256 &&
					p.x < spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].XMax * 256 &&
					p.z > spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].YMin * 256 &&
					p.z < spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].YMax * 256) outside = false;
			}
			if (outside) goto replace;
		}
		if (spawnGroup[cptr->SpawnGroupType].avoidRegionCh) {
			for (int ar = 0; ar < spawnGroup[cptr->SpawnGroupType].avoidRegionCh; ar++) {
				if (p.x > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMin * 256 &&
					p.x < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMax * 256 &&
					p.z > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMin * 256 &&
					p.z < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMax * 256) goto replace;
			}
		}

	}

	R += 512;

	if (tr < 256)
		if (CheckPlaceCollisionP(p, cptr->cpcpAquatic)) goto replace;

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
			}
			else {
				if (wy > 0) goto replace;
			}
		}
		else {
			if (wy > 400) goto replace;
			if (wy < 200) goto replace;
		}
		
		if (spawnGroup[cptr->SpawnGroupType].stayInRegion) {
			BOOL outside = true;
			for (int sr = 0; sr < spawnGroup[cptr->SpawnGroupType].spawnRegionCh; sr++) {
				if (p.x > spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].XMin * 256 &&
					p.x < spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].XMax * 256 &&
					p.z > spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].YMin * 256 &&
					p.z < spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].YMax * 256) outside = false;
			}
			if (outside) goto replace;
		}
		if (spawnGroup[cptr->SpawnGroupType].avoidRegionCh) {
			for (int ar = 0; ar < spawnGroup[cptr->SpawnGroupType].avoidRegionCh; ar++) {
				if (p.x > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMin * 256 &&
					p.x < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMax * 256 &&
					p.z > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMin * 256 &&
					p.z < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMax * 256) goto replace;
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

	if (p.x < 512) p.x = 512;
	if (p.x > 1018 * 256) p.x = 1018 * 256;
	if (p.z < 512) p.z = 512;
	if (p.z > 1018 * 256) p.z = 1018 * 256;

	tr++;
	if (tr < 128) {
		if (fabs(p.x - cptr->pos.x) + fabs(p.z - cptr->pos.z) < R * 0.7) goto replace;

		if (spawnGroup[cptr->SpawnGroupType].stayInRegion) {
			BOOL outside = true;
			for (int sr = 0; sr < spawnGroup[cptr->SpawnGroupType].spawnRegionCh; sr++) {
				if (p.x > spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].XMin * 256 &&
					p.x < spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].XMax * 256 &&
					p.z > spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].YMin * 256 &&
					p.z < spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].YMax * 256) outside = false;
			}
			if (outside) goto replace;
		}
		if (spawnGroup[cptr->SpawnGroupType].avoidRegionCh) {
			for (int ar = 0; ar < spawnGroup[cptr->SpawnGroupType].avoidRegionCh; ar++) {
				if (p.x > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMin * 256 &&
					p.x < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMax * 256 &&
					p.z > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMin * 256 &&
					p.z < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMax * 256) goto replace;
			}
		}

	}

	p.y = GetLandH(p.x, p.z);
	float wy = GetLandUpH(p.x, p.z) - GetLandH(p.x, p.z);


	int spcdm = 1;
	if (cptr->aquaticIdle) spcdm = 0.75;

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
	}
	else {
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
		
		if (spawnGroup[cptr->SpawnGroupType].stayInRegion) {
			BOOL outside = true;
			for (int sr = 0; sr < spawnGroup[cptr->SpawnGroupType].spawnRegionCh; sr++) {
				if (p.x > spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].XMin * 256 &&
					p.x < spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].XMax * 256 &&
					p.z > spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].YMin * 256 &&
					p.z < spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].YMax * 256) outside = false;
			}
			if (outside) goto replace;
		}
		if (spawnGroup[cptr->SpawnGroupType].avoidRegionCh) {
			for (int ar = 0; ar < spawnGroup[cptr->SpawnGroupType].avoidRegionCh; ar++) {
				if (p.x > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMin * 256 &&
					p.x < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMax * 256 &&
					p.z > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMin * 256 &&
					p.z < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMax * 256) goto replace;
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
		if (dbeta > pi / 8) {
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


BOOL ReplaceCharacterForward(TCharacter *cptr)
{

	if (!spawnGroup[cptr->SpawnGroupType].moveForward) return FALSE;

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

	BOOL outside = true;
	for (int sr = 0; sr < spawnGroup[cptr->SpawnGroupType].spawnRegionCh; sr++) {
		if (p.x > spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].XMin * 256 &&
			p.x < spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].XMax * 256 &&
			p.z > spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].YMin * 256 &&
			p.z < spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].YMax * 256) outside = false;
	}
	if (outside) return FALSE;
	if (spawnGroup[cptr->SpawnGroupType].avoidRegionCh) {
		for (int ar = 0; ar < spawnGroup[cptr->SpawnGroupType].avoidRegionCh; ar++) {
			if (p.x > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMin * 256 &&
				p.x < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMax * 256 &&
				p.z > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMin * 256 &&
				p.z < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMax * 256) return FALSE;
		}
	}

	if (cptr->Clone == AI_BRACH || cptr->Clone == AI_BRACHDANGER || cptr->Clone == AI_ICTH) {
		if (CheckPlaceCollisionBrahiP(p)) return FALSE;
	} else if (cptr->Clone == AI_FISH || cptr->Clone == AI_MOSA) {
			if (CheckPlaceCollisionFishP(p,
				DinoInfo[cptr->CType].minDepth,
				DinoInfo[cptr->CType].maxDepth)) return FALSE;
	} else if (CheckPlaceCollisionP(p, cptr->cpcpAquatic)) return FALSE;

//	cptr->State = 0;
	cptr->pos = p;
	ResetCharacter(cptr);
	//cptr->tgx = cptr->pos.x + siRand(2048);
	//cptr->tgz = cptr->pos.z + siRand(2048);
	if (cptr->Clone == AI_BRACH || cptr->Clone == AI_BRACHDANGER || cptr->Clone == AI_LANDBRACH) SetNewTargetPlace_Brahi(cptr, 2048.f);
	else if (cptr->Clone == AI_MOSA) SetNewTargetPlaceFish(cptr, 5048.f);
	else if (cptr->Clone == AI_FISH) SetNewTargetPlaceFish(cptr, 1024.f);
	else SetNewTargetPlace(cptr, 2048);

	if (cptr->Clone == AI_FISH || cptr->Clone == AI_MOSA) {
		cptr->pos.y = GetLandUpH(cptr->pos.x, cptr->pos.z) - ((GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z)) / 2);
	}

	if (cptr->Clone == AI_DIMOR || cptr->Clone == AI_PTERA) //===== dimor ========//
		cptr->pos.y += DinoInfo[cptr->CType].minDepth;
	return TRUE;
}



void Characters_AddSecondaryOne(TCharacter *cptr)
{

	if (!spawnGroup[cptr->SpawnGroupType].moveForward) return;

	if (ChCount > 64) return;
	Characters[ChCount].CType = cptr->CType;;
	int tr = 0;
replace1:
	tr++;
	if (tr > 128) return;
	Characters[ChCount].pos.x = PlayerX + siRand(20040);
	Characters[ChCount].pos.z = PlayerZ + siRand(20040);
	Characters[ChCount].pos.y = GetLandH(Characters[ChCount].pos.x,
		Characters[ChCount].pos.z);

	
	BOOL outside = true;
	for (int sr = 0; sr < spawnGroup[cptr->SpawnGroupType].spawnRegionCh; sr++) {
		if (cptr->pos.x > spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].XMin * 256 &&
			cptr->pos.x < spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].XMax * 256 &&
			cptr->pos.z > spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].YMin * 256 &&
			cptr->pos.z < spawnGroup[cptr->SpawnGroupType].spawnRegion[sr].YMax * 256) outside = false;
	}
	if (outside) goto replace1;
	if (spawnGroup[cptr->SpawnGroupType].avoidRegionCh) {
		for (int ar = 0; ar < spawnGroup[cptr->SpawnGroupType].avoidRegionCh; ar++) {
			if (cptr->pos.x > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMin * 256 &&
				cptr->pos.x < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].XMax * 256 &&
				cptr->pos.z > spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMin * 256 &&
				cptr->pos.z < spawnGroup[cptr->SpawnGroupType].avoidRegion[ar].YMax * 256) goto replace1;
		}
	}

	if (cptr->Clone == AI_BRACH || cptr->Clone == AI_BRACHDANGER || cptr->Clone == AI_ICTH) {
		if (CheckPlaceCollisionBrahiP(Characters[ChCount].pos))goto replace1;
	} else if (cptr->Clone == AI_FISH || cptr->Clone == AI_MOSA) {
		if (CheckPlaceCollisionFishP(Characters[ChCount].pos,
			DinoInfo[cptr->CType].minDepth,
			DinoInfo[cptr->CType].maxDepth)) goto replace1;
	} else if (CheckPlaceCollisionP(Characters[ChCount].pos, cptr->cpcpAquatic)) goto replace1;
	

	if (fabs(Characters[ChCount].pos.x - PlayerX) +
		fabs(Characters[ChCount].pos.z - PlayerZ) < 256 * 40)
		goto replace1;

	Characters[ChCount].tgx = Characters[ChCount].pos.x;
	Characters[ChCount].tgz = Characters[ChCount].pos.z;
	Characters[ChCount].tgtime = 0;

	if (cptr->Clone == AI_FISH || cptr->Clone == AI_MOSA) {
		cptr->pos.y = GetLandUpH(cptr->pos.x, cptr->pos.z) - ((GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z)) / 2);
	}

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



/*
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
*/

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
	bool stge = cptr->Phase != DinoInfo[cptr->CType].deathType[cptr->deathType].die;
	if (!DinoInfo[cptr->CType].deathType[cptr->deathType].nosleep) stge = stge && cptr->Phase != DinoInfo[cptr->CType].deathType[cptr->deathType].sleep;

	if (stge)
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
			if (Tranq && !DinoInfo[cptr->CType].deathType[cptr->deathType].nosleep)
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

	if (cptr->Clone == AI_TREX)ThinkY_Beta_Gamma(cptr, 200, 196, 0.6f, 0.5f);
	else ThinkY_Beta_Gamma(cptr, 100, 96, 0.6f, 0.5f);

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

		cptr->currentIdleGroup = -1;

		float aDist;
		aDist = ctViewR * DinoInfo[cptr->CType].aggress + OptAgres / AIInfo[cptr->Clone].agressMulti;
		if (cptr->gliding) aDist *= 2;

		if (!SurvivalMode) {
			if (pdist > aDist || ((PlayerY - cptr->pos.y > pdist) && cptr->gliding) ||
				DinoInfo[cptr->CType].aggress <= 0 || !cptr->awareHunter) {
				fleeMode = TRUE;
			}
			else if (DinoInfo[cptr->CType].defensive && cptr->Health == DinoInfo[cptr->CType].Health0) fleeMode = TRUE;
			else if (DinoInfo[cptr->CType].fearShot && cptr->Health < DinoInfo[cptr->CType].Health0) fleeMode = TRUE;
			else if (DinoInfo[cptr->CType].fearHearShot && cptr->heardShot) fleeMode = TRUE;
			else if (cptr->packId >= 0) Packs[cptr->packId].attack = TRUE;
		}

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
				if (leaderdist < cptr->packDensity * 128 * 0.6)
				{
					cptr->followLeader = false;
					SetNewTargetPlace(cptr, AIInfo[cptr->Clone].targetDistance);
					goto TBEGIN;
				}
			} else {
				if (leaderdist > cptr->packDensity * 128 * 1.3)
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

		if (cptr->State && pdist > DinoInfo[cptr->CType].weaveRange && !DinoInfo[cptr->CType].dontWeave)
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
	if (!cptr->gliding && cptr->State && pdist > FlDst) cptr->gliding = true;
    else if (cptr->pos.y < GetLandUpH(cptr->pos.x, cptr->pos.z) + 50
		&& cptr->Phase != DinoInfo[cptr->CType].takeoffAnim
		&& !(GetLandUpH(cptr->pos.x, cptr->pos.z) > GetLandH(cptr->pos.x, cptr->pos.z))) {
		cptr->gliding = false;
	}

	if (NewPhase){

		if (!cptr->State && rRand(50) == 2) cptr->gliding = true;
		
		if (cptr->gliding) {

			if (!cptr->State || fleeMode) {
				//WANDER/FLEE
				if (!cptr->State && cptr->shakeTime) cptr->shakeTime -= 1;

				if (cptr->Phase == DinoInfo[cptr->CType].flyAnim) {
					if (cptr->pos.y > GetLandUpH(cptr->pos.x, cptr->pos.z) + 5800) {
						cptr->Phase = DinoInfo[cptr->CType].glideAnim;
					}
				}
				else if (cptr->Phase == DinoInfo[cptr->CType].glideAnim) {
					
					if (!cptr->shakeTime) {
						if (cptr->pos.y < GetLandUpH(cptr->pos.x, cptr->pos.z) + 1200) {

							//lander
							if (GetLandUpH(cptr->pos.x, cptr->pos.z) > GetLandH(cptr->pos.x, cptr->pos.z)) cptr->Phase = DinoInfo[cptr->CType].flyAnim;
							else cptr->Phase = DinoInfo[cptr->CType].landAnim;
						}
					} else {
						if (cptr->pos.y < GetLandUpH(cptr->pos.x, cptr->pos.z) + 3800) {
							cptr->Phase = DinoInfo[cptr->CType].flyAnim;
						}
					}

				}
				else if (cptr->Phase == DinoInfo[cptr->CType].takeoffAnim) {
					if (cptr->pos.y > GetLandUpH(cptr->pos.x, cptr->pos.z) + 1024) {
						cptr->Phase = DinoInfo[cptr->CType].flyAnim;
					}
				}
				else if (cptr->Phase != DinoInfo[cptr->CType].landAnim){
					cptr->beta = 0;
					cptr->gamma = 0;
					//	//TITAN_SLIDE	cptr->Slide = 0;
					cptr->Phase = DinoInfo[cptr->CType].takeoffAnim;

					cptr->shakeTime = 25 + rRand(150);//lander
				}
				
			} else {

				cptr->shakeTime = 0;//lander

				if (cptr->Phase != DinoInfo[cptr->CType].takeoffAnim &&
					cptr->Phase != DinoInfo[cptr->CType].glideAnim &&
					cptr->Phase != DinoInfo[cptr->CType].flyAnim &&
					cptr->Phase != DinoInfo[cptr->CType].diveAnim) {
					cptr->beta = 0;
					cptr->gamma = 0;
					//	//TITAN_SLIDE	cptr->Slide = 0;
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
						else if (cptr->pos.y - PlayerY > pdist / (1.4 * DinoInfo[cptr->CType].divspd)) cptr->Phase = DinoInfo[cptr->CType].diveAnim;
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

			cptr->shakeTime = 0;//lander

			if (!cptr->State) {

				if (DinoInfo[cptr->CType].idleGroupCount
					&& (MyHealth || !DinoInfo[cptr->CType].killType[cptr->killType].carryCorpse)
					&& !(cptr->StateF & csONWATER)) {

					if (cptr->currentIdleGroup >= 0) {
						if (rRand(127) + 1 > (1 - DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].end) * 128
							&& (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].endOnAny
								|| cptr->Phase == DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1])) {
							cptr->Phase = DinoInfo[cptr->CType].walkAnim;
							if (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].instantRepeat) {
								cptr->currentIdleGroup = -1; //this must be done inside the if statement
							}
							else {
								cptr->currentIdleGroup = -1; //this must be done inside the if statement
								goto ENDPSELECT;
							}
						}
						else {
							cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[rRand(DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1)];
							goto ENDPSELECT;
						}
					}

					for (int idleGroupNo = 0; idleGroupNo < DinoInfo[cptr->CType].idleGroupCount; idleGroupNo++) {
						if (rRand(127) + 1 > (1 - DinoInfo[cptr->CType].idleGroup[idleGroupNo].start) * 128) cptr->currentIdleGroup = idleGroupNo;
					}
					if (cptr->currentIdleGroup >= 0) {
						if (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].startOnAny)
							cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[rRand(DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1)];
						else
							cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[0];
						goto ENDPSELECT;
					}
					else cptr->Phase = DinoInfo[cptr->CType].walkAnim;

				} else {
					cptr->Phase = DinoInfo[cptr->CType].walkAnim;
				}

			} else {
				cptr->Phase = DinoInfo[cptr->CType].runAnim;

				if (fabs(cptr->pos.y - PlayerY) > pdist / 2) {
					cptr->beta = 0;
					cptr->gamma = 0;
					cptr->gliding = TRUE;
					cptr->Phase = DinoInfo[cptr->CType].takeoffAnim;
				}

			}

		}
	}

	if (cptr->currentIdleGroup == -1 && !cptr->gliding) {
		if (!cptr->State) cptr->Phase = DinoInfo[cptr->CType].walkAnim;
		else if (fabs(cptr->tgalpha - cptr->alpha) < 1.0 ||
			fabs(cptr->tgalpha - cptr->alpha) > 2 * pi - 1.0)
			cptr->Phase = DinoInfo[cptr->CType].runAnim;
		else cptr->Phase = DinoInfo[cptr->CType].walkAnim;

	}

	if (DinoInfo[cptr->CType].canSwim) {
		if (cptr->StateF & csONWATER) cptr->Phase = DinoInfo[cptr->CType].swimAnim;
	}

	/*	//TITAN_SLIDE
	if (cptr->gliding) {
		if (cptr->Slide > 40) cptr->Phase = DinoInfo[cptr->CType].slideAnim;
	}
	*/

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
	if (cptr->currentIdleGroup >= 0) goto SKIPROT;




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

	/*	//TITAN_SLIDE
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
	*/


	//========== movement ==============================//
	cptr->lookx = (float)cos(cptr->alpha);
	cptr->lookz = (float)sin(cptr->alpha);

	float curspeed = 0;
	if (cptr->Phase == DinoInfo[cptr->CType].runAnim) curspeed = DinoInfo[cptr->CType].runspd;
	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) curspeed = DinoInfo[cptr->CType].wlkspd;
	if (cptr->Phase == DinoInfo[cptr->CType].flyAnim) curspeed = DinoInfo[cptr->CType].flyspd;
	if (cptr->Phase == DinoInfo[cptr->CType].glideAnim) curspeed = DinoInfo[cptr->CType].gldspd;
	if (cptr->Phase == DinoInfo[cptr->CType].takeoffAnim) curspeed = DinoInfo[cptr->CType].tkfspd;
	if (cptr->Phase == DinoInfo[cptr->CType].diveAnim) curspeed = DinoInfo[cptr->CType].divspd;
	if (cptr->Phase == DinoInfo[cptr->CType].landAnim) curspeed = DinoInfo[cptr->CType].lndspd;
	if (DinoInfo[cptr->CType].canSwim) {
		if (cptr->Phase == DinoInfo[cptr->CType].swimAnim) curspeed = DinoInfo[cptr->CType].swmspd;
	}

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) curspeed = 0.0f;

	/*	//TITAN_SLIDE
	if (cptr->gliding && cptr->Slide)
	{
		curspeed /= 8;
		if (drspd > pi / 2.f) curspeed = 0;
		else if (drspd > pi / 4.f) curspeed *= 2.f - 4.f*drspd / pi;
	}
	else */
	if (drspd > pi / 2.f) curspeed *= 2.f - 2.f*drspd / pi;

	if (cptr->Phase == DinoInfo[cptr->CType].flyAnim) cptr->pos.y += TimeDt / 5.f;
	if (cptr->Phase == DinoInfo[cptr->CType].takeoffAnim) cptr->pos.y += TimeDt / 4.f;
	if (cptr->Phase == DinoInfo[cptr->CType].glideAnim) cptr->pos.y -= TimeDt / 10.f;
	if (cptr->Phase == DinoInfo[cptr->CType].landAnim) cptr->pos.y -= TimeDt;
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

		/*	//TITAN_SLIDE
		//========== slide ==============//
		if (cptr->Slide && cptr->gliding)
		{
			MoveCharacter(cptr, cptr->slidex * cptr->Slide / 600.f * TimeDt * cptr->scale,
				cptr->slidez * cptr->Slide / 600.f * TimeDt * cptr->scale, !DinoInfo[cptr->CType].canSwim, TRUE);

			cptr->Slide -= TimeDt;
			if (cptr->Slide < 0) cptr->Slide = 0;
		}
		*/


		//============ Y movement =================//

		if (cptr->pos.y < GetLandH(cptr->pos.x, cptr->pos.z)) cptr->pos.y = GetLandH(cptr->pos.x, cptr->pos.z);

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
		cptr->currentIdleGroup = -1;

		float aDist;
		if (AIInfo[cptr->Clone].carnivore && (!AIInfo[cptr->Clone].iceAge || cptr->Clone == AI_WOLF)) {
			aDist = ctViewR * DinoInfo[cptr->CType].aggress + OptAgres / AIInfo[cptr->Clone].agressMulti;
		} else {
			aDist = AIInfo[cptr->Clone].agressMulti * DinoInfo[cptr->CType].aggress + OptAgres / 8;
			if (pdist < 6000 && cptr->Clone != AI_DEER) cptr->AfraidTime = 8000;
		}

		bool fleeMode = FALSE;
		if (!SurvivalMode) {
			if (pdist > aDist ||
				DinoInfo[cptr->CType].aggress <= 0 || !cptr->awareHunter) {
				fleeMode = TRUE;
			}
			else if (DinoInfo[cptr->CType].defensive && cptr->Health == DinoInfo[cptr->CType].Health0) fleeMode = TRUE;
			else if (DinoInfo[cptr->CType].fearShot && cptr->Health < DinoInfo[cptr->CType].Health0) fleeMode = TRUE;
			else if (DinoInfo[cptr->CType].fearHearShot && cptr->heardShot) fleeMode = TRUE;
			else if (cptr->packId >= 0) Packs[cptr->packId].attack = TRUE;
		}

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


	if (pdist > (ctViewR + 20) * 256)
		if (ReplaceCharacterForward(cptr)) goto TBEGIN;


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
				if (leaderdist < cptr->packDensity * 128 * 0.6)
				{
					cptr->followLeader = false;
					SetNewTargetPlace(cptr, AIInfo[cptr->Clone].targetDistance);
					goto TBEGIN;
				}
			}
			else {
				if (leaderdist > cptr->packDensity * 128 * 1.3)
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

		if (cptr->State && pdist > DinoInfo[cptr->CType].weaveRange && !DinoInfo[cptr->CType].dontWeave)
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




			if (DinoInfo[cptr->CType].idleGroupCount
				&& (MyHealth || !DinoInfo[cptr->CType].killType[cptr->killType].carryCorpse)
				&& !(cptr->StateF & csONWATER)) {

					if (cptr->currentIdleGroup >= 0) {
						if (rRand(127) + 1 > (1 - DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].end) * 128
							&& (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].endOnAny
							|| cptr->Phase == DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1])) {
							cptr->Phase = DinoInfo[cptr->CType].walkAnim;
							if (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].instantRepeat){
								cptr->currentIdleGroup = -1; //this must be done inside the if statement
							} else {
								cptr->currentIdleGroup = -1; //this must be done inside the if statement
								goto ENDPSELECT;
							}
						} else {
							cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[rRand(DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1)];
							goto ENDPSELECT;
						}
					}

					for (int idleGroupNo = 0; idleGroupNo < DinoInfo[cptr->CType].idleGroupCount; idleGroupNo++) {
						if (rRand(127) + 1 > (1 - DinoInfo[cptr->CType].idleGroup[idleGroupNo].start) * 128) cptr->currentIdleGroup = idleGroupNo;
					}
					if (cptr->currentIdleGroup >= 0) {
						if (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].startOnAny)
							cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[rRand(DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1)];
						else
							cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[0];
						goto ENDPSELECT;
					} else cptr->Phase = DinoInfo[cptr->CType].walkAnim;
					

			}
			else {
				cptr->Phase = DinoInfo[cptr->CType].walkAnim;
			}

		}
		else cptr->Phase = DinoInfo[cptr->CType].runAnim;

	if (cptr->currentIdleGroup == -1){
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
	if (cptr->currentIdleGroup >= 0) goto SKIPROT;


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
	if (cptr->Phase == DinoInfo[cptr->CType].runAnim) curspeed = DinoInfo[cptr->CType].runspd;
	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) curspeed = DinoInfo[cptr->CType].wlkspd;
	if (DinoInfo[cptr->CType].canSwim) {
		if (cptr->Phase == DinoInfo[cptr->CType].swimAnim) curspeed = DinoInfo[cptr->CType].swmspd;
	}
	if (AIInfo[cptr->Clone].jumper) {
		if (cptr->Phase == DinoInfo[cptr->CType].jumpAnim) curspeed = DinoInfo[cptr->CType].jmpspd;
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
			if (cptr->Phase == DinoInfo[cptr->CType].jumpAnim) cptr->vspeed = DinoInfo[cptr->CType].jmpspd;
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



Vector3d LookForATree(TCharacter *cptr) {

	float searchAlpha1 = cptr->tgalpha;
	float searchAlpha2 = cptr->tgalpha;
	float dalpha = 15.f;


	for (int i = 0; i < 12; i++)
	{
		searchAlpha1 = cptr->tgalpha + dalpha * pi / 180.f;
		searchAlpha2 = cptr->tgalpha - dalpha * pi / 180.f;
		Vector3d p1 = cptr->pos;
		Vector3d p2 = cptr->pos;
		float lookx1 = (float)cos(searchAlpha1);
		float lookz1 = (float)sin(searchAlpha1);
		float lookx2 = (float)cos(searchAlpha2);
		float lookz2 = (float)sin(searchAlpha2);
		for (int t = 0; t < 20; t++) {
			p1.x += lookx1 * 256.f;
			p1.z += lookz1 * 256.f;
			p2.x += lookx2 * 256.f;
			p2.z += lookz2 * 256.f;

			int ccx1 = (int)p1.x / 256;
			int ccz1 = (int)p1.z / 256;
			int ccx2 = (int)p2.x / 256;
			int ccz2 = (int)p2.z / 256;
			for (int z = -2; z <= 2; z++) {
				for (int x = -2; x <= 2; x++) {
					if (TreeTable[OMap[ccz1 + z][ccx1 + x]])
					{
						Vector3d tree;
						tree.x = ccx1 + x;
						tree.z = ccz1 + z;
						return tree;
					}
					if (TreeTable[OMap[ccz2 + z][ccx2 + x]])
					{
						Vector3d tree;
						tree.x = ccx2 + x;
						tree.z = ccz2 + z;
						return tree;
					}
				}
			}
		}
	}
	Vector3d tree;
	tree.x = 0;
	tree.z = 0;
	return tree;
}


Vector3d CheckForATree(TCharacter *cptr) {

	int ccx = (int)cptr->pos.x / 256;
	int ccz = (int)cptr->pos.z / 256;
	/* for (int z = -2; z <= 2; z++) {
		for (int x = -2; x <= 2; x++) {
			if (TreeTable[OMap[ccz + z][ccx + x]])
			{
				Vector3d tree;
				tree.x = ccx + x;
				tree.z = ccz + z;
				return tree;
			}
		}
	}*/

	if (TreeTable[OMap[ccz][ccx]])
	{
		Vector3d tree;
		tree.x = ccx;
		tree.z = ccz;
		return tree;
	}

	Vector3d tree;
	tree.x = 0;
	tree.z = 0;
	return tree;
}


void AnimateMicro(TCharacter *cptr)
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
		if (cptr->gliding) cptr->Phase = DinoInfo[cptr->CType].glideAnim;
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
	if (cptr->State)
	{

		cptr->currentIdleGroup = -1;

		float aDist;
		aDist = ctViewR * DinoInfo[cptr->CType].aggress + OptAgres / AIInfo[cptr->Clone].agressMulti;
		if (cptr->gliding) aDist *= 2;

		bool fleeMode = FALSE;
		if (!SurvivalMode) {
			if (pdist > aDist ||
				DinoInfo[cptr->CType].aggress <= 0 || !cptr->awareHunter) {
				fleeMode = TRUE;
			}
			else if (DinoInfo[cptr->CType].defensive && cptr->Health == DinoInfo[cptr->CType].Health0) fleeMode = TRUE;
			else if (DinoInfo[cptr->CType].fearShot && cptr->Health < DinoInfo[cptr->CType].Health0) fleeMode = TRUE;
			else if (DinoInfo[cptr->CType].fearHearShot && cptr->heardShot) fleeMode = TRUE;
			else if (cptr->packId >= 0) Packs[cptr->packId].attack = TRUE;
		}

		if (cptr->packId >= 0) {
			if (Packs[cptr->packId]._attack) fleeMode = FALSE;
		}


		Vector3d tree;
		cptr->gottaClimb = FALSE;
		if (pdist > 1000 && !cptr->gliding) {
			tree = LookForATree(cptr);
			if (tree.x) cptr->gottaClimb = TRUE;
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
		else {
			if (cptr->gottaClimb) {
				cptr->tgx = tree.x * 256.f;
				cptr->tgz = tree.z * 256.f;
			}
			else {
				cptr->tgx = PlayerX;
				cptr->tgz = PlayerZ;
			}
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
				if (leaderdist < cptr->packDensity * 128 * 0.6)
				{
					cptr->followLeader = false;
					SetNewTargetPlace(cptr, AIInfo[cptr->Clone].targetDistance);
					goto TBEGIN;
				}
			}
			else {
				if (leaderdist > cptr->packDensity * 128 * 1.3)
				{
					cptr->followLeader = true;
				}
			}

		}

		float tdst = 456;
		// todo randomly triple target distance

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

		if (cptr->State && (pdist > DinoInfo[cptr->CType].weaveRange || !cptr->gottaClimb) && !DinoInfo[cptr->CType].dontWeave)
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

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) goto ENDPSELECT;

	if (cptr->Phase == DinoInfo[cptr->CType].glideAnim) {
		if (cptr->pos.y <= GetLandUpH(cptr->pos.x, cptr->pos.z)) {
			cptr->Phase = DinoInfo[cptr->CType].runAnim;
			cptr->gliding = false;
		} else goto ENDPSELECT;
	} //temp

	if (cptr->State) {
		if (cptr->Phase != DinoInfo[cptr->CType].climbAnim) {
			Vector3d tree = CheckForATree(cptr);
			if (tree.x) {
				cptr->climbable.x = (tree.x * 256.f) +128.f;
				cptr->climbable.z = (tree.z * 256.f) +128.f;
				cptr->vspeed = 0;
				cptr->climbY = GetLandH(cptr->climbable.x, cptr->climbable.z) + MObjects[OMap[(int)tree.z][(int)tree.x]].info.YHi - 384;
				cptr->Phase = DinoInfo[cptr->CType].climbAnim;
				cptr->gliding = TRUE;
				goto ENDPSELECT;
			}
		} else {
			if (cptr->pos.y >= cptr->climbY) cptr->Phase = DinoInfo[cptr->CType].glideAnim;
			goto ENDPSELECT;
		}
	}

	if (NewPhase)
		if (!cptr->State)
		{

			if (DinoInfo[cptr->CType].idleGroupCount
				&& (MyHealth || !DinoInfo[cptr->CType].killType[cptr->killType].carryCorpse)
				&& !(cptr->StateF & csONWATER)) {

				if (cptr->currentIdleGroup >= 0) {
					if (rRand(127) + 1 > (1 - DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].end) * 128
						&& (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].endOnAny
							|| cptr->Phase == DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1])) {
						cptr->Phase = DinoInfo[cptr->CType].walkAnim;
						if (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].instantRepeat) {
							cptr->currentIdleGroup = -1; //this must be done inside the if statement
						}
						else {
							cptr->currentIdleGroup = -1; //this must be done inside the if statement
							goto ENDPSELECT;
						}
					}
					else {
						cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[rRand(DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1)];
						goto ENDPSELECT;
					}
				}

				for (int idleGroupNo = 0; idleGroupNo < DinoInfo[cptr->CType].idleGroupCount; idleGroupNo++) {
					if (rRand(127) + 1 > (1 - DinoInfo[cptr->CType].idleGroup[idleGroupNo].start) * 128) cptr->currentIdleGroup = idleGroupNo;
				}
				if (cptr->currentIdleGroup >= 0) {
					if (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].startOnAny)
						cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[rRand(DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1)];
					else
						cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[0];
					goto ENDPSELECT;
				}
				else cptr->Phase = DinoInfo[cptr->CType].walkAnim;

			}
			else {
				cptr->Phase = DinoInfo[cptr->CType].walkAnim;
			}

		}
		else cptr->Phase = DinoInfo[cptr->CType].runAnim;

	if (cptr->currentIdleGroup == -1) {
		if (!cptr->State) cptr->Phase = DinoInfo[cptr->CType].walkAnim;
		else if (fabs(cptr->tgalpha - cptr->alpha) < 1.0 ||
			fabs(cptr->tgalpha - cptr->alpha) > 2 * pi - 1.0)
			cptr->Phase = DinoInfo[cptr->CType].runAnim;
		else cptr->Phase = DinoInfo[cptr->CType].walkAnim;
	}

	if (DinoInfo[cptr->CType].canSwim) {
		if (cptr->StateF & csONWATER) cptr->Phase = DinoInfo[cptr->CType].swimAnim;
	}

	if (!cptr->gliding) {
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
		}
		else if (MORPHP) {
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
	if (cptr->currentIdleGroup >= 0) goto SKIPROT;

	if (cptr->Phase == DinoInfo[cptr->CType].climbAnim) {
		cptr->pos.x = cptr->climbable.x - (cptr->lookx * DinoInfo[cptr->CType].climbDist);
		cptr->pos.z = cptr->climbable.z - (cptr->lookz * DinoInfo[cptr->CType].climbDist);
		if (_Phase != DinoInfo[cptr->CType].climbAnim) cptr->pos.y = GetLandUpH(cptr->pos.x, cptr->pos.z);
		cptr->gamma = 0;
		cptr->beta = 0;
		goto SKIPROT;
	}


	if (drspd > 0.02)
		if (cptr->tgalpha > cptr->alpha) currspeed = 0.6f + drspd * 1.2f;
		else currspeed = -0.6f - drspd * 1.2f;
	else currspeed = 0;
	if (cptr->AfraidTime && !cptr->gliding) currspeed *= 2.5;
	//if (cptr->gliding) currspeed /= 2;

	if (dalpha > pi) currspeed *= -1;
	if (((cptr->StateF & csONWATER) || cptr->Phase == DinoInfo[cptr->CType].walkAnim) && !cptr->gliding) currspeed /= 1.4f;

	if (cptr->gliding) DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 460.f);
	else if (cptr->AfraidTime) DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 160.f);
	else DeltaFunc(cptr->rspeed, currspeed, (float)TimeDt / 180.f);

	if (cptr->gliding) {
		tgbend = drspd / 2.f;
		if (tgbend > pi / 2) tgbend = pi / 2;
	}
	else {
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

	if (!cptr->gliding) {
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
	if (cptr->Phase == DinoInfo[cptr->CType].runAnim) curspeed = DinoInfo[cptr->CType].runspd;
	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) curspeed = DinoInfo[cptr->CType].wlkspd;
	if (cptr->Phase == DinoInfo[cptr->CType].glideAnim) curspeed = DinoInfo[cptr->CType].gldspd;
	if (DinoInfo[cptr->CType].canSwim) {
		if (cptr->Phase == DinoInfo[cptr->CType].swimAnim) curspeed = DinoInfo[cptr->CType].swmspd;
	}

	if (cptr->Phase == DinoInfo[cptr->CType].killType[cptr->killType].anim && DinoInfo[cptr->CType].killTypeCount) curspeed = 0.0f;

	if (cptr->Phase == DinoInfo[cptr->CType].runAnim && cptr->Slide && !cptr->gliding)
	{
		curspeed /= 8;
		if (drspd > pi / 2.f) curspeed = 0;
		else if (drspd > pi / 4.f) curspeed *= 2.f - 4.f*drspd / pi;
	}
	else if (drspd > pi / 2.f) curspeed *= 2.f - 2.f*drspd / pi;


	if (cptr->Phase == DinoInfo[cptr->CType].climbAnim) cptr->pos.y += TimeDt / 4.f;
	if (cptr->Phase == DinoInfo[cptr->CType].glideAnim) cptr->pos.y -= TimeDt /	8.f;

	//========== process speed =============//

	if (cptr->gliding) {
		curspeed *= cptr->scale;
		DeltaFunc(cptr->vspeed, curspeed, TimeDt / 2024.f);

		cptr->pos.x += cptr->lookx * cptr->vspeed * TimeDt;
		cptr->pos.z += cptr->lookz * cptr->vspeed * TimeDt;

		cptr->tggamma = cptr->rspeed / 4.0f;
		if (cptr->tggamma > pi / 6.f) cptr->tggamma = pi / 6.f;
		if (cptr->tggamma < -pi / 6.f) cptr->tggamma = -pi / 6.f;
		DeltaFunc(cptr->gamma, cptr->tggamma, TimeDt / 2048.f);

	}
	else {

		DeltaFunc(cptr->vspeed, curspeed, TimeDt / 500.f);

		MoveCharacter(cptr, cptr->lookx * cptr->vspeed * TimeDt * cptr->scale,
			cptr->lookz * cptr->vspeed * TimeDt * cptr->scale, !DinoInfo[cptr->CType].canSwim, TRUE);

		if (!cptr->gliding) {
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

		if (cptr->pos.y < GetLandH(cptr->pos.x, cptr->pos.z)) cptr->pos.y = GetLandH(cptr->pos.x, cptr->pos.z);

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

		if (DinoInfo[cptr->CType].smellCount > 0) {
			cptr->Phase = DinoInfo[cptr->CType].smellAnim[rRand(DinoInfo[cptr->CType].smellCount - 1)];
		}

		//cptr->Phase == cptr->roarAnim;
		cptr->FTime = 0;
		goto ENDPSELECT;
	}

	if (NewPhase){

		if (cptr->State == 1) {
			cptr->Phase = DinoInfo[cptr->CType].runAnim;
		
		} else if (cptr->State == 0){
			if (DinoInfo[cptr->CType].lookCount) {

				if (rRand(AIInfo[cptr->Clone].idleStartD) > 110) {
					cptr->Phase = DinoInfo[cptr->CType].lookAnim[rRand(DinoInfo[cptr->CType].lookCount - 1)];
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

				if (DinoInfo[cptr->CType].smellCount > 0) {
					cptr->Phase = DinoInfo[cptr->CType].smellAnim[rRand(DinoInfo[cptr->CType].smellCount - 1)];
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

	for (int i = 0; i < DinoInfo[cptr->CType].smellCount; i++) {
		if (cptr->Phase == DinoInfo[cptr->CType].smellAnim[i]) goto SKIPROT;
	}

	for (int i = 0; i < DinoInfo[cptr->CType].lookCount; i++) {
		if (cptr->Phase == DinoInfo[cptr->CType].lookAnim[i]) goto SKIPROT;
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
	if (cptr->Phase == DinoInfo[cptr->CType].runAnim) curspeed = DinoInfo[cptr->CType].runspd;
	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) curspeed = DinoInfo[cptr->CType].wlkspd;

	if (DinoInfo[cptr->CType].canSwim) {
		if (cptr->Phase == DinoInfo[cptr->CType].swimAnim) curspeed = DinoInfo[cptr->CType].swmspd;
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

		cptr->currentIdleGroup = -1;

		cptr->tgx = PlayerX;
		cptr->tgz = PlayerZ;
		cptr->tgtime = 0;
		if (cptr->State > 1)
			if (AngleDifference(cptr->alpha, palpha) < 0.4f)
			{
				if (cptr->State == 2) {
					if (DinoInfo[cptr->CType].lookCount) {
						cptr->Phase = DinoInfo[cptr->CType].lookAnim[rRand(DinoInfo[cptr->CType].lookCount - 1)];
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
					if (DinoInfo[cptr->CType].smellCount) {
						cptr->Phase = DinoInfo[cptr->CType].smellAnim[rRand(DinoInfo[cptr->CType].smellCount - 1)];
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

	if (pdist > (ctViewR + 20) * 256)
		if (ReplaceCharacterForward(cptr)) goto TBEGIN;

	if (!cptr->State) {


		if (cptr->packId >= 0) {
			float leaderdx = Packs[cptr->packId].leader->pos.x - cptr->pos.x;
			float leaderdz = Packs[cptr->packId].leader->pos.z - cptr->pos.z;
			float leaderdist = (float)sqrt(leaderdx * leaderdx + leaderdz * leaderdz);

			if (cptr->followLeader) {
				if (leaderdist < cptr->packDensity * 128 * 0.6)
				{
					cptr->followLeader = false;
					SetNewTargetPlace(cptr, 8048.f);
					goto TBEGIN;
				}
			}
			else {
				if (leaderdist > cptr->packDensity * 128 * 1.3)
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

		if (cptr->State && pdist > DinoInfo[cptr->CType].weaveRange && !DinoInfo[cptr->CType].dontWeave)
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

	
	for (int i = 0; i < DinoInfo[cptr->CType].lookCount; i++) {
		if (cptr->Phase == DinoInfo[cptr->CType].lookAnim[i]) LookMode = TRUE;
	}

	for (int i = 0; i < DinoInfo[cptr->CType].smellCount; i++) {
		if (cptr->Phase == DinoInfo[cptr->CType].smellAnim[i]) LookMode = TRUE;
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
			/*
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
			*/

			if (DinoInfo[cptr->CType].idleGroupCount
				&& (MyHealth || !DinoInfo[cptr->CType].killType[cptr->killType].carryCorpse)
				&& !(cptr->StateF & csONWATER)) {

				if (cptr->currentIdleGroup >= 0) {
					if (rRand(127) + 1 > (1 - DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].end) * 128
						&& (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].endOnAny
							|| cptr->Phase == DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1])) {
						cptr->Phase = DinoInfo[cptr->CType].walkAnim;
						if (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].instantRepeat) {
							cptr->currentIdleGroup = -1; //this must be done inside the if statement
						}
						else {
							cptr->currentIdleGroup = -1; //this must be done inside the if statement
							goto ENDPSELECT;
						}
					}
					else {
						cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[rRand(DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1)];
						goto ENDPSELECT;
					}
				}

				for (int idleGroupNo = 0; idleGroupNo < DinoInfo[cptr->CType].idleGroupCount; idleGroupNo++) {
					if (rRand(127) + 1 > (1 - DinoInfo[cptr->CType].idleGroup[idleGroupNo].start) * 128) cptr->currentIdleGroup = idleGroupNo;
				}
				if (cptr->currentIdleGroup >= 0) {
					if (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].startOnAny)
						cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[rRand(DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1)];
					else
						cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[0];
					goto ENDPSELECT;
				}
				else cptr->Phase = DinoInfo[cptr->CType].walkAnim;
			}
			else {
				cptr->Phase = DinoInfo[cptr->CType].walkAnim;
			}


	if (!NewPhase) {
		if (LookMode) goto ENDPSELECT;
		if (cptr->currentIdleGroup >= 0) goto ENDPSELECT;
	}

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
	if (cptr->currentIdleGroup >= 0) goto SKIPROT;

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
	if (cptr->Phase == DinoInfo[cptr->CType].runAnim) curspeed = DinoInfo[cptr->CType].runspd;
	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) curspeed = DinoInfo[cptr->CType].wlkspd;
	if (DinoInfo[cptr->CType].canSwim) {
		if (cptr->Phase == DinoInfo[cptr->CType].swimAnim) curspeed = DinoInfo[cptr->CType].swmspd;
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
		cptr->currentIdleGroup = -1;

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

	if (pdist[0] > (ctViewR + 20) * 256 && cptr->CType)
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
				if (leaderdist < cptr->packDensity * 128 * 0.6)
				{
					cptr->followLeader = false;
					SetNewTargetPlace(cptr, 2048.f);
					goto TBEGIN;
				}
			}
			else {
				if (leaderdist > cptr->packDensity * 128 * 1.3)
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
	else
	{
		cptr->tgalpha = CorrectedAlpha(FindVectorAlpha(targetdx, targetdz), cptr->alpha);//FindVectorAlpha(targetdx, targetdz);

		if (cptr->State && pdist[0] > DinoInfo[cptr->CType].weaveRange && !DinoInfo[cptr->CType].dontWeave)
		{
			cptr->tgalpha += (float)sin(RealTime / 824.f) / 2.f;
			if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
			if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;
		}
	}
	
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

			//if (DinoInfo[cptr->CType].idleCount) {
			if (DinoInfo[cptr->CType].idleGroupCount
			//	&& (MyHealth || !DinoInfo[cptr->CType].killType[cptr->killType].carryCorpse)
				&& !(cptr->StateF & csONWATER)) {

				if (cptr->currentIdleGroup >= 0) {
					if (rRand(127) + 1 > (1 - DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].end) * 128
						&& (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].endOnAny
							|| cptr->Phase == DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1])) {
						cptr->Phase = DinoInfo[cptr->CType].walkAnim;
						if (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].instantRepeat) {
							cptr->currentIdleGroup = -1; //this must be done inside the if statement
						}
						else {
							cptr->currentIdleGroup = -1; //this must be done inside the if statement
							goto ENDPSELECT;
						}
					}
					else {
						cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[rRand(DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1)];
						goto ENDPSELECT;
					}
				}

				for (int idleGroupNo = 0; idleGroupNo < DinoInfo[cptr->CType].idleGroupCount; idleGroupNo++) {
					if (rRand(127) + 1 > (1 - DinoInfo[cptr->CType].idleGroup[idleGroupNo].start) * 128) cptr->currentIdleGroup = idleGroupNo;
				}
				if (cptr->currentIdleGroup >= 0) {
					if (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].startOnAny)
						cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[rRand(DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1)];
					else
						cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[0];
					goto ENDPSELECT;
				}
				else cptr->Phase = DinoInfo[cptr->CType].walkAnim;

				/*
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
				*/
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

	if (cptr->currentIdleGroup >= 0) goto SKIPROT;

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
	if (cptr->Phase == DinoInfo[cptr->CType].runAnim) curspeed = DinoInfo[cptr->CType].runspd;
	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) curspeed = DinoInfo[cptr->CType].wlkspd;

	if (DinoInfo[cptr->CType].canSwim) {
		if (cptr->Phase == DinoInfo[cptr->CType].swimAnim) curspeed = DinoInfo[cptr->CType].swmspd;
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

	if (pdist > (ctViewR + 20) * 256) {
		if (ReplaceCharacterForward(cptr)) {
			goto TBEGIN;
		}
	}

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
		if (SurvivalMode) attackmode = TRUE;
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
					if (leaderdist < cptr->packDensity * 128 * 0.6)
					{
						cptr->followLeader = false;
						SetNewTargetPlaceFish(cptr, tv);
						goto TBEGIN;
					}
				}
				else {
					if (leaderdist > cptr->packDensity * 128 * 1.3)
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

		if (DinoInfo[cptr->CType].DangerFish || SurvivalMode) {
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
					float md = ((DinoInfo[cptr->CType].jumpRange * DinoInfo[cptr->CType].jmpspd) - (pUp * 1.3)) * cptr->scale;
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
		
		if (cptr->State && pdist > DinoInfo[cptr->CType].weaveRange && !DinoInfo[cptr->CType].dontWeave)
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
	else if (DinoInfo[cptr->CType].lookCount > 0) {
		for (int i = 0; i < DinoInfo[cptr->CType].lookCount; i++) {
			if (NewPhase && _Phase == DinoInfo[cptr->CType].lookAnim[i]) {
				cptr->aquaticIdle = false;
			}
		}
	}

	if (NewPhase) {
		if (!cptr->State) {
			cptr->Phase = DinoInfo[cptr->CType].walkAnim;
			if (DinoInfo[cptr->CType].lookCount){
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

					cptr->Phase = DinoInfo[cptr->CType].lookAnim[rRand(DinoInfo[cptr->CType].lookCount - 1)];
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
		
	for (int i = 0; i < DinoInfo[cptr->CType].lookCount; i++) {
		if (cptr->Phase == DinoInfo[cptr->CType].lookAnim[i]) goto SKIPROT;
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
	if (cptr->Phase == DinoInfo[cptr->CType].runAnim) curspeed = DinoInfo[cptr->CType].runspd;
	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) curspeed = DinoInfo[cptr->CType].wlkspd;
	if (AIInfo[cptr->Clone].jumper) {
		if (cptr->Phase == DinoInfo[cptr->CType].jumpAnim) curspeed = DinoInfo[cptr->CType].jmpspd;
	}

	if (DinoInfo[cptr->CType].lookCount > 0) {
		for (int i = 0; i < DinoInfo[cptr->CType].lookCount; i++) {
			if (cptr->Phase == DinoInfo[cptr->CType].lookAnim[i]) {
				curspeed = DinoInfo[cptr->CType].wlkspd;
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
		if (cptr->Phase == DinoInfo[cptr->CType].jumpAnim) cptr->vspeed = DinoInfo[cptr->CType].jmpspd;
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

		cptr->currentIdleGroup = -1;
		cptr->currentIdle2Group = -1;

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


	if (pdist > (ctViewR + 20) * 256)
		if (ReplaceCharacterForward(cptr)) goto TBEGIN;

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
			if (leaderdist < cptr->packDensity * 128 * 0.6)
			{
				cptr->followLeader = false;
				SetNewTargetPlace_Icth(cptr, 4048.f);
				goto TBEGIN;
			}
		}
		else {
			if (leaderdist > cptr->packDensity * 128 * 1.3)
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
					

					if (DinoInfo[cptr->CType].idle2GroupCount) {

						if (cptr->currentIdle2Group >= 0) {
							if (rRand(127) + 1 > (1 - DinoInfo[cptr->CType].idle2Group[cptr->currentIdle2Group].end) * 128
								&& (DinoInfo[cptr->CType].idle2Group[cptr->currentIdle2Group].endOnAny
									|| cptr->Phase == DinoInfo[cptr->CType].idle2Group[cptr->currentIdle2Group].anim[DinoInfo[cptr->CType].idle2Group[cptr->currentIdle2Group].count - 1])) {
								cptr->Phase = DinoInfo[cptr->CType].swimAnim;
								if (DinoInfo[cptr->CType].idle2Group[cptr->currentIdle2Group].instantRepeat) {
									cptr->currentIdle2Group = -1; //this must be done inside the if statement
								}
								else {
									cptr->currentIdle2Group = -1; //this must be done inside the if statement
									goto ENDPSELECT;
								}
							}
							else {
								cptr->Phase = DinoInfo[cptr->CType].idle2Group[cptr->currentIdle2Group].anim[rRand(DinoInfo[cptr->CType].idle2Group[cptr->currentIdle2Group].count - 1)];
								goto ENDPSELECT;
							}
						}

						for (int idle2GroupNo = 0; idle2GroupNo < DinoInfo[cptr->CType].idle2GroupCount; idle2GroupNo++) {
							if (rRand(127) + 1 > (1 - DinoInfo[cptr->CType].idle2Group[idle2GroupNo].start) * 128) cptr->currentIdle2Group = idle2GroupNo;
						}
						if (cptr->currentIdle2Group >= 0) {
							if (DinoInfo[cptr->CType].idle2Group[cptr->currentIdle2Group].startOnAny)
								cptr->Phase = DinoInfo[cptr->CType].idle2Group[cptr->currentIdle2Group].anim[rRand(DinoInfo[cptr->CType].idle2Group[cptr->currentIdle2Group].count - 1)];
							else
								cptr->Phase = DinoInfo[cptr->CType].idle2Group[cptr->currentIdle2Group].anim[0];
							goto ENDPSELECT;
						}
						else cptr->Phase = DinoInfo[cptr->CType].swimAnim;

					}
					else cptr->Phase = DinoInfo[cptr->CType].swimAnim;


				}
				else
				{
					

					if (DinoInfo[cptr->CType].idleGroupCount) {

						if (cptr->currentIdleGroup >= 0) {
							if (rRand(127) + 1 > (1 - DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].end) * 128
								&& (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].endOnAny
									|| cptr->Phase == DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1])) {
								cptr->Phase = DinoInfo[cptr->CType].walkAnim;
								if (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].instantRepeat) {
									cptr->currentIdleGroup = -1; //this must be done inside the if statement
								}
								else {
									cptr->currentIdleGroup = -1; //this must be done inside the if statement
									goto ENDPSELECT;
								}
							}
							else {
								cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[rRand(DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1)];
								goto ENDPSELECT;
							}
						}

						for (int idleGroupNo = 0; idleGroupNo < DinoInfo[cptr->CType].idleGroupCount; idleGroupNo++) {
							if (rRand(127) + 1 > (1 - DinoInfo[cptr->CType].idleGroup[idleGroupNo].start) * 128) cptr->currentIdleGroup = idleGroupNo;
						}
						if (cptr->currentIdleGroup >= 0) {
							if (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].startOnAny)
								cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[rRand(DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1)];
							else
								cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[0];
							goto ENDPSELECT;
						}
						else cptr->Phase = DinoInfo[cptr->CType].walkAnim;


					}
					else {
						cptr->Phase = DinoInfo[cptr->CType].walkAnim;
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
					if (wy >= swimLevel) cptr->Phase = DinoInfo[cptr->CType].swimAnim;
					else cptr->Phase = DinoInfo[cptr->CType].walkAnim;

				}
			}

			if (cptr->currentIdleGroup >= 0 || cptr->currentIdle2Group >= 0) {
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

		if (cptr->currentIdleGroup >= 0) {
					NewPhase = true;
					cptr->currentIdleGroup = -1;
					goto TBEGIN;
		}
	}


	if (!(wy >= swimLevel))
	{
		if (cptr->Phase == DinoInfo[cptr->CType].swimAnim) {
			NewPhase = true;
			goto TBEGIN;
		}

		if (cptr->currentIdle2Group >= 0) {
					NewPhase = true;
					cptr->currentIdle2Group = -1;
					goto TBEGIN;
		}
	}
	


	//LAST


	if (NewPhase)
	{

		if (cptr->Phase == DinoInfo[cptr->CType].walkAnim || cptr->currentIdleGroup >= 0)
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

	if (cptr->currentIdleGroup >= 0) {
				goto SKIPROT;
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


	if (cptr->currentIdle2Group >= 0) {
				currspeed /= 1.4f;
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
	if (cptr->Phase == DinoInfo[cptr->CType].flyAnim) curspeed = DinoInfo[cptr->CType].flyspd;
	if (cptr->Phase == DinoInfo[cptr->CType].glideAnim) curspeed = DinoInfo[cptr->CType].gldspd;
	if (cptr->Phase == DinoInfo[cptr->CType].takeoffAnim) curspeed = DinoInfo[cptr->CType].tkfspd;
	if (cptr->Phase == DinoInfo[cptr->CType].landAnim) curspeed = DinoInfo[cptr->CType].lndspd;
	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) curspeed = DinoInfo[cptr->CType].wlkspd;
	if (cptr->Phase == DinoInfo[cptr->CType].swimAnim) curspeed = DinoInfo[cptr->CType].swmspd;
	if (cptr->Phase == DinoInfo[cptr->CType].shakeWaterAnim) curspeed = DinoInfo[cptr->CType].swmspd;

	if (cptr->currentIdle2Group >= 0) {
				curspeed = DinoInfo[cptr->CType].swmspd;
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
	if (cptr->currentIdle2Group >= 0) {
				swimmingAnim = TRUE;
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

		cptr->canSleep = (Tranq && !OnWaterQ && cptr->Clone == AI_ICTH &&
			(cptr->deathPhase == DinoInfo[cptr->CType].walkAnim || cptr->deathPhase == DinoInfo[cptr->CType].shakeLandAnim || cptr->currentIdleGroup >= 0));

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

	bool autoCorrect = false;

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

		cptr->currentIdleGroup = -1;

		bool fleeMode = FALSE;
		if (!SurvivalMode) {
			if (pdist > attackDist || !playerAttackable || DinoInfo[cptr->CType].aggress <= 0 || !cptr->awareHunter) {
				fleeMode = TRUE;
			}
			else if (DinoInfo[cptr->CType].defensive && cptr->Health == DinoInfo[cptr->CType].Health0) fleeMode = TRUE;
			else if (DinoInfo[cptr->CType].fearShot && cptr->Health < DinoInfo[cptr->CType].Health0) fleeMode = TRUE;
			else if (DinoInfo[cptr->CType].fearHearShot && cptr->heardShot) fleeMode = TRUE;
			else if (cptr->packId >= 0) Packs[cptr->packId].attack = TRUE;
		}

		if (cptr->packId >= 0) {
			if (Packs[cptr->packId]._attack) fleeMode = FALSE;
		}

		if (!autoCorrect) {
			if (GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z) > 550) {
				autoCorrect = TRUE;
				SetNewTargetPlace_Brahi(cptr, 2048.f);
				goto TBEGIN;
			}
			else if (!fleeMode)
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

	if (pdist > (ctViewR + 20) * 256)
		if (ReplaceCharacterForward(cptr)) goto TBEGIN;

	if (!cptr->State)
	{
		attacking = false;
		cptr->AfraidTime = 0;


		if (cptr->packId >= 0) {
			float leaderdx = Packs[cptr->packId].leader->pos.x - cptr->pos.x;
			float leaderdz = Packs[cptr->packId].leader->pos.z - cptr->pos.z;
			float leaderdist = (float)sqrt(leaderdx * leaderdx + leaderdz * leaderdz);

			if (cptr->followLeader) {
				if (leaderdist < cptr->packDensity * 128 * 0.6)
				{
					cptr->followLeader = false;
					SetNewTargetPlace_Brahi(cptr, 2048.f);
					goto TBEGIN;
				}
			}
			else {
				if (leaderdist > cptr->packDensity * 128 * 1.3)
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
	
	if ((cptr->Clone == AI_LANDBRACH || cptr->State) && !autoCorrect) {
		if (pdist < 2048) cptr->NoFindCnt = 0;
		if (cptr->NoFindCnt) cptr->NoFindCnt--;
		else
		{
			cptr->tgalpha = CorrectedAlpha(FindVectorAlpha(targetdx, targetdz), cptr->alpha);//FindVectorAlpha(targetdx, targetdz);

			if (cptr->State && pdist > DinoInfo[cptr->CType].weaveRange && !DinoInfo[cptr->CType].dontWeave)
			{
				cptr->tgalpha += (float)sin(RealTime / 824.f) / 4.f;
				if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
				if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;
			}


		}

		if (cptr->Clone == AI_LANDBRACH) {
			LookForAWay(cptr, !DinoInfo[cptr->CType].canSwim, TRUE);
		}
		else {
			LookForAWay(cptr, TRUE, TRUE);
		}


		if (cptr->NoWayCnt > 12)
		{
			cptr->NoWayCnt = 0;
			cptr->NoFindCnt = 16 + rRand(20);
		}
	} else {
		cptr->tgalpha = FindVectorAlpha(targetdx, targetdz);

		if (cptr->State && pdist > DinoInfo[cptr->CType].weaveRange && !DinoInfo[cptr->CType].dontWeave)
		{
			cptr->tgalpha += (float)sin(RealTime / 824.f) / 4.f;
			if (cptr->tgalpha < 0) cptr->tgalpha += 2 * pi;
			if (cptr->tgalpha > 2 * pi) cptr->tgalpha -= 2 * pi;
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

	if (NewPhase)
	{
		if (!cptr->State)
		{

			if (DinoInfo[cptr->CType].idleGroupCount
				&& (MyHealth || !DinoInfo[cptr->CType].killType[cptr->killType].carryCorpse)) {


				if (cptr->currentIdleGroup >= 0) {
					if (rRand(127) + 1 > (1 - DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].end) * 128
						&& (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].endOnAny
							|| cptr->Phase == DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1])) {
						cptr->Phase = DinoInfo[cptr->CType].walkAnim;
						if (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].instantRepeat) {
							cptr->currentIdleGroup = -1; //this must be done inside the if statement
						}
						else {
							cptr->currentIdleGroup = -1; //this must be done inside the if statement
							goto ENDPSELECT;
						}
					}
					else {
						cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[rRand(DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1)];
						goto ENDPSELECT;
					}
				}

				for (int idleGroupNo = 0; idleGroupNo < DinoInfo[cptr->CType].idleGroupCount; idleGroupNo++) {
					if (rRand(127) + 1 > (1 - DinoInfo[cptr->CType].idleGroup[idleGroupNo].start) * 128) cptr->currentIdleGroup = idleGroupNo;
				}
				if (cptr->currentIdleGroup >= 0) {
					if (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].startOnAny)
						cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[rRand(DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1)];
					else
						cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[0];
					goto ENDPSELECT;
				}
				else cptr->Phase = DinoInfo[cptr->CType].walkAnim;



				/*
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
				*/


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

	if (cptr->currentIdleGroup >= 0) goto SKIPROT;

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

	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) curspeed = DinoInfo[cptr->CType].wlkspd;
	if (cptr->Phase == DinoInfo[cptr->CType].runAnim) curspeed = DinoInfo[cptr->CType].runspd;

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

	float playerdx = PlayerX - cptr->pos.x - cptr->lookx * 108;
	float playerdz = PlayerZ - cptr->pos.z - cptr->lookz * 108;
	float pdist = (float)sqrt(playerdx * playerdx + playerdz * playerdz);
	if (pdist > (ctViewR + 20) * 256)
		if (ReplaceCharacterForward(cptr)) goto TBEGIN;

	if (cptr->packId >= 0) {
		float leaderdx = Packs[cptr->packId].leader->pos.x - cptr->pos.x;
		float leaderdz = Packs[cptr->packId].leader->pos.z - cptr->pos.z;
		float leaderdist = (float)sqrt(leaderdx * leaderdx + leaderdz * leaderdz);

		if (cptr->followLeader) {
			if (leaderdist < cptr->packDensity * 128 * 0.6)
			{
				cptr->followLeader = false;
				SetNewTargetPlace_Brahi(cptr, 2048.f);
				goto TBEGIN;
			}
		}
		else {
			if (leaderdist > cptr->packDensity * 128 * 1.3)
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
		
		if (DinoInfo[cptr->CType].idleGroupCount
			&& (MyHealth || !DinoInfo[cptr->CType].killType[cptr->killType].carryCorpse)) {


			if (cptr->currentIdleGroup >= 0) {
				if (rRand(127) + 1 > (1 - DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].end) * 128
					&& (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].endOnAny
						|| cptr->Phase == DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1])) {
					cptr->Phase = DinoInfo[cptr->CType].walkAnim;
					if (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].instantRepeat) {
						cptr->currentIdleGroup = -1; //this must be done inside the if statement
					}
					else {
						cptr->currentIdleGroup = -1; //this must be done inside the if statement
						goto ENDPSELECT;
					}
				}
				else {
					cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[rRand(DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1)];
					goto ENDPSELECT;
				}
			}

			for (int idleGroupNo = 0; idleGroupNo < DinoInfo[cptr->CType].idleGroupCount; idleGroupNo++) {
				if (rRand(127) + 1 > (1 - DinoInfo[cptr->CType].idleGroup[idleGroupNo].start) * 128) cptr->currentIdleGroup = idleGroupNo;
			}
			if (cptr->currentIdleGroup >= 0) {
				if (DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].startOnAny)
					cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[rRand(DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].count - 1)];
				else
					cptr->Phase = DinoInfo[cptr->CType].idleGroup[cptr->currentIdleGroup].anim[0];
				goto ENDPSELECT;
			}
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

	if (cptr->currentIdleGroup >= 0) goto SKIPROT;

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
	if (cptr->Phase == DinoInfo[cptr->CType].walkAnim) curspeed = DinoInfo[cptr->CType].wlkspd;

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

	if (pdist > (ctViewR + 20) * 256)
		if (ReplaceCharacterForward(cptr)) goto TBEGIN;


	//======== exploring area ===============//




	if (cptr->packId >= 0) {
		float leaderdx = Packs[cptr->packId].leader->pos.x - cptr->pos.x;
		float leaderdz = Packs[cptr->packId].leader->pos.z - cptr->pos.z;
		float leaderdist = (float)sqrt(leaderdx * leaderdx + leaderdz * leaderdz);

		if (cptr->followLeader) {
			if (leaderdist < cptr->packDensity * 128 * 0.6)
			{
				cptr->followLeader = false;
				SetNewTargetPlace(cptr, 4048.f);
				goto TBEGIN;
			}
		}
		else {
			if (leaderdist > cptr->packDensity * 128 * 1.3)
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
			if (cptr->pos.y > GetLandH(cptr->pos.x, cptr->pos.z) + DinoInfo[cptr->CType].maxDepth)
				cptr->Phase = DinoInfo[cptr->CType].glideAnim;
			else;
		else if (cptr->Phase == DinoInfo[cptr->CType].glideAnim)
			if (cptr->pos.y < GetLandH(cptr->pos.x, cptr->pos.z) + DinoInfo[cptr->CType].minDepth)
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
	if (cptr->Phase == DinoInfo[cptr->CType].flyAnim) curspeed = DinoInfo[cptr->CType].flyspd;
	if (cptr->Phase == DinoInfo[cptr->CType].glideAnim) curspeed = DinoInfo[cptr->CType].gldspd;

	if (drspd > pi / 2.f) curspeed *= 2.f - 2.f*drspd / pi;

	if (cptr->Phase == DinoInfo[cptr->CType].flyAnim)
		DeltaFunc(cptr->pos.y, GetLandH(cptr->pos.x, cptr->pos.z) + (DinoInfo[cptr->CType].maxDepth * 1.5), TimeDt / 6.f);
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

	if (SurvivalMode) {
		bool waveOver = true;
		for (CurDino = 0; CurDino < ChCount; CurDino++) {
			if (Characters[CurDino].Health) waveOver = false;
		}
		if (waveOver) {
			WaveNoteTime = 2000;
			PlaceCharactersSurvival();
		}
	}


	//packs
	for (int packN = 0; packN < PackCount; packN++) {

		Packs[packN]._alert = Packs[packN].alert;
		Packs[packN]._attack = Packs[packN].attack;
		Packs[packN].alert = FALSE;
		Packs[packN].attack = FALSE;

	}

	TrophyDisplay = false;

	for (CurDino = 0; CurDino < ChCount; CurDino++)
	{
		cptr = &Characters[CurDino];
		if (cptr->StateF == 0xFF) continue;
		cptr->tgtime += TimeDt;

		// tracker bullets
		if (cptr->RTime && WeapInfo[cptr->tracker].radarTime) {
			cptr->RTime -= TimeDt;
			if (cptr->RTime < 0) {
				cptr->RTime = 0;
				cptr->tracker = -1;
			}
		}


		// replace pack leader
		if (cptr->Health && cptr->packId >= 0) {
			if (!Packs[cptr->packId].leader->Health) Packs[cptr->packId].leader = cptr;
		}

		if (cptr->tgtime > 30 * 1000) {

			if (cptr->Clone == AI_BRACH || cptr->Clone == AI_BRACHDANGER || cptr->Clone == AI_LANDBRACH) SetNewTargetPlace_Brahi(cptr, 2048.f);
			else if (cptr->Clone == AI_MOSA) SetNewTargetPlaceFish(cptr, 5048.f);
			else if (cptr->Clone == AI_FISH) SetNewTargetPlaceFish(cptr, 1024.f);
			else SetNewTargetPlace(cptr, 2048);

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

		if (cptr->AfraidTime <= 0) {
			cptr->awareHunter = FALSE;
			cptr->heardShot = FALSE;
		}

		BOOL TROPHYON = (GetLandUpH(cptr->pos.x, cptr->pos.z) - GetLandH(cptr->pos.x, cptr->pos.z) < 100);

		//disp ship info
		if (!cptr->Health && TROPHYON && DinoInfo[cptr->CType].trophy) {
			if (fabs(VectorLength(SubVectors(PlayerPos, cptr->pos))) < 300.f && !cptr->claimed) {
				TrophyDisplayBody.ctype = cptr->CType;
				TrophyDisplayBody.scale = cptr->scale;
				TrophyDisplayBody.weapon = CurrentWeapon;
				TrophyDisplayBody.score = cptr->tempScore;
				TrophyDisplayBody.phase = (RealTime & 3);
				TrophyDisplayBody.time = cptr->tempTime;
				TrophyDisplayBody.date = cptr->tempDate;
				TrophyDisplayBody.range = cptr->tempRange;
				TrophyDisplay = true;
				TrophyDisplayC = CurDino;
			}
		}

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
			else AnimateDeadCommon(cptr);
			break;
		case AI_TITAN:
			//TEMP DISABLED
			//if (cptr->Health) AnimateTitan(cptr);
			//else AnimateIcthDead(cptr);
			break;
		case AI_MICRO:
			//TEMP DISABLED
			//if (cptr->Health) AnimateMicro(cptr);
			//else AnimateIcthDead(cptr);
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
		float r = range * (DinoInfo[cptr->CType].HearK * 2);
		if (l > r) continue;


		if (cptr->Clone == AI_TREX) {  //===== T-Rex
			if (!cptr->State) {
				cptr->State = 2;
				cptr->awareHunter = TRUE;
				cptr->heardShot = TRUE;
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
			cptr->heardShot = TRUE;
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
		if (!AIInfo[cptr->Clone].sniffer) continue;
		//if (cptr->AfraidTime || cptr->State == 1) continue;

		if (cptr->Clone == AI_TREX && (cptr->AfraidTime || cptr->State == 1)) continue; //here to check if hunter detected once it starts running from fear call, trex doesn't like it tho

		if (SurvivalMode) goto isAfraid;

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

			isAfraid:

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

	for (int c = 0; c < TROPHY2_COUNT; c++)
	{

		//MessageBox(hwndMain, c.tostring(), "boring", IDOK);

		if (!TrophyRoom2.Body[c].ctype) continue;

			//do this in addshiptask too

		Characters[ChCount].CType = TrophyRoom2.Body[c].ctype;
//		Characters[ChCount].tropIndex = c;

//		if (DinoInfo[Characters[ChCount].CType].tCounter >= DinoInfo[TrophyIndex[TrophyRoom2.Body[c].ctype]].trophyTypeCount) continue;
		

		//Characters[ChCount].CType = TrophyRoom.Body[c].ctype;

		/*
		if (c < 6) Characters[ChCount].alpha = pi / 2;
		else if (c < 12) Characters[ChCount].alpha = pi;
		else if (c < 18) Characters[ChCount].alpha = pi * 3 / 2;
		else			*/



		ResetCharacter(&Characters[ChCount]);

		
		Characters[ChCount].State = c;
		Characters[ChCount].scale = TrophyRoom2.Body[c].scale;
		
		float scaleDif = Characters[ChCount].scale - (DinoInfo[Characters[ChCount].CType].Scale0 / 1000.f);
		if (scaleDif != 0) {
			if (DinoInfo[Characters[ChCount].CType].ScaleA != 0) scaleDif /= (DinoInfo[Characters[ChCount].CType].ScaleA / 1000.f);
			else scaleDif = 0;
		}
		int scaleDifx = (int)(scaleDif * trophyType[c].xoffsetScale);
		int scaleDifz = (int)(scaleDif * trophyType[c].zoffsetScale);
		int scaleDify = (int)(scaleDif * trophyType[c].yoffsetScale);

		Characters[ChCount].pos.x = LandingList.list[trophyType[c].trophyPos].x
			* 256 + 128 + trophyType[c].xoffset +scaleDifx;
		Characters[ChCount].pos.z = LandingList.list[trophyType[c].trophyPos].y
			* 256 + 128 + trophyType[c].zoffset +scaleDifz;

		Characters[ChCount].Phase = trophyType[c].anim;
		Characters[ChCount].PrevPhase = Characters[ChCount].Phase;


		Characters[ChCount].pos.y = GetLandH(Characters[ChCount].pos.x,
			Characters[ChCount].pos.z);

		Characters[ChCount].pos.y += trophyType[c].yoffset +scaleDify;
		
		float a = (float)trophyType[c].alpha;
		float b = (float)trophyType[c].beta;
		float g = (float)trophyType[c].gamma;
		Characters[ChCount].alpha = pi * 2 * a / 360.f;
		Characters[ChCount].beta = pi * 2 * b / 360.f;
		Characters[ChCount].gamma = pi * 2 * g / 360.f;

		Characters[ChCount].xdata = (int)(LandingList.list[trophyType[c].trophyPos].x
			* 256 + 128 + trophyType[c].xdata);
		Characters[ChCount].zdata = (int)(LandingList.list[trophyType[c].trophyPos].y
			* 256 + 128 + trophyType[c].zdata);
		Characters[ChCount].ydata = trophyType[c].ydata;

		Characters[ChCount].animateTrophy = trophyType[c].playAnim;

		//DinoInfo[Characters[ChCount].CType].tCounter++;
		ChCount++;
	}
}


















void spawnPositionPackLeader() {
	TSpawnRegion *sr = &spawnGroup[Characters[ChCount].SpawnGroupType].spawnRegion[rRand(spawnGroup[Characters[ChCount].SpawnGroupType].spawnRegionCh - 1)];

	

	Characters[ChCount].pos.x = sr->XMin * 256
		+ abs(rRand(sr->XMax - sr->XMin) * 256);

	Characters[ChCount].pos.z = sr->YMin * 256
		+ abs(rRand(sr->YMax - sr->YMin) * 256);
}


void spawnPositionPackFollower(int leader) {
	//spawn on same spot, so the pack doesn't spawn over a cliff edge or something
	Characters[ChCount].pos.x = Characters[leader].pos.x;
	Characters[ChCount].pos.z = Characters[leader].pos.z;
}

/*
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
*/

void spawnMapAmbient(int &tr, int leader, bool moveForward) {

replaceSMA:

	if (moveForward && tr < 1024) {
		Characters[ChCount].pos.x = PlayerX + siRand(10040);
		Characters[ChCount].pos.z = PlayerZ + siRand(10040);

		BOOL outside = true;
		for (int sr = 0; sr < spawnGroup[Characters[ChCount].SpawnGroupType].spawnRegionCh; sr++) {
			if (Characters[ChCount].pos.x > spawnGroup[Characters[ChCount].SpawnGroupType].spawnRegion[sr].XMin * 256 &&
				Characters[ChCount].pos.x < spawnGroup[Characters[ChCount].SpawnGroupType].spawnRegion[sr].XMax * 256 &&
				Characters[ChCount].pos.z > spawnGroup[Characters[ChCount].SpawnGroupType].spawnRegion[sr].YMin * 256 &&
				Characters[ChCount].pos.z < spawnGroup[Characters[ChCount].SpawnGroupType].spawnRegion[sr].YMax * 256) outside = false;
		}
		if (outside) {
			tr++;
			goto replaceSMA;
		}
		
	} else {
	
		if (leader >= 0) {
			spawnPositionPackFollower(leader);
		}
		else {
			spawnPositionPackLeader();
		}

	}

	Characters[ChCount].pos.y = GetLandH(Characters[ChCount].pos.x,
		Characters[ChCount].pos.z);
	float wy = GetLandUpH(Characters[ChCount].pos.x,
		Characters[ChCount].pos.z) - Characters[ChCount].pos.y;
	tr++;
	if (tr > 10240) return;


	int mindist = 40;
	if (SurvivalMode) {
		mindist = ctViewR + 1;
	}

	if (fabs(Characters[ChCount].pos.x - PlayerX) +
		fabs(Characters[ChCount].pos.z - PlayerZ) < 256 * mindist)
		goto replaceSMA;

	if (DinoInfo[Characters[ChCount].CType].Clone == AI_BRACH ||
		DinoInfo[Characters[ChCount].CType].Clone == AI_BRACHDANGER ||
		DinoInfo[Characters[ChCount].CType].Clone == AI_ICTH) {

		if (wy > 380) goto replaceSMA;
		if (wy < 220) goto replaceSMA;

	}

	if (DinoInfo[Characters[ChCount].CType].Clone == AI_FISH ||
		DinoInfo[Characters[ChCount].CType].Clone == AI_MOSA) {
		if (wy < DinoInfo[Characters[ChCount].CType].minDepth) goto replaceSMA;
		if (wy > DinoInfo[Characters[ChCount].CType].maxDepth) goto replaceSMA;
	}



	if (spawnGroup[Characters[ChCount].SpawnGroupType].avoidRegionCh) {
		for (int ar = 0; ar < spawnGroup[Characters[ChCount].SpawnGroupType].avoidRegionCh; ar++) {
			if (Characters[ChCount].pos.x > spawnGroup[Characters[ChCount].SpawnGroupType].avoidRegion[ar].XMin * 256 &&
				Characters[ChCount].pos.x < spawnGroup[Characters[ChCount].SpawnGroupType].avoidRegion[ar].XMax * 256 &&
				Characters[ChCount].pos.z > spawnGroup[Characters[ChCount].SpawnGroupType].avoidRegion[ar].YMin * 256 &&
				Characters[ChCount].pos.z < spawnGroup[Characters[ChCount].SpawnGroupType].avoidRegion[ar].YMax * 256) goto replaceSMA;
		}
	}

	if (!moveForward)
		if (fabs(Characters[ChCount].pos.x - PlayerX) +
			fabs(Characters[ChCount].pos.z - PlayerZ) < 256 * 40)
			goto replaceSMA;

	//note character not reset yet, cpcpAquatic not set.
	if (DinoInfo[Characters[ChCount].CType].Clone == AI_BRACH ||
		DinoInfo[Characters[ChCount].CType].Clone == AI_BRACHDANGER ||
		DinoInfo[Characters[ChCount].CType].Clone == AI_ICTH ||
		DinoInfo[Characters[ChCount].CType].Clone == AI_FISH ||
		DinoInfo[Characters[ChCount].CType].Clone == AI_MOSA) {
		if (CheckPlaceCollisionP(Characters[ChCount].pos, true)) goto replaceSMA;
	}
	else if (CheckPlaceCollisionP(Characters[ChCount].pos, false)) goto replaceSMA;


	
	

	Characters[ChCount].tgx = Characters[ChCount].pos.x;
	Characters[ChCount].tgz = Characters[ChCount].pos.z;
	Characters[ChCount].tgtime = 0;

	if (DinoInfo[Characters[ChCount].CType].Clone == AI_BRACH ||
		DinoInfo[Characters[ChCount].CType].Clone == AI_BRACHDANGER ||
		DinoInfo[Characters[ChCount].CType].Clone == AI_LANDBRACH ||
		DinoInfo[Characters[ChCount].CType].Clone == AI_ICTH) {

		Characters[ChCount].spawnAlt = GetLandUpH(Characters[ChCount].pos.x,
			Characters[ChCount].pos.z);

	}

	if (DinoInfo[Characters[ChCount].CType].Clone == AI_FISH ||
		DinoInfo[Characters[ChCount].CType].Clone == AI_MOSA) {

		Characters[ChCount].depth = GetLandH(Characters[ChCount].tgx, Characters[ChCount].tgz) +
			((GetLandUpH(Characters[ChCount].tgx, Characters[ChCount].tgz) - GetLandH(Characters[ChCount].tgx, Characters[ChCount].tgz) / 2)
				);
		Characters[ChCount].tdepth = Characters[ChCount].depth;

	}

	ResetCharacter(&Characters[ChCount]);


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




void dispSighting(int dii, int xx, int zz) {

	char buff[100];
	char loc[100];
	bool lob = false;

	int ccx = xx;
	int ccz = zz;

	int ccxd = rRand(8) ^ 3;
	int cczd = rRand(8) ^ 3;
	if ((rand() % 2)) ccxd *= -1;
	if ((rand() % 2)) cczd *= -1;

	bool foundLoc = false;
	if (rRand(4) != 1) {

		if (ccz > 400 &&
			ccx < 350) {
			sprintf(loc, "on the western approaches");
			foundLoc = true;
		}
		else if (ccz > 800 &&
			ccz < 850 &&
			ccx > 660 &&
			ccx < 820) {
			sprintf(loc, "in Fort Ciskin");
			foundLoc = true;
		}
		else if (ccz > 330 &&
			ccz < 430 &&
			ccx > 550 &&
			ccx < 650) {
			sprintf(loc, "in the desert");
			foundLoc = true;
		}
		else if (ccz > 180 &&
			ccz < 330 &&
			ccx > 450 &&
			ccx < 600) {
			sprintf(loc, "near the lake");
			foundLoc = true;
		}
		else if (ccz > 100 &&
			ccz < 160 &&
			ccx > 450 &&
			ccx < 570) {
			sprintf(loc, "in the gultch");
			foundLoc = true;
		}
		else if (ccz > 220 &&
			ccz < 300 &&
			ccx > 570 &&
			ccx < 820) {
			sprintf(loc, "in the Catacombs");
			foundLoc = true;
		}
		else if (ccz > 320 &&
			ccz < 340 &&
			ccx > 900) {
			sprintf(loc, "at the stone circle");
			foundLoc = true;
		}
		else if (ccz > 600 &&
			ccz < 680 &&
			ccx > 800) {
			sprintf(loc, "near Feltcher's Bog");
			foundLoc = true;
		}
		else if (ccz > 280 &&
			ccz < 350 &&
			ccx > 350 &&
			ccx < 550) {
			sprintf(loc, "along the twin cliffs");
			foundLoc = true;
		}
		else if (ccz > 200 &&
			ccz < 300 &&
			ccx > 250 &&
			ccx < 350) {
			sprintf(loc, "along the twin cliffs");
			foundLoc = true;
		}
		else if (ccz > 100 &&
			ccz < 250 &&
			ccx > 200 &&
			ccx < 450) {
			sprintf(loc, "along the estuary");
			foundLoc = true;
		}
		else if (ccz > 750 &&
			ccz < 830 &&
			ccx > 300 &&
			ccx < 650) {
			sprintf(loc, "along the river valley");
			foundLoc = true;
		}
		else if (ccz > 600 &&
			ccz < 700 &&
			ccx > 500 &&
			ccx < 700) {
			sprintf(loc, "near the Great Swamp");
			foundLoc = true;
		}
	}


	if (!foundLoc) {
		int locc;
		if (ccz > 683) {
			locc = 0;
		}
		else if (ccz < 341) {
			locc = 3;
		}
		else {
			locc = 6;
		}
		if (ccx > 683) {
			locc += 1;
		}
		else if (ccx < 341) {
			locc += 2;
		}
		switch (locc) {
		case 0:
			sprintf(loc, "in the South");
			break;
		case 1:
			sprintf(loc, "in the Southeast");
			break;
		case 2:
			sprintf(loc, "in the Southwest");
			break;
		case 3:
			sprintf(loc, "in the North");
			break;
		case 4:
			sprintf(loc, "in the Northeast");
			break;
		case 5:
			sprintf(loc, "in the Northwest");
			break;
		case 6:
			sprintf(loc, "in the Centre of the Island");
			break;
		case 7:
			sprintf(loc, "in the East");
			break;
		case 8:
			sprintf(loc, "in the West");
			break;
		}
	}

	sprintf(buff, "Sighting:\n%s", DinoInfo[dii].Name);
	sprintf(buff + strlen(buff), " %s", loc);
	MessageBox(hwndMain, buff, "TEST", IDOK);

}

void PlaceCharactersSurvival()
{
	if (SurvivalWave > TrophyRoom2.survivalHighScore) TrophyRoom2.survivalHighScore = SurvivalWave;
	SaveTrophy();
	SurvivalWave++;
	if (SurvivalWave >= 27) DoHalt("Limit Exceeded");
	int tr = 0;
	int waveTotal = 1;
	int dinoCost = 1;
	for (int w = 0; w < SurvivalWave; w++) waveTotal *= 2;
	for (int d = 0; d < SurvivalIndexCh - 1; d++) dinoCost *= 3;
	int dinoIndex = SurvivalIndexCh;
	
	//char buff[100];
	//sprintf(buff, "SurvivalIndexCh %i", SurvivalIndexCh);
	//MessageBox(hwndMain, buff, "TEST", IDOK);
	//sprintf(buff, "/nSurvivalWave = %i", SurvivalWave);
	//MessageBox(hwndMain, buff, "TEST", IDOK);
	//sprintf(buff, "START-waveTotal %i", waveTotal);
	//MessageBox(hwndMain, buff, "TEST", IDOK);
	//sprintf(buff, "START-dinoCost  %i", dinoCost);
	//MessageBox(hwndMain, buff, "TEST", IDOK);
	//sprintf(buff, "START-dinoIndex %i", dinoIndex);
	//MessageBox(hwndMain, buff, "TEST", IDOK);

	while (waveTotal < dinoCost) {
		dinoCost /= 3;
		dinoIndex --;
	}
	while (dinoIndex > 0) {
		while (waveTotal >= dinoCost) {
			if (tr > 10500) DoHalt("Cannot spawn characters");
			if (ChCount > 254) DoHalt("Character limit exceeded");
			Characters[ChCount].CType = SurvivalIndex[dinoIndex];
			Characters[ChCount].SpawnGroupType = 0;
			Characters[ChCount].packId = -1;
			spawnMapAmbient(tr, -1, false);
			Characters[ChCount].State = 2;
			ChCount++;
			waveTotal -= dinoCost;
		}
		dinoCost /= 3;
		dinoIndex--;
	}
}

void PlaceCharacters()
{

	int c, tr;
	for (int i = 0; i < ChCount; i++) {
		Characters[i] = {};
	}
	ChCount = 0;

	PrintLog("Placing...");

	for (c = 10; c < 30; c++)
		if ((TargetDino & (1 << c)) > 0)
		{
			TargetCall = c;
			break;
		}

	// NEW SYSTEM

	//TODO - NO PACK HUNTING WITH MOVEFORWARD
	//TODO - HALF AMBIENT SPAWN AT NIGHT MANUALLY IN RES?


	if (!RestartMode) {

		for (int di = 0; di < DINOINFO_MAX; di++) {
			if (DinoInfo[di].packMember2Ch) {
				for (int pin = 0; pin < DinoInfo[di].packMember2Ch; pin++) {
					packType[DinoInfo[di].packMember2[pin].packGroup]
						.packMember[packType[DinoInfo[di].packMember2[pin].packGroup].packMemberCh]
						.ctype = di;
					packType[DinoInfo[di].packMember2[pin].packGroup]
						.packMember[packType[DinoInfo[di].packMember2[pin].packGroup].packMemberCh]
						.ratio = DinoInfo[di].packMember2[pin].ratio;
					packType[DinoInfo[di].packMember2[pin].packGroup].packMemberCh++;
				}
			}
		}

		for (int di = 0; di < DINOINFO_MAX; di++) {
			if (DinoInfo[di].SpawnInfoCh) {
				
				packType[packTypeCount].packMember[packType[packTypeCount].packMemberCh].ctype = di;
				packType[packTypeCount].packMember[packType[packTypeCount].packMemberCh].ratio = 1;
				packType[packTypeCount].packMax = 1;
				packType[packTypeCount].packMin = 1;
				packType[packTypeCount].packMemberCh++;
				for (int si = 0; si < DinoInfo[di].SpawnInfoCh; si++) {
					packType[packTypeCount].SpawnInfo[packType[packTypeCount].SpawnInfoCh].spawnGroup = DinoInfo[di].SpawnInfo[si].spawnGroup;
					packType[packTypeCount].SpawnInfo[packType[packTypeCount].SpawnInfoCh].spawnRatio = DinoInfo[di].SpawnInfo[si].spawnRatio;
					packType[packTypeCount].SpawnInfoCh++;
				}
				packTypeCount++;
			}
		}

		for (int p = 0; p < packTypeCount; p++) {
			if (packType[p].SpawnInfoCh){
				for (int si = 0; si < packType[p].SpawnInfoCh; si++) {
					spawnGroup[packType[p].SpawnInfo[si].spawnGroup].packIndex[spawnGroup[packType[p].SpawnInfo[si].spawnGroup].packIndexCh] = p;
					spawnGroup[packType[p].SpawnInfo[si].spawnGroup].spawnInfoIndex[spawnGroup[packType[p].SpawnInfo[si].spawnGroup].packIndexCh] = si;
					spawnGroup[packType[p].SpawnInfo[si].spawnGroup].packIndexCh++;
				}
			}
		}

	}

	for (int sg = 0; sg < TotalSpawnGroup; sg++){
	//for (TSpawnGroup sg : spawnGroup) {
		
		

		if (spawnGroup[sg].packIndexCh) {
			int spawnNo = spawnGroup[sg].SpawnMin;
			for (int i = 0; i < spawnGroup[sg].SpawnMax - spawnGroup[sg].SpawnMin; i++) {
				if (spawnGroup[sg].SpawnRate * 30000 > rRand(30000)) spawnNo++;
			}
			if (spawnGroup[sg].densityMulti != 0) {
				float m = OptDens - 128;
				m /= 128.f;
				m *= spawnGroup[sg].densityMulti;
				spawnNo += (int)m;
				if (spawnNo < 0) spawnNo = 0;
			}

			float ratioScores[256];
			float totalRatio = 0;
			//int counter[256];
			for (c = 0; c < spawnGroup[sg].packIndexCh; c++) {
				ratioScores[c] = 0.f;
				//counter[c] = 0;
				totalRatio += packType[spawnGroup[sg].packIndex[c]].SpawnInfo[spawnGroup[sg].spawnInfoIndex[c]].spawnRatio;
				            //DinoInfo[spawnGroup[sg].dinoIndex[c]].SpawnInfo[spawnGroup[sg].spawnInfoIndex[c]].spawnRatio;
			}
			int posi = 0;
			tr = 0;

			for (c = 0; c < spawnNo; c++) {

				int packInd = -1;

				// select ctype accounting for spawn ratio
				if (spawnGroup[sg].Randomised) {
					float selector = rRand(30000);
					selector /= 30000;
					selector *= totalRatio;
					for (int ch = 0; ch < spawnGroup[sg].packIndexCh; ch++) {
						if (selector <= packType[spawnGroup[sg].packIndex[ch]].SpawnInfo[spawnGroup[sg].spawnInfoIndex[ch]].spawnRatio) {
							packInd = spawnGroup[sg].packIndex[ch];
							break;
						} else selector -= packType[spawnGroup[sg].packIndex[ch]].SpawnInfo[spawnGroup[sg].spawnInfoIndex[ch]].spawnRatio;
					}
				} else {
					while (packInd == -1) {
						int post = posi % spawnGroup[sg].packIndexCh;
						if (ratioScores[post] >= 1.f) {
							ratioScores[post] -= 1.f;
							packInd = spawnGroup[sg].packIndex[post];
						}
						else {
							ratioScores[post] += packType[spawnGroup[sg].packIndex[post]].SpawnInfo[spawnGroup[sg].spawnInfoIndex[post]].spawnRatio;
							posi++;
						}
					}
				}

				/*
				std::list<int> spawnList;
				for (int indx = 0; indx < spawnGroup[sg].dinoIndexCh; indx++) {
					for (int rat = 0; rat < DinoInfo[spawnGroup[sg].dinoIndex[indx]].SpawnInfo[spawnGroup[sg].spawnInfoIndex[indx]].spawnRatio; rat++) {
						spawnList.push_back(spawnGroup[sg].dinoIndex[indx]);
					}
				}

				auto it = spawnList.begin();
				if (spawnGroup[sg].Randomised) {	
					std::advance(it, rRand(spawnList.size() - 1));	
				} else {
					std::advance(it, c % spawnList.size());
				}
				Characters[ChCount].CType = *it;
				*/

				//DinoInfo[spawnGroup[sg].dinoIndex[indx]].SpawnInfo[spawnGroup[sg].spawnInfoIndex[indx]].spawnRatio;
				/*
				if (spawnGroup[sg].Randomised) Characters[ChCount].CType = spawnGroup[sg].dinoIndex[rRand(spawnGroup[sg].dinoIndexCh - 1)];
				else Characters[ChCount].CType = spawnGroup[sg].dinoIndex[c % spawnGroup[sg].dinoIndexCh];
				*/

				Characters[ChCount].CType = packType[packInd].packMember[0].ctype;
				Characters[ChCount].Clone = DinoInfo[Characters[ChCount].CType].Clone;
				Characters[ChCount].packDensity = packType[packInd].packDensity;
				Characters[ChCount].SpawnGroupType = sg;
				
				int leaderIndex = ChCount;
				// pack leaders
				spawnMapAmbient(tr, -1, spawnGroup[sg].moveForward);

				//pack size
				if (spawnGroup[sg].moveForward) {
					Characters[ChCount].packId = -1;
					ChCount++;
				} else {
				
					int packNo = 1;
					if (packType[packInd].packMax > 1) {//!spawnGroup[sg].moveForward
						packNo = packType[packInd].packMin;
						if (packType[packInd].packMax != packType[packInd].packMin) {
							for (int i = 0; i < packType[packInd].packMax - packType[packInd].packMin; i++) {
								if (1 == rRand(2)) packNo++;
							}
						}
					}

					ChCount++;


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

							Characters[ChCount].CType = packType[packInd].packMember[0].ctype; //failsafe

							//recalculate every time a member is added
							float memberRatio = 0;
							for (int pm = 0; pm < packType[packInd].packMemberCh; pm++) {
								memberRatio += packType[packInd].packMember[pm].ratio;
							}
							float memberSelector = rRand(30000);
							memberSelector /= 30000;
							memberSelector *= memberRatio;
							for (int pm = 0; pm < packType[packInd].packMemberCh; pm++) {
								if (memberSelector <= packType[packInd].packMember[pm].ratio) {
									Characters[ChCount].CType = packType[packInd].packMember[pm].ctype;
									break;
								}
								else memberSelector -= packType[packInd].packMember[pm].ratio;
							}

							//Characters[ChCount].CType = packType[packInd].packMember[rRand(packType[packInd].packMemberCh - 1)].ctype;//Characters[leaderIndex].CType;
							Characters[ChCount].SpawnGroupType = sg;
							Characters[ChCount].packDensity = Characters[leaderIndex].packDensity;
							spawnMapAmbient(tr, leaderIndex, false);
							ChCount++;
						}
						PackCount++;
					}
					else Characters[leaderIndex].packId = -1;

				}

				//ChCount++;

				if (tr > 10500) break;



			}
		}
		
	}

	//END NEW SYSTEM




	//======== lohs =========//
	/*
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

		if (Characters[ChCount].Clone == AI_DIMOR ||
			Characters[ChCount].Clone == AI_PTERA)
			Characters[ChCount].pos.y += DinoInfo[Characters[ChCount].CType].minDepth;

		Characters[ChCount].tgx = Characters[ChCount].pos.x;
		Characters[ChCount].tgz = Characters[ChCount].pos.z;
		Characters[ChCount].tgtime = 0;


		ChCount++;
	}
	*/

	/*
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
	*/


	//MAP AMBIENTS

	/*
	for (int i9 = 0; i9 < TotalMA; i9++) {

		int DinoInfoIndex = AI_to_CIndex[i9 + AI_FINAL + 1];

		for (int r = 0; r < DinoInfo[DinoInfoIndex].RegionCount; r++) {

			int RegionNo = DinoInfo[DinoInfoIndex].RType0[r];


			//spawn count
			int SpwnMax = Region[RegionNo].SpawnMax;
			if ( CiskMode && DinoInfo[DinoInfoIndex].Clone > 0) {
				if (Region[RegionNo].SpawnRate * 1000 > rRand(10000)){
				SpwnMax *= 10;
				char buff[100];
				sprintf_s(buff, "Influx:%s", DinoInfo[DinoInfoIndex].Name);
				MessageBox(hwndMain, buff, "TEST", IDOK);
				}
				else if (Region[RegionNo].SpawnRate * 1000 > rRand(100000)) {
					SpwnMax *= 50;
					char buff[100];
					sprintf_s(buff, "Unprecidented Influx:%s", DinoInfo[DinoInfoIndex].Name);
					MessageBox(hwndMain, buff, "TEST", IDOK);
				}
			}
			int spawnNo = Region[RegionNo].SpawnMin;
			for (int i = 0; i < SpwnMax - Region[RegionNo].SpawnMin; i++) { 
				if (Region[RegionNo].SpawnRate * 1000 > rRand(1000)) spawnNo++;
			}

			if (!spawnNo) {
				if (CiskMode && DinoInfo[DinoInfoIndex].Clone > 0 && Region[RegionNo].SpawnRate * 1000 > rRand(4000)) {//fake
					dispSighting(DinoInfoIndex, rRand(944) + 40, rRand(944) + 40);
				}
			}

			tr = 0;
			for (int spawnN = 0; spawnN < spawnNo; spawnN++)
			{

				int leaderIndex = ChCount;
				// pack leaders
				spawnMapAmbient(DinoInfoIndex, Region[RegionNo], tr, -1, RegionNo);

				if (CiskMode && DinoInfo[DinoInfoIndex].Clone > 0 && rRand(3) == 1) {//real
					dispSighting(DinoInfoIndex, (int)Characters[ChCount - 1].pos.x / 256, (int)Characters[ChCount - 1].pos.z / 256);
				}

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
						spawnMapAmbient(DinoInfoIndex, Region[RegionNo], tr, leaderIndex, RegionNo);
					}
					PackCount++;
				}
				else Characters[leaderIndex].packId = -1;

				if (tr > 10500) break;

			}


		}


	}
	*/



	// main
	/*
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
	*/

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
		if (!DinoInfo[cptr->CType].dontBend) {

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

		}


		cptr->pinfo->mptr->gVertex[v].x = xx * scale;
		cptr->pinfo->mptr->gVertex[v].y = cb * yy - sb * zz;
		cptr->pinfo->mptr->gVertex[v].z = cb * zz + sb * yy;


	}
}


void CreateMorphedModelBetaGamma(TModel* mptr, TAni *aptr, int FTime, float scale, float beta, float gamma) {

	int CurFrame, SplineD, PCurFrame, PSplineD;

	CurFrame = ((aptr->FramesCount - 1) * FTime * 256) / aptr->AniTime;
	SplineD = CurFrame & 0xFF;
	CurFrame = (CurFrame >> 8);

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


	int VCount = mptr->VCount;
	short int* adptr = aptr->aniData + CurFrame * VCount * 3;

	float sb = (float)sin(beta) * scale;
	float cb = (float)cos(beta) * scale;
	float sg = (float)sin(gamma);
	float cg = (float)cos(gamma);

	for (int v = 0; v < VCount; v++)
	{



		float x = *(adptr + v * 3 + 0) * k1 + *(adptr + (v + VCount) * 3 + 0) * k2;
		float y = *(adptr + v * 3 + 1) * k1 + *(adptr + (v + VCount) * 3 + 1) * k2;
		float z = -(*(adptr + v * 3 + 2) * k1 + *(adptr + (v + VCount) * 3 + 2) * k2);


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

		mptr->gVertex[v].x = xx * scale;
		mptr->gVertex[v].y = cb * yy - sb * zz;
		mptr->gVertex[v].z = cb * zz + sb * yy;


	}
}


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