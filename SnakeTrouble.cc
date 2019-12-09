#include<iostream>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<Windows.h>
#define PI 3.1415926535
//settings
HANDLE OutPut,OutBuf;
COORD coord={0,0};
DWORD bytes=0;
int output_flag=0;
double SIZE_X=200;
double SIZE_Y=60;
double LENGTH=1;
double ANGULAR_SPEED=0.15;
int INI_ARMOR=0;
int SHELL_SPEED=2;
double SHELL_RANGE=2;
double WALL_RANGE=1;
double FOOD_RANGE=3;
double MINE_RANGE=8;
int FREQUENCY=60;
int FOOD_FREQUENCY=10;
int FIRE_FREQUENCY=200;
int BOMB_FREQUENCY=2000;
enum Mode{Attack,Bomb,Feed,Dodge};
//class declearation
class Wall;
class SnakeNode
{
public:
    double X;
    double Y;
    int Armor;
    SnakeNode*next;
    SnakeNode*back;
};
class SnakeHead:public SnakeNode
{
public:
    double Angle;
};
class Snake
{
public:
    Snake();
    ~Snake();
    void move();
    void longer();
    void shorter();
    void strike(SnakeNode*,Wall&);
    SnakeHead*Head;
    SnakeNode*Tail;
};
class Wall
{
public:
    Wall();
    ~Wall();    
    void split(Snake&,SnakeNode*);
    void strike(SnakeNode*&);
    void remove(SnakeNode*&);
    double R;
    SnakeNode*Head;
};
class ShellNode
{
public:
    double X;
    double Y;
    double R;
    double Angle;
    bool AI;
    ShellNode*next;
    ShellNode*back;
};
class Shell
{
public:
    Shell();
    ~Shell();
    void fire(Snake&,bool);
    void remove(ShellNode*&);
    void move();
    int t;
    ShellNode*Head;
};
class FoodNode
{
public:
    double X;
    double Y;
    FoodNode*next;
    FoodNode*back;
};
class Food
{
public:
    Food();
    ~Food();
    void remove(FoodNode*&);
    void create();
    FoodNode*Head;
};
class MineNode
{
public:
    double X;
    double Y;
    int T_INI;
    int T_WAIT;
    int N;
    int Activate;
    int Print;
    MineNode*next;
    MineNode*back;
};
class Mine
{
public:
    Mine();
    ~Mine();
    void add(Snake&,int);
    void remove(MineNode*&);
    void explode(Shell&,MineNode*&);
    int t[2];
    MineNode*Head;
};
//function declarations
void Output(Snake&snake1,Snake&snake2,Wall&wall,Shell&shell,Food&food,Mine&mine);
template<class ObjectNode1,class ObjectNode2>
inline double SquareDistence(ObjectNode1*o1,ObjectNode2*o2);
void MoveFood(Snake&snake,Food&food);
//AI classes
class EnemyInfo
{
public:
    double direction_angle;
    double motion_angle;
    double distence;
};
class Vision
{
public:
    int CountMine(Snake&,Mine&,double,double,double);
    int CountShell(Snake&,Shell&,double,double,double);
    int CountWall(Snake&,Wall&,double,double,double);
    int CountSWall(Snake&,Snake&,double,double,double);
    int CountFood(Snake&,Food&,double,double,double);
    int CountAmmo(Snake&);
    void Enemy(Snake&,Snake&);
    template<class ObjectNode>
    void GetAngle(ObjectNode*,Snake&);
    int mine_num;
    int shell_num;
    int wall_num;
    int s_wall_num;
    int food_num;
    int ammo;
    EnemyInfo enemy;
private:
    double angle;
};
class MovingPattern
{
public:
    MovingPattern();
    void Forward(Snake&,Food&);
    void Left(Snake&,Food&);
    void Right(Snake&,Food&);
    void TurnAroundRight(Snake&,Food&);
    void TurnAroundLeft(Snake&,Food&);
    void Bombing(Vision&eye,Snake&ai,Snake&player,Wall&wall,Shell&shell,Food&food,Mine&mine);
    void Attacking(Vision&eye,Snake&ai,Snake&player,Wall&wall,Shell&shell,Food&food,Mine&mine);
    void Feeding(Vision&eye,Snake&ai,Snake&player,Wall&wall,Shell&shell,Food&food,Mine&mine);
    void Dodging(Vision&eye,Snake&ai,Snake&player,Wall&wall,Shell&shell,Food&food,Mine&mine);
    void AILogic(Vision&eye,Snake&ai,Snake&player,Wall&wall,Shell&shell,Food&food,Mine&mine);
    bool changemode;    
    int modecount;
    Mode currentmode;
};
//AI functions
template<class ObjectNode>
inline void Vision::GetAngle(ObjectNode*pr,Snake&snake)
{
    angle=atan((pr->Y-snake.Head->Y)/(pr->X-snake.Head->X));
    if (snake.Head->Y-pr->Y>0&&angle>0)
        angle-=PI;
    if(snake.Head->Y-pr->Y<0&&angle<0)
        angle+=PI;
}
int Vision::CountMine(Snake&snake,Mine&mine,double distence,double start_angle,double end_angle)
{
    MineNode*pr;
    for(mine_num=0,pr=mine.Head->next;pr!=mine.Head;pr=pr->next)
    {   
        if(SquareDistence(pr,snake.Head)>distence*distence) continue;
        if(pr->Print==0) continue;
        GetAngle(pr,snake);
        if(start_angle<angle-snake.Head->Angle&&angle-snake.Head->Angle<end_angle)
            mine_num++;
    }
    return mine_num;
}
int Vision::CountShell(Snake&snake,Shell&shell,double distence,double start_angle,double end_angle)
{
    ShellNode*pr;
    for(shell_num=0,pr=shell.Head->next;pr!=shell.Head;pr=pr->next)
    {   
        if(pr->AI==true) continue;
        if(SquareDistence(pr,snake.Head)>distence*distence) continue;
        GetAngle(pr,snake);
        if(start_angle<angle-snake.Head->Angle&&angle-snake.Head->Angle<end_angle)
            shell_num++;
    }
    return shell_num;
}
int Vision::CountWall(Snake&snake,Wall&wall,double distence,double start_angle,double end_angle)
{
    SnakeNode*pr;
    for(wall_num=0,pr=wall.Head->next;pr!=wall.Head;pr=pr->next)
    {   
        if(SquareDistence(pr,snake.Head)>distence*distence) continue;
        GetAngle(pr,snake);
        if(start_angle<angle-snake.Head->Angle&&angle-snake.Head->Angle<end_angle)
            wall_num++;
    }
    return wall_num;
}
int Vision::CountSWall(Snake&snake,Snake&s_wall,double distence,double start_angle,double end_angle)
{
    SnakeNode*pr;
    for(s_wall_num=0,pr=s_wall.Head->next;pr!=NULL;pr=pr->next)
    {   
        if(SquareDistence(pr,snake.Head)>distence*distence) continue;
        GetAngle(pr,snake);
        if(start_angle<angle-snake.Head->Angle&&angle-snake.Head->Angle<end_angle)
            s_wall_num++;
    }
    return s_wall_num;
}
int Vision::CountFood(Snake&snake,Food&food,double distence,double start_angle,double end_angle)
{
    FoodNode*pr;
    for(food_num=0,pr=food.Head->next;pr!=food.Head;pr=pr->next)
    {   
        if(SquareDistence(pr,snake.Head)>distence*distence) continue;
        GetAngle(pr,snake);
        if(start_angle<angle-snake.Head->Angle&&angle-snake.Head->Angle<end_angle)
            food_num++;
    }
    return food_num;
}
inline void Vision::Enemy(Snake&snake,Snake&player)
{
    GetAngle(player.Head,snake);
    enemy.direction_angle=angle-snake.Head->Angle;
    enemy.motion_angle=player.Head->Angle-snake.Head->Angle-angle;
}
int Vision::CountAmmo(Snake&snake)
{
    SnakeNode*pr;
    for(ammo=0,pr=snake.Head->next;pr!=snake.Tail;pr=pr->next)
        ammo++;
    return ammo;
}
MovingPattern::MovingPattern():changemode(false),modecount(20),currentmode(Feed){}
inline void MovingPattern::Forward(Snake&snake,Food&food)
{
    int x=rand()%10;
    if(x<3)
        snake.Head->Angle-=ANGULAR_SPEED,MoveFood(snake,food);
    else if(x<6)
        snake.Head->Angle+=ANGULAR_SPEED,MoveFood(snake,food);
    else 
        MoveFood(snake,food);
}
inline void MovingPattern::Left(Snake&snake,Food&food)
{
    int x=rand()%10;
    if(x<4)
        snake.Head->Angle-=ANGULAR_SPEED,MoveFood(snake,food),snake.Head->Angle-=ANGULAR_SPEED;
    else if(x<7)
        snake.Head->Angle+=ANGULAR_SPEED,MoveFood(snake,food),snake.Head->Angle-=ANGULAR_SPEED;
    else 
        MoveFood(snake,food);
}
inline void MovingPattern::Right(Snake&snake,Food&food)
{
    int x=rand()%10;
    if(x<4)
        snake.Head->Angle+=ANGULAR_SPEED,MoveFood(snake,food),snake.Head->Angle+=ANGULAR_SPEED;
    else if(x<7)
        snake.Head->Angle-=ANGULAR_SPEED,MoveFood(snake,food),snake.Head->Angle+=ANGULAR_SPEED;
    else 
        MoveFood(snake,food);
}
inline void MovingPattern::TurnAroundRight(Snake&snake,Food&food)
{
        snake.Head->Angle+=2*ANGULAR_SPEED,MoveFood(snake,food);
}
inline void MovingPattern::TurnAroundLeft(Snake&snake,Food&food)
{
        snake.Head->Angle-=2*ANGULAR_SPEED,MoveFood(snake,food);
}
void MovingPattern::Attacking(Vision&eye,Snake&ai,Snake&player,Wall&wall,Shell&shell,Food&food,Mine&mine)
{
    shell.fire(ai,true);
    eye.CountAmmo(ai);
    if(eye.ammo<6) changemode=true;
    eye.CountWall(ai,wall,3,-PI/3,0);
    eye.CountShell(ai,shell,3,-PI/3,0);
    eye.CountSWall(ai,player,3,-PI/3,0);
    eye.CountMine(ai,mine,3,-PI/3,0);
    if((eye.wall_num+eye.s_wall_num+eye.shell_num+eye.mine_num)!=0)
    {
        TurnAroundRight(ai,food); 
        return;
    }    
    eye.CountWall(ai,wall,3,0,PI/3);
    eye.CountShell(ai,shell,3,0,PI/3);
    eye.CountSWall(ai,player,3,0,PI/3);
    eye.CountMine(ai,mine,3,0,PI/3);
    if((eye.wall_num+eye.s_wall_num+eye.shell_num+eye.mine_num)!=0)
    {
        TurnAroundLeft(ai,food);
        return;
    }
    eye.CountWall(ai,wall,5,-PI/6,0);
    eye.CountShell(ai,shell,5,-PI/6,0);
    eye.CountSWall(ai,player,5,-PI/6,0);
    eye.CountMine(ai,mine,5,-PI/6,0);
    if((eye.wall_num+eye.s_wall_num+eye.shell_num+eye.mine_num)!=0)
    {
        Right(ai,food);
        return;
    }
    eye.CountWall(ai,wall,5,0,PI/6);
    eye.CountShell(ai,shell,5,0,PI/6);
    eye.CountSWall(ai,player,5,0,PI/6);
    eye.CountMine(ai,mine,5,0,PI/6);
    if((eye.wall_num+eye.s_wall_num+eye.shell_num+eye.mine_num)!=0)
    {
        Left(ai,food);
        return;
    }
    eye.Enemy(ai,player);
    if(eye.enemy.direction_angle<0)
        Left(ai,food);
    else
        Right(ai,food);
}
void MovingPattern::Feeding(Vision&eye,Snake&ai,Snake&player,Wall&wall,Shell&shell,Food&food,Mine&mine)
{
    eye.CountWall(ai,wall,3,-PI/3,0);
    eye.CountShell(ai,shell,3,-PI/3,0);
    eye.CountSWall(ai,player,3,-PI/3,0);
    eye.CountMine(ai,mine,3,-PI/3,0);
    if((eye.wall_num+eye.s_wall_num+eye.shell_num+eye.mine_num)!=0)
    {
        shell.fire(ai,true);
        TurnAroundRight(ai,food); 
        return;
    }    
    eye.CountWall(ai,wall,3,0,PI/3);
    eye.CountShell(ai,shell,3,0,PI/3);
    eye.CountSWall(ai,player,3,0,PI/3);
    eye.CountMine(ai,mine,3,0,PI/3);
    if((eye.wall_num+eye.s_wall_num+eye.shell_num+eye.mine_num)!=0)
    {
        shell.fire(ai,true);
        TurnAroundLeft(ai,food);
        return;
    }
    eye.CountWall(ai,wall,5,-PI/6,0);
    eye.CountShell(ai,shell,5,-PI/6,0);
    eye.CountSWall(ai,player,5,-PI/6,0);
    eye.CountMine(ai,mine,5,-PI/6,0);
    if((eye.wall_num+eye.s_wall_num+eye.shell_num+eye.mine_num)!=0)
    {
        Right(ai,food);
        return;
    }
    eye.CountWall(ai,wall,5,0,PI/6);
    eye.CountShell(ai,shell,5,0,PI/6);
    eye.CountSWall(ai,player,5,0,PI/6);
    eye.CountMine(ai,mine,5,0,PI/6);
    if((eye.wall_num+eye.s_wall_num+eye.shell_num+eye.mine_num)!=0)
    {
        Left(ai,food);
        return;
    }
    int left=eye.CountFood(ai,food,10,-PI/6,0);
    int right=eye.CountFood(ai,food,10,0,PI/6);
    if(left>right)
        Left(ai,food);
    else
        Right(ai,food);
}
void MovingPattern::Dodging(Vision&eye,Snake&ai,Snake&player,Wall&wall,Shell&shell,Food&food,Mine&mine)
{
    eye.CountWall(ai,wall,3,-PI/3,0);
    eye.CountShell(ai,shell,3,-PI/3,0);
    eye.CountSWall(ai,player,3,-PI/3,0);
    eye.CountMine(ai,mine,3,-PI/3,0);
    if((eye.wall_num+eye.s_wall_num+eye.shell_num+eye.mine_num)!=0)
    {
        shell.fire(ai,true);
        TurnAroundRight(ai,food); 
        return;
    }    
    eye.CountWall(ai,wall,3,0,PI/3);
    eye.CountShell(ai,shell,3,0,PI/3);
    eye.CountSWall(ai,player,3,0,PI/3);
    eye.CountMine(ai,mine,3,0,PI/3);
    if((eye.wall_num+eye.s_wall_num+eye.shell_num+eye.mine_num)!=0)
    {
        shell.fire(ai,true);
        TurnAroundLeft(ai,food);
        return;
    }
    eye.CountWall(ai,wall,5,-PI/6,0);
    eye.CountShell(ai,shell,5,-PI/6,0);
    eye.CountSWall(ai,player,5,-PI/6,0);
    eye.CountMine(ai,mine,5,-PI/6,0);
    if((eye.wall_num+eye.s_wall_num+eye.shell_num+eye.mine_num)!=0)
    {
        Right(ai,food);
        return;
    }
    eye.CountWall(ai,wall,5,0,PI/6);
    eye.CountShell(ai,shell,5,0,PI/6);
    eye.CountSWall(ai,player,5,0,PI/6);
    eye.CountMine(ai,mine,5,0,PI/6);
    if((eye.wall_num+eye.s_wall_num+eye.shell_num+eye.mine_num)!=0)
    {
        Left(ai,food);
        return;
    }
    eye.Enemy(ai,player);
    if(eye.enemy.direction_angle<PI/6&&eye.enemy.direction_angle>-PI/6)
    {
        if(eye.CountShell(ai,shell,6,-PI/6,0))
        {
            Right(ai,food);
            return;
        }
        if(eye.CountShell(ai,shell,6,0,PI/6))
        {
            Left(ai,food);
            return;
        }
    }
    if(eye.enemy.direction_angle<0)
        Right(ai,food);
    else
        Left(ai,food);
}
void MovingPattern::Bombing(Vision&eye,Snake&ai,Snake&player,Wall&wall,Shell&shell,Food&food,Mine&mine)
{
    mine.add(ai,2);
    mine.add(ai,2);
    mine.add(ai,2);
    eye.CountWall(ai,wall,3,-PI/3,0);
    eye.CountShell(ai,shell,3,-PI/3,0);
    eye.CountSWall(ai,player,3,-PI/3,0);
    eye.CountMine(ai,mine,3,-PI/3,0);
    if((eye.wall_num+eye.s_wall_num+eye.shell_num+eye.mine_num)!=0)
    {
        shell.fire(ai,true);
        TurnAroundRight(ai,food); 
        return;
    }    
    eye.CountWall(ai,wall,3,0,PI/3);
    eye.CountShell(ai,shell,3,0,PI/3);
    eye.CountSWall(ai,player,3,0,PI/3);
    eye.CountMine(ai,mine,3,0,PI/3);
    if((eye.wall_num+eye.s_wall_num+eye.shell_num+eye.mine_num)!=0)
    {
        shell.fire(ai,true);
        TurnAroundLeft(ai,food);
        return;
    }
    eye.CountWall(ai,wall,5,-PI/6,0);
    eye.CountShell(ai,shell,5,-PI/6,0);
    eye.CountSWall(ai,player,5,-PI/6,0);
    eye.CountMine(ai,mine,5,-PI/6,0);
    if((eye.wall_num+eye.s_wall_num+eye.shell_num+eye.mine_num)!=0)
    {
        Right(ai,food);
        return;
    }
    eye.CountWall(ai,wall,5,0,PI/6);
    eye.CountShell(ai,shell,5,0,PI/6);
    eye.CountSWall(ai,player,5,0,PI/6);
    eye.CountMine(ai,mine,5,0,PI/6);
    if((eye.wall_num+eye.s_wall_num+eye.shell_num+eye.mine_num)!=0)
    {
        Left(ai,food);
        return;
    }
    MoveFood(ai,food);
}
void MovingPattern::AILogic(Vision&eye,Snake&ai,Snake&player,Wall&wall,Shell&shell,Food&food,Mine&mine)
{
    if(modecount<20&&changemode==false)
    {
        switch (currentmode)
        {
        case Feed:
            Feeding(eye,ai,player,wall,shell,food,mine);
            break;
        case Attack:
            Attacking(eye,ai,player,wall,shell,food,mine);
            break;
        case Bomb:
            Bombing(eye,ai,player,wall,shell,food,mine);
            break;
        case Dodge:
            Dodging(eye,ai,player,wall,shell,food,mine);
            break;
        }
        modecount++;
        return;
    }
    else
    modecount=0;
    eye.Enemy(ai,player);
    eye.CountAmmo(ai);
    if(eye.enemy.direction_angle<PI/4&&eye.enemy.direction_angle>-PI/4&&eye.enemy.distence<20&&eye.ammo>12)
    {
        currentmode=Attack;
        return;
    }
    if((eye.enemy.direction_angle>2*PI/3||eye.enemy.direction_angle<-2*PI/3)&&eye.enemy.distence<15)
    {
        currentmode=Dodge;
        return;
    }
    if(eye.ammo<10)
    {
        currentmode=Feed;
        return;
    }
    if(rand()%5==0)
        currentmode=Bomb;
    else
        currentmode=Feed;
}
//functions in the classes
Mine::Mine()
{
    Head=new MineNode;
    Head->back=Head;
    Head->next=Head;
    Head->T_INI=0;
    Head->T_WAIT=0;
    Head->Activate=0;
    Head->Print=0;
    Head->X=0;
    Head->Y=0;
    t[0]=GetTickCount();
    t[1]=GetTickCount();
}
Mine::~Mine()
{
    MineNode*pr=Head->next;
    while (pr!=Head)
    {
        pr=pr->next;
        delete pr->back;
    }
    delete Head;
}
void Mine::add(Snake&snake,int i)
{
    if(snake.Head->next==snake.Tail) return;
    if(i!=2)
    {
        int now=GetTickCount();
        if(now-t[i]<BOMB_FREQUENCY) return;
        t[i]=now;
    }
    MineNode*mine=new MineNode;
    mine->T_INI=GetTickCount();
    mine->T_WAIT=rand()%(30000-5000)+5000;
    mine->X=snake.Tail->X;
    mine->Y=snake.Tail->Y;
    mine->N=rand()%(15-1)+1;
    mine->back=Head;
    mine->next=Head->next;
    mine->Activate=0;
    mine->Print=0;
    Head->next->back=mine;
    Head->next=mine;
    snake.shorter();
}
void Mine::remove(MineNode*&pr)
{
    MineNode*pr1=pr;
    pr=pr->back;
    pr1->back->next=pr1->next;
    pr1->next->back=pr1->back;
    delete pr1;
}
void Mine::explode(Shell&head,MineNode*&mine)
{
    mine->N--;
    if(mine->N<0) {remove(mine);return;}
    ShellNode*shell=new ShellNode;
    shell->Angle=((double)rand())/RAND_MAX*2*PI;
    shell->R=SHELL_RANGE/2;
    shell->X=mine->X;
    shell->Y=mine->Y;
    shell->back=head.Head;
    shell->next=head.Head->next;
    head.Head->next->back=shell;
    head.Head->next=shell;
}
Snake::Snake()
{
    Head=new SnakeHead;
    Tail=new SnakeNode;
    Head->next=Tail;
    Head->back=NULL;
    Tail->back=Head;
    Tail->next=NULL;
    Head->X=((double)rand())/RAND_MAX*SIZE_X;
    Head->Y=((double)rand())/RAND_MAX*SIZE_Y;
    Head->Angle=((double)rand())/RAND_MAX*2*PI-PI;
    Head->Armor=0;
    Tail->Armor=INI_ARMOR;
    Tail->X=Head->X-LENGTH*cos(Head->Angle);
    Tail->Y=Head->Y-LENGTH*sin(Head->Angle);
}
Snake::~Snake()
{
    SnakeNode*pr=Head;
    while(pr->next!=NULL)
    {
        pr=pr->next;
        delete pr->back;
    }
    delete pr;
}
void Snake::longer()
{
    SnakeNode*pr=new SnakeNode;
    pr->X=Head->X;
    pr->Y=Head->Y;
    pr->back=Head;
    pr->next=Head->next;
    pr->Armor=INI_ARMOR;
    Head->next->back=pr;
    Head->next=pr;
    Head->X+=LENGTH*cos(Head->Angle);
    Head->Y+=LENGTH*sin(Head->Angle);
}
inline void Snake::shorter()
{
    Tail=Tail->back;
    delete Tail->next;
    Tail->next=NULL;
}
void Snake::move()
{
    SnakeNode*pr=new SnakeNode;
    pr->X=Head->X;
    pr->Y=Head->Y;
    pr->back=Head;
    pr->next=Head->next;
    Head->next->back=pr;
    Head->next=pr;
    Head->X+=LENGTH*cos(Head->Angle);
    Head->Y+=LENGTH*sin(Head->Angle);
    for(;pr!=Tail;pr=pr->next)
        pr->Armor=pr->next->Armor;
    Tail=Tail->back;
    delete Tail->next;
    Tail->next=NULL;
}
void Snake::strike(SnakeNode*pr,Wall&wall)
{
    pr->Armor-=1;
    if(pr->Armor<0&&pr!=Head->next)
        wall.split(*this,pr);
}
Shell::Shell()
{
    Head=new ShellNode;
    Head->back=Head;
    Head->next=Head;
    Head->Angle=0;
    Head->R=0;
    Head->X=0;
    Head->Y=0;
    t=GetTickCount();
}
Shell::~Shell()
{
    ShellNode*pr=Head->next;
    while (pr!=Head)
    {
        pr=pr->next;
        delete pr->back;
    }
    delete Head;
}
inline void Shell::remove(ShellNode*&pr)
{
    ShellNode*pr1=pr;
    pr=pr->back;
    pr1->back->next=pr1->next;
    pr1->next->back=pr1->back;
    delete pr1;
}
void Shell::fire(Snake&snake,bool ai=false)
{
    if(snake.Head->next==snake.Tail) return;
    int now=GetTickCount();
    if(now-t<FIRE_FREQUENCY) return;
    t=now;
    ShellNode*shell=new ShellNode;
    shell->Angle=snake.Head->Angle;
    shell->R=SHELL_RANGE;
    shell->X=snake.Head->X+1.1*shell->R*cos(shell->Angle);
    shell->Y=snake.Head->Y+1.1*shell->R*sin(shell->Angle);
    shell->back=Head;
    shell->next=Head->next;
    Head->next->back=shell;
    Head->next=shell;
    shell->AI=ai;
    snake.shorter();
}
inline void Shell::move()
{
    ShellNode*pr;
    for(pr=Head->next;pr!=Head;pr=pr->next)
    {   
        pr->X=pr->X+SHELL_SPEED*cos(pr->Angle);
        pr->Y=pr->Y+SHELL_SPEED*sin(pr->Angle);
    }
}
Wall::Wall()
{
    Head=new SnakeNode;
    Head->back=Head;
    Head->next=Head;
    Head->Armor=0;
    Head->X=0;
    Head->Y=0;
    R=WALL_RANGE;
}
Wall::~Wall()
{
    SnakeNode*pr=Head->next;
    while (pr!=Head)
    {
        pr=pr->next;
        delete pr->back;
    }
    delete Head;
}
inline void Wall::remove(SnakeNode*&pr)
{
    SnakeNode*pr1=pr;
    pr=pr->back;
    pr1->back->next=pr1->next;
    pr1->next->back=pr1->back;
    delete pr1;
}
void Wall::split(Snake&snake,SnakeNode*pr)
{
    snake.Tail->next=Head->next;
    Head->next->back=snake.Tail;
    snake.Tail=pr->back;
    snake.Tail->next=NULL;
    pr->next->back=Head;
    Head->next=pr->next;
    delete pr;
}
void Wall::strike(SnakeNode*&pr)
{
    pr->Armor-=1;
    if(pr->Armor<=0)
        remove(pr);
}
Food::Food()
{
    Head=new FoodNode;
    Head->back=Head;
    Head->next=Head;
    Head->X=0;
    Head->Y=0;
}
Food::~Food()
{
    FoodNode*pr=Head->next;
    while (pr!=Head)
    {
        pr=pr->next;
        delete pr->back;
    }
    delete Head;
}
inline void Food::remove(FoodNode*&pr)
{
    FoodNode*pr1=pr;
    pr=pr->back;
    pr1->back->next=pr1->next;
    pr1->next->back=pr1->back;
    delete pr1;
}
inline void Food::create()
{
    FoodNode*pr=new FoodNode;
    pr->X=((double)rand())/RAND_MAX*SIZE_X;
    pr->Y=((double)rand())/RAND_MAX*SIZE_Y;
    pr->next=Head->next;
    pr->back=Head;
    Head->next->back=pr;
    Head->next=pr;
}
//free functions
template<class ObjectNode1,class ObjectNode2>
inline double SquareDistence(ObjectNode1*o1,ObjectNode2*o2)
{
    return (o1->X-o2->X)*(o1->X-o2->X)+(o1->Y-o2->Y)*(o1->Y-o2->Y);
}
int ShellSnake(Shell&shell,Snake&snake,Wall&wall,bool ai=false)
{
    ShellNode*pr_shell=shell.Head->next;
    SnakeNode*pr_snake=snake.Head->next;
    while(pr_shell!=shell.Head)
    {
        if(ai==true&&pr_shell->AI==true)
        {
            pr_shell=pr_shell->next;
            continue;
        }
        if(SquareDistence(pr_shell,snake.Head)<pow(pr_shell->R,2))
        {
            snake.Head->Armor-=1;
            shell.remove(pr_shell);
            if(snake.Head->Armor<0)
                return 1;
        }
        for(pr_snake=snake.Head->next;pr_snake!=NULL;pr_snake=pr_snake->next)
            if(SquareDistence(pr_shell,pr_snake)<pow(pr_shell->R,2))
            {   
                snake.strike(pr_snake,wall);
                shell.remove(pr_shell);
                break;
            }
        pr_shell=pr_shell->next;
    }
    return 0;
}
void ShellWall(Shell&shell,Wall&wall)
{
    ShellNode*pr_shell;
    SnakeNode*pr_wall;
    for(pr_shell=shell.Head->next;pr_shell!=shell.Head;pr_shell=pr_shell->next)
        for(pr_wall=wall.Head->next;pr_wall!=wall.Head;pr_wall=pr_wall->next)
            if(SquareDistence(pr_shell,pr_wall)<pow(wall.R,2))
            {
                wall.strike(pr_wall),shell.remove(pr_shell);
                break;
            }
}
void ShellShell(Shell&shell)
{
    ShellNode*pr1;
    ShellNode*pr2;
    for(pr1=shell.Head->next;pr1!=shell.Head;pr1=pr1->next)
        for(pr2=pr1->next;pr2!=shell.Head;pr2=pr2->next)
            if(SquareDistence(pr1,pr2)<pow((pr1->R+pr2->R)/4,2))
            {    
                shell.remove(pr1),shell.remove(pr2);
                break;
            }
}
int SnakeSnake(Snake&snake1,Snake&snake2)
{
    SnakeNode*p1;
    SnakeNode*p2;
    for(p1=snake1.Head,p2=snake2.Head->next;p2!=NULL;p2=p2->next)
    {
        if(SquareDistence(p1,p2)<pow(WALL_RANGE,2))
            return 1;
    }
    for(p1=snake2.Head,p2=snake1.Head->next;p2!=NULL;p2=p2->next)
    {
        if(SquareDistence(p1,p2)<pow(WALL_RANGE,2))
            return 2;
    }
    return 0;
}
int SnakeWall(Snake&snake,Wall&wall)
{
    SnakeNode*pr_s;
    SnakeNode*pr_w;
    for(pr_s=snake.Head,pr_w=wall.Head->next;pr_w!=wall.Head;pr_w=pr_w->next)
    {
        if(SquareDistence(pr_s,pr_w)<pow(WALL_RANGE,2))
            return 1;
    }
    return 0;
}
void MoveFood(Snake&snake,Food&food)
{
    FoodNode*pr_f;
    for(pr_f=food.Head->next;pr_f!=food.Head;pr_f=pr_f->next)
        if(SquareDistence(pr_f,snake.Head)<pow(FOOD_RANGE,2))
        {   snake.longer();
            food.remove(pr_f);
            return;
        }
    snake.move();
}
void MineAction(Mine&mine,Shell&shell,int now)
{
    MineNode*pr;
    for(pr=mine.Head->next;pr!=mine.Head;pr=pr->next)
    {
        if(pr->Print==0)
        {
            if(now-pr->T_INI>pr->T_WAIT-3000)
                pr->Print=1;
            else continue;
        }
        if(pr->Activate==0)
        {
            if(now-pr->T_INI>pr->T_WAIT)
                pr->Activate=1;
            else continue;
        }
        if(pr->Activate==1)
            mine.explode(shell,pr);
    }
}
void Boundary(Snake&snake1,Snake&snake2,Shell&shell)
{
    if(snake1.Head->X<0)
        snake1.Head->X+=SIZE_X;
    if(snake1.Head->Y<0)
        snake1.Head->Y+=SIZE_Y;
    if(snake1.Head->X>SIZE_X)
        snake1.Head->X-=SIZE_X;
    if(snake1.Head->Y>SIZE_Y)
        snake1.Head->Y-=SIZE_Y;
    if(snake2.Head->X<0)
        snake2.Head->X+=SIZE_X;
    if(snake2.Head->Y<0)
        snake2.Head->Y+=SIZE_Y;
    if(snake2.Head->X>SIZE_X)
        snake2.Head->X-=SIZE_X;
    if(snake2.Head->Y>SIZE_Y)
        snake2.Head->Y-=SIZE_Y;
    if(snake1.Head->Angle>PI)
        snake1.Head->Angle-=2*PI;
    if(snake2.Head->Angle>PI)
        snake2.Head->Angle-=2*PI;
    if(snake1.Head->Angle<-PI)
        snake1.Head->Angle+=2*PI;
    if(snake2.Head->Angle<-PI)
        snake2.Head->Angle+=2*PI;
    ShellNode*pr=shell.Head->next;
    for(;pr!=shell.Head;pr=pr->next)
        if(pr->X<0||pr->Y<0||pr->X>SIZE_X||pr->Y>SIZE_Y)
            shell.remove(pr);
}
void KeyCtrl1(Snake&snake1,Wall&wall,Shell&shell,Food&food,Mine&mine)
{
    if(GetAsyncKeyState(VK_UP)&0x8000)
        MoveFood(snake1,food);
    if(GetAsyncKeyState(VK_LEFT)&0x8000)
        snake1.Head->Angle-=ANGULAR_SPEED;
    if(GetAsyncKeyState(VK_RIGHT)&0x8000)
        snake1.Head->Angle+=ANGULAR_SPEED;
    if(GetAsyncKeyState('L')&0x8000)
        shell.fire(snake1);
    if(GetAsyncKeyState(VK_DOWN)&0x8000)
        mine.add(snake1,0);
}
void KeyCtrl2(Snake&snake2,Wall&wall,Shell&shell,Food&food,Mine&mine)
{
    if(GetAsyncKeyState('T')&0x8000)
        MoveFood(snake2,food);
    if(GetAsyncKeyState('F')&0x8000)
        snake2.Head->Angle-=ANGULAR_SPEED;
    if(GetAsyncKeyState('H')&0x8000)
        snake2.Head->Angle+=ANGULAR_SPEED;
    if(GetAsyncKeyState('Z')&0x8000)
        shell.fire(snake2);
    if(GetAsyncKeyState('G')&0x8000)
        mine.add(snake2,1);
}
int Interaction(Snake&snake1,Snake&snake2,Wall&wall,Shell&shell,Food&food,bool ai=false)
{
    int n;
    n=SnakeSnake(snake1,snake2);
    if(n==1)
        return 1;
    if(n==2)
        return 2;
    if(SnakeWall(snake1,wall)==1)
        return 1;
    if(SnakeWall(snake2,wall)==1)
        return 2;
    if(ShellSnake(shell,snake1,wall)==1)
        return 1;
    if(ShellSnake(shell,snake2,wall,ai)==1)
        return 2;
    ShellWall(shell,wall);
    ShellShell(shell);
    shell.move();
    return 0;
}
int TimeCtrl()
{
    int begin=GetTickCount();
    srand(begin);
    Snake snake1,snake2;
    Wall wall;
    Shell shell;
    Food food;
    Mine mine;
    int now;
    int n=0,m=0;
    while(1)
    {
        now=GetTickCount();
        if(now-begin>FREQUENCY)
        {
            n++;
            begin=now;
            KeyCtrl1(snake1,wall,shell,food,mine);
            KeyCtrl2(snake2,wall,shell,food,mine);
            if(n%3==0)
                food.create();
            Boundary(snake1,snake2,shell);
            Output(snake1,snake2,wall,shell,food,mine);
        }
        if(n>100) break;
    }
    m=0;
    while(1)
    {
        now=GetTickCount();
        if(now-begin>FREQUENCY)
        {
            begin=now;
            KeyCtrl1(snake1,wall,shell,food,mine);
            KeyCtrl2(snake2,wall,shell,food,mine);
            m++;
            if(m==FOOD_FREQUENCY)
            {   
                food.create();
                m=0;
            }
            n=Interaction(snake1,snake2,wall,shell,food);
            if(n==1) return 1;
            if(n==2) return 2;
            Boundary(snake1,snake2,shell);
            MineAction(mine,shell,now);
            Output(snake1,snake2,wall,shell,food,mine);
        }
    }
}
int TimeCtrl_AI()
{
    int begin=GetTickCount();
    srand(begin);
    Snake player,ai;
    Wall wall;
    Shell shell;
    Food food;
    Mine mine;
    Vision eye;
    MovingPattern move;
    int now;
    int n=0,m=0;
    while(1)
    {
        now=GetTickCount();
        if(now-begin>FREQUENCY)
        {
            n++;
            begin=now;
            KeyCtrl1(player,wall,shell,food,mine);
            move.Forward(ai,food);
            if(n%3==0)
                food.create();
            Boundary(player,ai,shell);
            Output(player,ai,wall,shell,food,mine);
        }
        if(n>100) break;
    }
    m=0;
    while(1)
    {
        now=GetTickCount();
        if(now-begin>FREQUENCY)
        {
            begin=now;
            KeyCtrl1(player,wall,shell,food,mine);
            move.AILogic(eye,ai,player,wall,shell,food,mine);
            m++;
            if(m==FOOD_FREQUENCY)
            {   
                food.create();
                m=0;
            }
            n=Interaction(player,ai,wall,shell,food,true);
            if(n==1) return 1;
            if(n==2) return 2;
            Boundary(player,ai,shell);
            MineAction(mine,shell,now);
            Output(player,ai,wall,shell,food,mine);
        }
    }
}
void Output(Snake&snake1,Snake&snake2,Wall&wall,Shell&shell,Food&food,Mine&mine)
{
    SnakeNode*pr1;
    ShellNode*pr2;
    FoodNode*pr3;
    MineNode*pr4;
    char data[(int)SIZE_Y+1][(int)SIZE_X+1];
    memset(data,0,((int)SIZE_X+1)*((int)SIZE_Y+1));
    int i;
    data[0][0]='X';
    data[(int)SIZE_Y][0]='X';
    data[0][(int)SIZE_X]='X';
    data[(int)SIZE_Y][(int)SIZE_X]='X';  
    data[(int)(snake1.Head->Y+0.2*LENGTH*sin(snake1.Head->Angle))][(int)(snake1.Head->X+LENGTH*cos(snake1.Head->Angle))]='0';
    data[(int)(snake2.Head->Y+0.2*LENGTH*sin(snake2.Head->Angle))][(int)(snake2.Head->X+LENGTH*cos(snake2.Head->Angle))]='0';
    for(pr1=snake1.Head;pr1!=NULL;pr1=pr1->next)
        data[(int)pr1->Y][(int)pr1->X]='1';
    for(pr1=snake2.Head;pr1!=NULL;pr1=pr1->next)
        data[(int)pr1->Y][(int)pr1->X]='2';
    for(pr1=wall.Head->next;pr1!=wall.Head;pr1=pr1->next)
        data[(int)pr1->Y][(int)pr1->X]='w';
    for(pr2=shell.Head->next;pr2!=shell.Head;pr2=pr2->next)
        data[(int)pr2->Y][(int)pr2->X]='!';
    for(pr3=food.Head->next;pr3!=food.Head;pr3=pr3->next)
        data[(int)pr3->Y][(int)pr3->X]='c';
    for(pr4=mine.Head->next;pr4!=mine.Head;pr4=pr4->next)
        if(pr4->Print==1) data[(int)pr4->Y][(int)pr4->X]='B';
    if(output_flag==0)
    {
        for(i=0;i<(int)SIZE_Y+1;i++)
        {
            coord.Y=i;
            WriteConsoleOutputCharacter(OutBuf,data[i],SIZE_X+1,coord,&bytes);
        }
        SetConsoleActiveScreenBuffer(OutBuf);
        output_flag=1;
        return;
    }
    else
    {
        for(i=0;i<(int)SIZE_Y+1;i++)
        {
            coord.Y=i;
            WriteConsoleOutputCharacter(OutPut,data[i],SIZE_X+1,coord,&bytes);
        }
        SetConsoleActiveScreenBuffer(OutPut);
        output_flag=0;
    }
}
int main()
{
    int flag;
    printf("WELCOME TO SNAKE TROUBLE\nset flash frequency:\n");
    scanf("%d",&FREQUENCY);
    printf("set SIZE_X:\n");
    scanf("%lf",&SIZE_X);
    printf("set SIZE_Y:\n");
    scanf("%lf",&SIZE_Y);
    printf("AI:\"1\",Human:\"0\":\n");
    scanf("%d",&flag);
    OutBuf=CreateConsoleScreenBuffer(GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
    OutPut=CreateConsoleScreenBuffer(GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
    CONSOLE_CURSOR_INFO inf;
    inf.bVisible=0;
    inf.dwSize=1;
    SetConsoleCursorInfo(OutPut,&inf);
    SetConsoleCursorInfo(OutBuf,&inf);
    if(flag==0)
        while(1)
        {
            int n=TimeCtrl();
            coord.X=0;
            coord.Y=0;
            output_flag=0;
            if(n==1) WriteConsoleOutputCharacter(OutPut,"2win",4,coord,&bytes);
            if(n==2) WriteConsoleOutputCharacter(OutPut,"1win",4,coord,&bytes);
            SetConsoleActiveScreenBuffer(OutPut);
            Sleep(4000);
        }
    else
    while(1)
        {
            int n=TimeCtrl_AI();
            coord.X=0;
            coord.Y=0;
            output_flag=0;
            if(n==1) WriteConsoleOutputCharacter(OutPut,"you've lost",11,coord,&bytes);
            if(n==2) WriteConsoleOutputCharacter(OutPut,"you win!",8,coord,&bytes);
            SetConsoleActiveScreenBuffer(OutPut);
            Sleep(4000);
        }
    return 0;
}