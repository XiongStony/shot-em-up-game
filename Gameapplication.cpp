#include "head.h"
int game_time;
int position_x, position_y;//position of the plane
int width, length;//scope of the game
int bullet_x[10] = { 0 }, bullet_y[10] = { 0 }, pre_bullet_y[10];
int bulletnum = 0;
int object_x, object_y, enhanced_enemy;//敌人位置
int destroyed = 0;
int score = 0, score_plus = 0, consist_kill = 0, consist_farkill;
int velocity_x, velocity_y, time_gap;
int dispeared_time;
int input;
int health_point;
void startup()
{
	dispeared_time = 0;
	srand((unsigned)time(0) * 71);
	width = 20, length = 18;
	position_x = width / 2;
	position_y = length * 3 / 4;
	object_x = rand() % (width - 1);
	object_y = 0;
	enhanced_enemy = 0;
	health_point=1;
	for (int i = 0; i < 10; i++)
	{
		bullet_x[i] = -1;
		bullet_y[i] = -1;
	}
	velocity_x = 1, velocity_y = 1, time_gap = 3;
	std::cout << "w a s d 移动。空格键发射子弹。p键退出" << std::endl;
	std::cout << "press any key to start" << std::endl;
	_getch();
}
void show()
{
	system("cls");
	for (int j = 0; j < length; j++)
	{
		for (int i = 0; i < width; i++)
		{
			if ((i == position_x) && (j == position_y))
				printf("A");
			else if ((i == object_x) && (j == object_y))
			{
				if (enhanced_enemy == 0)
					printf("@");
				else
					printf("$");
			}
			else
			{
				int bulletPrinted = 0;
				for (int z = 0; z < 10; z++)
				{
					if ((i == bullet_x[z]) && (j == bullet_y[z]))
					{
						printf("|");
						bulletPrinted = 1;
						break;
					}
				}
				if (!bulletPrinted)
				{
					if (i == width - 1)
						printf("||");
					else if (j == length - 1)
						printf("=");
					else
						printf(" ");
				}
			}
		}
		printf("\n");
	}
	std::cout << "\n得分=" << score << "  本次得分=" << score_plus << "  游戏计时= " << game_time <<" enhanced enemy=" <<enhanced_enemy
		<<"health_point=" << health_point << std::endl;
	std::cout << "w a s d 移动。空格键发射子弹。p键结束" << std::endl;
}
void updateinput()
{
	static int consistshoot = 0;
	int bulletremain = 0;
	for (int i = 0; i < 10; i++)
	{
		if (bullet_y[i] == -1)
		{
			bulletremain = 1;
			break;
		}
	}
	if (GetAsyncKeyState('A') && (position_x > 0))
		position_x--;
	if (GetAsyncKeyState('D') && (position_x < width - 2))
		position_x++;
	if (GetAsyncKeyState('W') && (position_y > 0))
		position_y--;
	if (GetAsyncKeyState('S') && (position_y < length - 2))
		position_y++;
	if (GetAsyncKeyState(VK_SPACE))
	{
		consistshoot++;
		if (bulletnum >= 0 && bulletnum < 10 && bulletremain && consistshoot % 2 == 1)
		{
			bullet_x[bulletnum] = position_x;
			bullet_y[bulletnum] = position_y - 1;
			bulletnum++;
		}
		if (bulletnum >= 10)
			bulletnum = 0;
	}
	else
		consistshoot = 0;
	if (_kbhit())
	{
		input = _getch();
	}
}
void updatewithoutinput()
{
	static int enemy_round = 0;
	if (!destroyed)
	{	
		if (game_time % time_gap == 1)
		{
			object_x += velocity_x;
			if (object_x <= 0)
			{
				velocity_x = 1;
			}
			else if (object_x >= width - 2)
			{
				velocity_x = -1;
			}
		}
		if (game_time % (time_gap * 3) == 1)
		{
			object_y += velocity_y;
			if (object_y >= length - 1)
			{
				destroyed = 1;
				consist_kill = 0;
				consist_farkill = 0;
			}
		}
	}
	srand((unsigned)time(0) * 71);
	for (int i = 0; i < 10; i++)
	{
		if (bullet_y[i] >= 0)
			bullet_y[i]--;
	}
	for (int i = 0; i < 10; i++)	//得分系统
	{
		if ((bullet_y[i] == object_y) && (bullet_x[i] == object_x))
		{
			if (position_y - object_y >= 2 * length / 3)
			{				
					score_plus = 4 + consist_kill * 20 + consist_farkill;
					consist_farkill++;
			}
			else if (position_y - object_y >= length / 3)
			{
					score_plus = 2 + consist_kill * 20;
					consist_farkill = 0;
			}
			else
			{
					score_plus = 1 + consist_kill * 20;
					consist_farkill = 0;
			}
			bullet_y[i] = -1; bullet_y[i] = -1;
			health_point--;                      //生命值减少一
				if (health_point == 0) 
				{
					destroyed = 1;
					score += score_plus;
					consist_kill++;
				}
			break;
		}
	}
	for (int i = 0; i < 10; i++)	//加分清零系统
	{
		if (bullet_y[i] == -1 && pre_bullet_y[i] == 0)
		{
			consist_kill = 0;
		}
		pre_bullet_y[i] = bullet_y[i];
	}
	if (destroyed)
	{
		dispeared_time++;
		object_x = -100, object_y = -100;
		if (dispeared_time == 20)    //敌人出现
		{
			enemy_round++;
			if (enemy_round % 5 == 4)
				enhanced_enemy = 1;
			else
				enhanced_enemy = 0;
			object_x = rand() % (width - 1);
			object_y = 0;
			if (enhanced_enemy == 1)
				health_point = 2;
			else
				health_point = 1;
			destroyed = 0;
			dispeared_time = 0;
		}
	}
	game_time++;
}
int main()
{
	startup();
	while (input != 'p')
	{
		show();
		updatewithoutinput();
		updateinput();
		Sleep(50);
	}
	return 0;
}