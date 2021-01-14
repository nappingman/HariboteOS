#include "bootpack.h"

extern char super[50][50];
extern char pacman[50][50];
extern char ghost_1[50][50];
extern char ghost_2[50][50];
extern char ghost_angry_1[50][50];
extern char ghost_angry_2[50][50];
extern int  fake_rand[100];
extern int pac_changing;
extern int rand_indix;

extern int check_boundary(struct SHEET*sht,int next_x, int next_y);

int i=0;
int layer=2;

void Set_Status(struct Ghost* ghost,int status)
{
	ghost->status=status;
}
void Game_Init(struct Game* game, struct MEMMAN *memman, struct Pac_Man* pac_man, struct Ghost* ghost)
{
	Game_Start(game,pac_man,ghost);
	return;
}
void Game_Start(struct Game* game, struct Pac_Man* pac_man, struct Ghost* ghost)
{
	game->game_status			=1;
	game->pac_status			=1;
	game->ghost_status		=0;
	game->pac_speed				=PAC_DEFEND_SPEED;
	game->ghost_speed			=GHOST_ATTACK_SPEED;
	game->super_beans_num	=SUPER_BEAN_NUM;
	game->ghost_num				=GHOST_NUM;
	game->ture_man_time		=5;
	game->using_time  		=0;
	game->score						=0;
	return;
}
void Game_Over(struct Pac_Man* pac_man, struct Ghost* ghost)
{
	Pac_Show(pac_man);
	pac_man->status=-1;
}
void Pac_Init(struct SHTCTL *shtctl, struct Game* game, struct Pac_Man* pac_man,int start_x, int start_y)
{
	pac_man->sht_pac=sheet_alloc(shtctl);
	sheet_setbuf(pac_man->sht_pac, pac_man->buf_pac, PAC_SIZE, PAC_SIZE, 0);
	sheet_updown(pac_man->sht_pac,-1);
	sheet_slide(pac_man->sht_pac,start_x,start_y);

	pac_man->status				=1;
	pac_man->suit					=1;
	pac_man->color				=COL8_FFFF00;
	pac_man->cur_x				=start_x;
	pac_man->cur_y				=start_y;
	pac_man->next_x				=start_x;
	pac_man->next_y				=start_y;
	pac_man->sht_pac->vx0	=start_x;
	pac_man->sht_pac->vy0	=start_y;
	pac_man->direction		=0;
	pac_man->speed				=game->pac_speed;

	Pac_Show(pac_man);
}
void Gho_Init(struct SHTCTL *shtctl, struct Game* game, struct Ghost* ghost,int start_x,int start_y,int gho_c)
{
	ghost->sht_gho=sheet_alloc(shtctl);
	sheet_setbuf(ghost->sht_gho, ghost->buf_gho, GHOST_SIZE, GHOST_SIZE, 0);
	sheet_updown(ghost->sht_gho,-1);
	sheet_slide(ghost->sht_gho,start_x,start_y);

	ghost->status			=0;
	ghost->suit				=1;
	ghost->color			=gho_c;
	ghost->cur_x			=start_x;
	ghost->cur_y			=start_y;
	ghost->next_x			=start_x;
	ghost->next_y			=start_y;
	ghost->direction	=0;
	ghost->speed			=game->ghost_speed;

	Gho_Show(ghost);
}
void Pac_Show(struct Pac_Man* pac_man)
{
	if(pac_man->status==-1)
		return;
	put_figure_sht(pac_man->sht_pac, 0,0, (char*)pacman, 	  PAC_SIZE, PAC_SIZE, GAME_BGC,pac_man->color,pac_man->direction);
	sheet_slide(pac_man->sht_pac,pac_man->next_x,pac_man->next_y);
}
void Gho_Show(struct Ghost* ghost)
{
	if(ghost->status==-1)
		return;

	if(pac_changing==1)
	{
		if(ghost->suit==1)			put_figure_sht(ghost->sht_gho, 0,0, (char*)ghost_1, 	  	GHOST_SIZE, GHOST_SIZE, GAME_BGC, ghost->color, ghost->direction);
		else if(ghost->suit==0)	put_figure_sht(ghost->sht_gho, 0,0, (char*)ghost_angry_1, GHOST_SIZE, GHOST_SIZE, GAME_BGC, ghost->color, ghost->direction);
	}
	else
	{
		if(ghost->suit==1)			put_figure_sht(ghost->sht_gho, 0,0, (char*)ghost_2, 	 	 	GHOST_SIZE, GHOST_SIZE, GAME_BGC, ghost->color, ghost->direction);
		else if(ghost->suit==0)	put_figure_sht(ghost->sht_gho, 0,0, (char*)ghost_angry_2, GHOST_SIZE, GHOST_SIZE, GAME_BGC, ghost->color, ghost->direction);
	}

	sheet_slide(ghost->sht_gho,ghost->next_x,ghost->next_y);
}
void Pac_Move(struct SHEET*sht,struct Pac_Man* pac_man)
{
	pac_man->next_x=pac_man->sht_pac->vx0;
	pac_man->next_y=pac_man->sht_pac->vy0;

	if	   (pac_man->direction	==1	)		pac_man->next_y-=pac_man->speed;
	else if(pac_man->direction	==-1)		pac_man->next_y+=pac_man->speed;
	else if(pac_man->direction	==2	)		pac_man->next_x-=pac_man->speed;
	else if(pac_man->direction	==-2)		pac_man->next_x+=pac_man->speed;
	else if(pac_man->direction	==0	)							      						   ;

	if(pac_man->next_x								<	GAM_X_MIN)	pac_man->next_x=GAM_X_MIN;
	else if(pac_man->next_x+PAC_SIZE	>	GAM_X_MAX)	pac_man->next_x=GAM_X_MAX-PAC_SIZE;
	if(pac_man->next_y								<	GAM_Y_MIN)	pac_man->next_y=GAM_Y_MIN;
	else if(pac_man->next_y+PAC_SIZE	>	GAM_Y_MAX)	pac_man->next_y=GAM_Y_MAX-PAC_SIZE;

	int flag=check_boundary(sht,pac_man->next_x,pac_man->next_y);

	if(flag==1);
	else if(flag==-1)
	{
		while(check_boundary(sht,pac_man->next_x,pac_man->next_y)!=1)
		{
			if(pac_man->direction==-2)	pac_man->next_x--;
			if(pac_man->direction==-1)	pac_man->next_y--;
			if(pac_man->direction== 2)	pac_man->next_x++;
			if(pac_man->direction== 1)	pac_man->next_y++;
		}
	}
	else
	{
		pac_man->next_x=pac_man->sht_pac->vx0;
		pac_man->next_y=pac_man->sht_pac->vy0;
	}

	Pac_Show(pac_man);
}
void Gho_Move(struct SHEET*sht,struct Ghost* ghost)
{
	ghost->next_x=ghost->sht_gho->vx0;
	ghost->next_y=ghost->sht_gho->vy0;

	if		(ghost->direction		==1	)		ghost->next_y-=ghost->speed;
	else if(ghost->direction	==-1)		ghost->next_y+=ghost->speed;
	else if(ghost->direction	==2	)		ghost->next_x-=ghost->speed;
	else if(ghost->direction	==-2)		ghost->next_x+=ghost->speed;
	else if(ghost->direction	==0	)							   							 ;

	if(ghost->next_x									<	GAM_X_MIN)	ghost->next_x=GAM_X_MIN;
	else if(ghost->next_x+GHOST_SIZE	>	GAM_X_MAX)	ghost->next_x=GAM_X_MAX-GHOST_SIZE;
	if(ghost->next_y									<	GAM_Y_MIN)	ghost->next_y=GAM_Y_MIN;
	else if(ghost->next_y+GHOST_SIZE	>	GAM_Y_MAX)	ghost->next_y=GAM_Y_MAX-GHOST_SIZE;

	int flag=check_boundary(sht,ghost->next_x,ghost->next_y);

	if(flag==1);
	else if(flag==-1)
	{
		while(check_boundary(sht,ghost->next_x,ghost->next_y)!=1)
		{
			if(ghost->direction==-2)	ghost->next_x--;
			if(ghost->direction==-1)	ghost->next_y--;
			if(ghost->direction== 2)	ghost->next_x++;
			if(ghost->direction== 1)	ghost->next_y++;
		}
		if 		(ghost->direction		==2	)	ghost->direction	=1;
		else if(ghost->direction	==-2)	ghost->direction	=-1;
		else if(ghost->direction	==1	)	ghost->direction	=2;
		else if(ghost->direction	==-1)	ghost->direction	=-2;
	}
	else
	{
		ghost->next_x=ghost->sht_gho->vx0;
		ghost->next_y=ghost->sht_gho->vy0;
		ghost->direction=-ghost->direction;
	}

	Gho_Show(ghost);
}
void Pac_Die(struct Pac_Man* pac_man,struct Ghost* ghost)
{
	pac_man->color=COL8_C6C6C6;
	pac_man->speed=0;
	pac_man->suit=-1;

	Game_Over(pac_man,ghost);
}
void Gho_Die(struct Ghost* ghost)
{
	ghost->suit=0;
	ghost->color=COL8_C6C6C6;
	ghost->speed=0;

	Gho_Show(ghost);
	ghost->status=-1;
}
int Pac_Meet_Ghost(struct Game* game,struct Pac_Man* pac_man, struct Ghost* ghost)
{
	if(ghost->status==-1)
		return 0;

	int pac_pos_x=(pac_man->sht_pac->vx0+PAC_SIZE)/2;
	int pac_pos_y=(pac_man->sht_pac->vy0+PAC_SIZE)/2;

	int gho_pos_x=(ghost->sht_gho->vx0+GHOST_SIZE)/2;
	int gho_pos_y=(ghost->sht_gho->vy0+GHOST_SIZE)/2;

	int distance=(pac_pos_x-gho_pos_x)*(pac_pos_x-gho_pos_x)+(pac_pos_y-gho_pos_y)*(pac_pos_y-gho_pos_y);

	if(distance<PAC_SIZE*PAC_SIZE/10)
	/* "TWO WORLD MEET, ONE SURVIVE" ---OTOMAS */
	{
		if(game->pac_status==1)//PACMAN ATTACKING
		{
			Gho_Die(ghost);
			game->score+=200;
			return 1;
		}
		else
		{
			Pac_Die(pac_man,ghost);
			return -1;
		}
	}
}
int Pac_Meet_Bean(struct SHEET*sht, struct Game* game,struct Pac_Man* pac_man, struct Bean* bean,int xsize, int ysize)
{
	int pac_pos_x=(pac_man->sht_pac->vx0+PAC_SIZE)/2;
	int pac_pos_y=(pac_man->sht_pac->vy0+PAC_SIZE)/2;

	int bea_pos_x=(bean->sht_bea->vx0+GHOST_SIZE)/2;
	int bea_pos_y=(bean->sht_bea->vy0+GHOST_SIZE)/2;

	int distance=(pac_pos_x-bea_pos_x)*(pac_pos_x-bea_pos_x)+(pac_pos_y-bea_pos_y)*(pac_pos_y-bea_pos_y);

	if(distance<PAC_SIZE*PAC_SIZE/10)
	/* "TWO WORLD MEET, ONE SURVIVE" ---OTOMAS */
	{
		if(game->pac_status==1)//PACMAN ATTACKING
			game->ture_man_time+=7;
		else
		{
			game->pac_status		=1;
			game->ture_man_time	=7;
		}
		game->score+=50;

		Creat_Baen_Randomly(sht,bean,xsize,ysize);
	}
}
int Gho_Meet_Bean(struct SHEET*sht, struct Game* game,struct Ghost* ghost, struct Bean* bean,int xsize, int ysize)
{
	int gho_pos_x=(ghost->sht_gho->vx0+GHOST_SIZE)/2;
	int gho_pos_y=(ghost->sht_gho->vy0+GHOST_SIZE)/2;

	int bea_pos_x=(bean->sht_bea->vx0+GHOST_SIZE)/2;
	int bea_pos_y=(bean->sht_bea->vy0+GHOST_SIZE)/2;

	int distance=(gho_pos_x-bea_pos_x)*(gho_pos_x-bea_pos_x)+(gho_pos_y-bea_pos_y)*(gho_pos_y-bea_pos_y);

	if(distance<PAC_SIZE*PAC_SIZE/10)
	/* "TWO WORLD MEET, ONE SURVIVE" ---OTOMAS */
	{
		Creat_Baen_Randomly(sht,bean,xsize,ysize);
		return 1;
	}
	return -1;
}
void Creat_Baen_Randomly(struct SHEET*sht ,struct Bean* bean,int xsize, int ysize)
{
	int base=0;
	int k=fake_rand[(rand_indix++)%100];

	int per_x=xsize/10;
	int per_y=ysize/10;

	bean->cur_x=k*per_x;
	bean->cur_y=k*per_y;

	while(check_boundary(sht,bean->cur_x,bean->cur_y)!=1)
	{
		k=fake_rand[(rand_indix++)%100];
		if(rand_indix==100)
			rand_indix=0;
		bean->cur_x=k*per_x;
		bean->cur_y=k*per_y;
	}

	bean->color=(rand_indix)%20+7;

	put_figure_sht(bean->sht_bea,0,0, (char*)super,  GHOST_SIZE, GHOST_SIZE, GAME_BGC, bean->color, 0);
	sheet_updown(bean->sht_bea,7);
	sheet_slide(bean->sht_bea,bean->cur_x,bean->cur_y);
}
