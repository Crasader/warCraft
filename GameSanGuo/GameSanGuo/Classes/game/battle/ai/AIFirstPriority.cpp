//
//  AIFirstPriority.cpp
//  GameSanGuo
//
//  Created by 赤霄 on 13-10-12.
//
//

#include "AIFirstPriority.h"
 
#include "SGNormalAttackList.h"

#define FOURSB 2 //四格武将
#define TWOSB 1 //二格武将
#define ONESB 0 //一格武将

#define SAME_COLOR 1 //相同颜色
#define DIFF_COLOR 0 //不同颜色
#define SPACE 2 //空位
#define NO_EXIST 3 //不存在,最后一排
#define IGNORE  4 //忽略
#define NO_EXIST_OR_SPACE 5//最后一排或者空格

#define OFFSETJ0 0
#define OFFSETJ1 -1
#define OFFSETJ2 -2
#define OFFSETJ3 3
#define OFFSETJ4 -4
#define OFFSETJN2 2
#define OFFSETJN1 1

#define OFFSETI1 1
#define OFFSETI0 0
#define OFFSETIN1 -1


#define DEL 0
#define MOVE 1
#define MOVETO 2

#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

SGGridBase *SBMOVEGRID= new SGGridBase;

 #pragma mark 数组
 int AIFirstPriority::del1Get2List[][ARRAY_ROW][ARRAR_COLOM]={
     {
         {FOURSB,DEL,2,0},//士兵类型， 武将右上向右的偏移量（），删除的坐标(相对武将左上的偏移量)
         {3,3,OFFSETI1,OFFSETJ1},
         {IGNORE,SAME_COLOR,SAME_COLOR},
         {SAME_COLOR,DIFF_COLOR,SAME_COLOR},
         {IGNORE,SAME_COLOR,IGNORE}
     },
     {
         {FOURSB,DEL,2,1},
         {3,3,OFFSETI1,OFFSETJ0},
         {SAME_COLOR,SAME_COLOR,IGNORE},
         {SAME_COLOR,DIFF_COLOR,SAME_COLOR},
         {IGNORE,SAME_COLOR,IGNORE}
         
     },
     {
         {FOURSB,DEL,1,1},
         {3,3,OFFSETI1,OFFSETJ0},
         {SAME_COLOR,DIFF_COLOR,SAME_COLOR},
         {SAME_COLOR,SAME_COLOR,IGNORE},
         {IGNORE,SAME_COLOR,IGNORE}
     },
     {
         {FOURSB,DEL,1,0},
         {3,3,OFFSETI1,OFFSETJ1},
         {SAME_COLOR,DIFF_COLOR,SAME_COLOR},
         {IGNORE,SAME_COLOR,SAME_COLOR},
         {IGNORE,SAME_COLOR,IGNORE}
     },
    {
        {TWOSB,DEL,2,0},
        {3,3,OFFSETI1,OFFSETJ2},
        {IGNORE,IGNORE,SAME_COLOR},//从下到上，从右到左
        {SAME_COLOR,SAME_COLOR,DIFF_COLOR},
        {IGNORE,IGNORE,SAME_COLOR}
    },
    {
        {TWOSB,DEL,2,0},
        {3,3,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,IGNORE,IGNORE},
        {DIFF_COLOR,SAME_COLOR,SAME_COLOR},
        {SAME_COLOR,IGNORE,IGNORE}
    },
    {
        {TWOSB,DEL,2,0},
        {3,3,OFFSETI1,OFFSETJ1},
        {IGNORE,SAME_COLOR,IGNORE},
        {SAME_COLOR,DIFF_COLOR,SAME_COLOR},
        {IGNORE,SAME_COLOR,IGNORE}
        
    },
    {
        {TWOSB,DEL,1,0},
        {3,3,OFFSETI1,OFFSETJ2},
        {SAME_COLOR,SAME_COLOR,DIFF_COLOR},
        {IGNORE,IGNORE,SAME_COLOR},
        {IGNORE,IGNORE,SAME_COLOR}
    },
    {
        {TWOSB,DEL,1,0},
        {3,3,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR,SAME_COLOR},
        {SAME_COLOR,IGNORE,IGNORE,},
        {SAME_COLOR,IGNORE,IGNORE}
    },
    {
        {TWOSB,DEL,1,0},
        {3,3,OFFSETI1,OFFSETJ1},
        {SAME_COLOR,DIFF_COLOR,SAME_COLOR},
        {IGNORE,SAME_COLOR,IGNORE},
        {IGNORE,SAME_COLOR,IGNORE}
    },
    
     {
         {ONESB,DEL,2,0},//0,1,2偏移量向右的,如果是三就是同行,删除的偏移量
         {3,3,OFFSETI1,OFFSETJ2},
         {IGNORE,IGNORE,SAME_COLOR},
         {SAME_COLOR,SAME_COLOR,DIFF_COLOR},
         {IGNORE,IGNORE,SAME_COLOR}
     },
     {
         {ONESB,DEL,1,0},
         {3,3,OFFSETI1,OFFSETJ1},
         {SAME_COLOR,DIFF_COLOR,SAME_COLOR},
         {IGNORE,SAME_COLOR,IGNORE},
         {IGNORE,SAME_COLOR,IGNORE}
     },
     {
         {ONESB,DEL,2,0},
         {3,3,OFFSETI1,OFFSETJ0},
         {SAME_COLOR,IGNORE,IGNORE},
         {DIFF_COLOR,SAME_COLOR,SAME_COLOR},
         {SAME_COLOR,IGNORE,IGNORE}
     },
     {
         {ONESB,DEL,2,0},
         {3,3,OFFSETI1,OFFSETJ1},
         {IGNORE,SAME_COLOR,IGNORE},
         {SAME_COLOR,DIFF_COLOR,SAME_COLOR},
         {IGNORE,SAME_COLOR,IGNORE}
     },
     {
         {ONESB,DEL,1,0},
         {3,3,OFFSETI1,OFFSETJ0},
         {DIFF_COLOR,SAME_COLOR,SAME_COLOR},
         {SAME_COLOR,IGNORE,IGNORE},
         {SAME_COLOR,IGNORE,IGNORE}
     },
     {
         {ONESB,DEL,1,0},
         {3,3,OFFSETI1,OFFSETJ2},
         {SAME_COLOR,SAME_COLOR,DIFF_COLOR},
         {IGNORE,IGNORE,SAME_COLOR},
         {IGNORE,IGNORE,SAME_COLOR},
     },
     {
         {ONESB,DEL,0,-2},
         {2,4,OFFSETI0,OFFSETJ4},
         {SAME_COLOR,SAME_COLOR,DIFF_COLOR,SAME_COLOR},
         {IGNORE,IGNORE,SAME_COLOR,IGNORE}
     }

};
int AIFirstPriority::del1Get1List[][ARRAY_ROW][ARRAR_COLOM]={

    {
        {FOURSB,DEL,1,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {SAME_COLOR,SAME_COLOR},
        {IGNORE,SAME_COLOR}
    },
    {
        {FOURSB,DEL,1,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {SAME_COLOR,SAME_COLOR},
        {SAME_COLOR,IGNORE}
    },
    {
        {FOURSB,DEL,2,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {DIFF_COLOR,SAME_COLOR},
        {SAME_COLOR,IGNORE}
    },
    {
        {FOURSB,DEL,2,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {SAME_COLOR,DIFF_COLOR},
        {IGNORE,SAME_COLOR},
    },
    {
        {TWOSB,DEL,1,0},
        {3,1,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR},
        {SAME_COLOR},
        {SAME_COLOR}
    },
    {
        {TWOSB,DEL,2,0},
        {3,1,OFFSETI1,OFFSETJ0},
        {SAME_COLOR},
        {DIFF_COLOR},
        {SAME_COLOR}
    },
    {
        {ONESB,DEL,1,0},
        {3,1,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR},
        {SAME_COLOR},
        {SAME_COLOR}
    },
    {
        {ONESB,DEL,2,0},
        {3,1,OFFSETI1,OFFSETJ0},
        {SAME_COLOR},
        {DIFF_COLOR},
        {SAME_COLOR}
    },
    {
        {ONESB,DEL,-1,0},
        {1,3,OFFSETIN1,OFFSETJ2},
        {SAME_COLOR,SAME_COLOR,DIFF_COLOR}
      
    },
    {
        {ONESB,DEL,0,-1},
        {2,2,OFFSETI0,OFFSETJ2},
        {SAME_COLOR,DIFF_COLOR},
        {IGNORE,SAME_COLOR}
    },
    {
        {ONESB,DEL,0,-2},
        {2,2,OFFSETI0,OFFSETJ2},
        {DIFF_COLOR,SAME_COLOR},
        {SAME_COLOR,IGNORE}
    } 

};

int AIFirstPriority::move1Get2List[][ARRAY_ROW][ARRAR_COLOM]={

    {
        {FOURSB,MOVE,2,1},
        {3,3,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR,IGNORE},
        {SAME_COLOR,SPACE,SAME_COLOR},
        {IGNORE,NO_EXIST_OR_SPACE,IGNORE}
    },
    {
        {FOURSB,MOVE,2,0},
        {3,3,OFFSETI1,OFFSETJ1},
        {IGNORE,SAME_COLOR,SAME_COLOR},
        {SAME_COLOR,SPACE,SAME_COLOR},
        {IGNORE,NO_EXIST_OR_SPACE,IGNORE}
    },
    {
        {TWOSB,MOVE,2,0},
        {3,3,OFFSETI1,OFFSETJ1},
        {IGNORE,SAME_COLOR,IGNORE},
        {SAME_COLOR,SPACE,SAME_COLOR},
        {IGNORE,NO_EXIST_OR_SPACE,IGNORE}
    },
    {
        {TWOSB,MOVE,2,0},
        {3,3,OFFSETI1,OFFSETJ2},
        {IGNORE,IGNORE,SAME_COLOR},
        {SAME_COLOR,SAME_COLOR,SPACE},
        {IGNORE,IGNORE,NO_EXIST_OR_SPACE}
    },
    {
        {TWOSB,MOVE,2,0},
        {3,3,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,IGNORE,IGNORE},
        {SPACE,SAME_COLOR,SAME_COLOR},
        {NO_EXIST_OR_SPACE,IGNORE,IGNORE},
    }
};
int AIFirstPriority::move1Get2F1List[][ARRAY_ROW][ARRAR_COLOM]={
    //
    {
        {ONESB,MOVE,2,0},
        {3,3,OFFSETI1,OFFSETJ1},
        {IGNORE,SAME_COLOR,IGNORE},
        {SAME_COLOR,SPACE,SAME_COLOR},
        {IGNORE,NO_EXIST_OR_SPACE,IGNORE}
      
    },
    {
        {ONESB,MOVE,2,0},
        {3,3,OFFSETI1,OFFSETJ2},
        {IGNORE,IGNORE,SAME_COLOR},
        {SAME_COLOR,SAME_COLOR,SPACE},
        {IGNORE,IGNORE,NO_EXIST_OR_SPACE}
    },
    {
        {ONESB,MOVE,2,0},
        {3,3,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,IGNORE,IGNORE},
        {SPACE,SAME_COLOR,SAME_COLOR},
        {NO_EXIST_OR_SPACE,IGNORE,IGNORE}
    },
    {
        {ONESB,MOVE,0,-2},
        {3,4,OFFSETI0,OFFSETJ4},
        {SAME_COLOR,SAME_COLOR,SPACE,SAME_COLOR},
        {IGNORE,IGNORE,NO_EXIST_OR_SPACE,IGNORE},
        {IGNORE,IGNORE,NO_EXIST_OR_SPACE,IGNORE}
        
    }

};

int AIFirstPriority::n1C1F4[][ARRAY_ROW][ARRAR_COLOM]={
    {
        {FOURSB,MOVE,2,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {SPACE,SAME_COLOR},
        {NO_EXIST_OR_SPACE,IGNORE}
    },
    {
        {FOURSB,MOVE,2,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {SAME_COLOR,SPACE},
        {IGNORE,NO_EXIST_OR_SPACE}
    }

};
int AIFirstPriority::n2C1F4D[][ARRAY_ROW][ARRAR_COLOM]={
    {
        {FOURSB,DEL,1,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {SAME_COLOR,DIFF_COLOR},
        {SAME_COLOR,SAME_COLOR}
    },
    {
        {FOURSB,DEL,2,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {DIFF_COLOR,SAME_COLOR},
        {SAME_COLOR,SAME_COLOR}
    },
    {
        {FOURSB,DEL,2,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {DIFF_COLOR,DIFF_COLOR},
        {SAME_COLOR,SAME_COLOR}
    },
    {
        {FOURSB,DEL,1,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,DIFF_COLOR},
        {SAME_COLOR,SAME_COLOR},
        {SAME_COLOR,SAME_COLOR}
    },
    {
        {FOURSB,DEL,1,1},
        {4,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {SAME_COLOR,DIFF_COLOR},
        {IGNORE,SAME_COLOR},
        {IGNORE,SAME_COLOR}
    },
    {
        {FOURSB,DEL,1,1},
        {4,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {SAME_COLOR,SAME_COLOR},
        {IGNORE,DIFF_COLOR},
        {IGNORE,SAME_COLOR}
    },
    {
        {FOURSB,DEL,2,1},
        {4,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {SAME_COLOR,DIFF_COLOR},
        {IGNORE,DIFF_COLOR},
        {IGNORE,SAME_COLOR}
    },
    {
        {FOURSB,DEL,1,0},
        {4,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {DIFF_COLOR,SAME_COLOR},
        {SAME_COLOR,IGNORE},
        {SAME_COLOR,IGNORE},
        
    },
    
    {
        {FOURSB,DEL,1,0},
        {4,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {SAME_COLOR,SAME_COLOR},
        {DIFF_COLOR,IGNORE},
        {SAME_COLOR,IGNORE},
        
    },
    
    {
        {FOURSB,DEL,2,0},
        {4,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {DIFF_COLOR,SAME_COLOR},
        {DIFF_COLOR,IGNORE},
        {SAME_COLOR,IGNORE},
        
    }
    
};
int AIFirstPriority::n2C1F4M[][ARRAY_ROW][ARRAR_COLOM]={
    
    
    {
        {FOURSB,MOVE,3,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {SAME_COLOR,SAME_COLOR},
        {IGNORE,SPACE}
    },
    {
        {FOURSB,MOVE,3,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {SAME_COLOR,SAME_COLOR},
        {SPACE,IGNORE}
         
    },
    {
        {FOURSB,MOVE,3,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {DIFF_COLOR,SAME_COLOR},
        {SPACE,IGNORE}
    },
    {
        {FOURSB,MOVE,3,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {SAME_COLOR,DIFF_COLOR},
        {IGNORE,SPACE}
        
    },
    {
        {FOURSB,MOVE,2,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {SPACE,SAME_COLOR},
        {SPACE,SAME_COLOR}
    },
    {
        {FOURSB,MOVE,2,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {SAME_COLOR,SPACE},
        {SAME_COLOR,SPACE}
    },
    {
        {FOURSB,MOVE,2,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {DIFF_COLOR,SPACE},
        {SAME_COLOR,SPACE}
    },
    {
        {FOURSB,MOVE,2,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {SPACE,DIFF_COLOR},
        {SPACE,SAME_COLOR}
    }
    
};

int AIFirstPriority::n2C2F4M[][ARRAY_ROW][ARRAR_COLOM]={
    {
        {FOURSB,MOVE,1,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SPACE,SAME_COLOR},
        {SPACE,SAME_COLOR},
        {NO_EXIST_OR_SPACE,IGNORE}
    },
    {
        {FOURSB,MOVE,1,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SPACE},
        {SAME_COLOR,SPACE},
        {IGNORE,NO_EXIST_OR_SPACE}
    },
    {
        {FOURSB,MOVE,2,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {SPACE,SPACE},
        {NO_EXIST_OR_SPACE,NO_EXIST_OR_SPACE}
    } ,
    {
        {FOURSB,MOVETO,2,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {DIFF_COLOR,SAME_COLOR},
        {NO_EXIST,NO_EXIST}
    },
    {
        {FOURSB,MOVETO,2,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {SAME_COLOR,DIFF_COLOR},
        {NO_EXIST,NO_EXIST}
    },
    {
        {FOURSB,DEL,1,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {SAME_COLOR,SAME_COLOR},
        {NO_EXIST,NO_EXIST}
    }
    ,
    {
        {FOURSB,DEL,1,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {SAME_COLOR,SAME_COLOR},
        {NO_EXIST,NO_EXIST}
    }
     
    
};
 
int AIFirstPriority::n3C2F4D[][ARRAY_ROW][ARRAR_COLOM]={
    {
        {FOURSB,DEL,1,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {SAME_COLOR,DIFF_COLOR},
        {SAME_COLOR,IGNORE}
        
    },
    {
        {FOURSB,DEL,1,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {DIFF_COLOR,SAME_COLOR},
        {IGNORE,SAME_COLOR}
        
    },
    {
        {FOURSB,DEL,1,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,DIFF_COLOR},
        {SAME_COLOR,SAME_COLOR},
        {IGNORE,SAME_COLOR}
        
    },
    {
        {FOURSB,DEL,1,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,DIFF_COLOR},
        {SAME_COLOR,SAME_COLOR},
        {SAME_COLOR,IGNORE}
        
    },
    {
        {FOURSB,DEL,2,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {DIFF_COLOR,DIFF_COLOR},
        {IGNORE,SAME_COLOR}
    },
    {
        {FOURSB,DEL,2,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {DIFF_COLOR,DIFF_COLOR},
        {SAME_COLOR,IGNORE}
    },
    {
        {FOURSB,DEL,2,1},
        {4,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {SAME_COLOR,DIFF_COLOR},
        {IGNORE,DIFF_COLOR},
        {IGNORE,IGNORE}
    },
    {
        {FOURSB,DEL,2,0},
        {4,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {DIFF_COLOR,SAME_COLOR},
        {DIFF_COLOR,IGNORE},
        {IGNORE,IGNORE}
    },
    {
        {FOURSB,DEL,1,0},
        {4,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {SAME_COLOR,SAME_COLOR},
        {DIFF_COLOR,IGNORE},
        {IGNORE,IGNORE}
    },
    {
        {FOURSB,DEL,1,1},
        {4,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {SAME_COLOR,SAME_COLOR},
        {IGNORE,DIFF_COLOR},
        {IGNORE,IGNORE}
    },
    {
        {FOURSB,DEL,1,1},
        {4,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {SAME_COLOR,DIFF_COLOR},
        {IGNORE,SAME_COLOR},
        {IGNORE,IGNORE}
    },
    {
        {FOURSB,DEL,1,0},
        {4,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {DIFF_COLOR,SAME_COLOR},
        {SAME_COLOR,IGNORE},
        {IGNORE,IGNORE}
    },
    {
        {FOURSB,DEL,1,1},
        {4,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {SPACE,DIFF_COLOR},
        {SPACE,SAME_COLOR},
        {IGNORE,SAME_COLOR}
    },
    {
        {FOURSB,DEL,1,0},
        {4,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {DIFF_COLOR,SPACE},
        {SAME_COLOR,SPACE},
        {SAME_COLOR,IGNORE}
    },
    {
        {FOURSB,DEL,2,1},
        {4,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {SPACE,DIFF_COLOR},
        {SPACE,DIFF_COLOR},
        {IGNORE,SAME_COLOR}
    },
    {
        {FOURSB,DEL,2,0},
        {4,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {DIFF_COLOR,SPACE},
        {DIFF_COLOR,SPACE},
        {SAME_COLOR,IGNORE}
    },
    {
        {FOURSB,DEL,1,0},
        {4,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {SAME_COLOR,SPACE},
        {DIFF_COLOR,SPACE},
        {SAME_COLOR,IGNORE}
    },
    {
        {FOURSB,DEL,1,1},
        {4,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {SPACE,SAME_COLOR},
        {SPACE,DIFF_COLOR},
        {IGNORE,SAME_COLOR}
    }
    
};

int AIFirstPriority::n3C2F4M[][ARRAY_ROW][ARRAR_COLOM]={

    {
        {FOURSB,MOVE,2,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {SAME_COLOR,SPACE},
        {IGNORE,SPACE}
    },
    {
        {FOURSB,MOVE,2,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {SPACE,SAME_COLOR},
        {SPACE,IGNORE},
    },
    {
        {FOURSB,MOVE,2,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {DIFF_COLOR,SPACE},
        {SPACE,SPACE}
    },
    {
        {FOURSB,MOVE,2,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {SPACE,DIFF_COLOR},
        {SPACE,SPACE}
    },
    {
        {FOURSB,MOVE,2,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {SAME_COLOR,SPACE},
        {SPACE,SPACE}
    },
    {
        {FOURSB,MOVE,2,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {SPACE,SAME_COLOR},
        {SPACE,SPACE}
    },
    {
        {FOURSB,MOVE,1,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SPACE,DIFF_COLOR},
        {SPACE,SAME_COLOR},
        {SPACE,SAME_COLOR}
    },
    {
        {FOURSB,MOVE,1,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SPACE},
        {SAME_COLOR,SPACE},
        {SAME_COLOR,SPACE}
    }

};
int AIFirstPriority::n3C3F4M[][ARRAY_ROW][ARRAR_COLOM]={
    {
        {FOURSB,MOVE,2,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SPACE,SAME_COLOR},
        {SPACE,SPACE},
        {NO_EXIST_OR_SPACE,NO_EXIST_OR_SPACE}
    },
    {
        {FOURSB,MOVE,2,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SPACE},
        {SPACE,SPACE},
        {NO_EXIST_OR_SPACE,NO_EXIST_OR_SPACE}
    },
    {
        {FOURSB,MOVE,2,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {SPACE,SAME_COLOR},
        {NO_EXIST,NO_EXIST}
    },
    {
        {FOURSB,MOVE,2,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {SAME_COLOR,SPACE},
        {NO_EXIST,NO_EXIST}
    },
    {
        {FOURSB,MOVE,2,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {SPACE,SAME_COLOR},
        {NO_EXIST,NO_EXIST}
    },
    {
        {FOURSB,MOVETO,1,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {SAME_COLOR,SPACE},
        {NO_EXIST,NO_EXIST}
    } ,
    {
        {FOURSB,MOVETO,1,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {SPACE,SAME_COLOR},
        {NO_EXIST,NO_EXIST}
    } ,
    {
        {FOURSB,DEL,1,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {SAME_COLOR,SPACE},
        {NO_EXIST,NO_EXIST}
    } ,
    {
        {FOURSB,DEL,1,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {SPACE,SAME_COLOR},
        {NO_EXIST,NO_EXIST}
    },
    {
        {FOURSB,DEL,-2,0},
        {2,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {NO_EXIST,NO_EXIST}
    },//这两个是n3c3M
    {
        {FOURSB,DEL,-2,1},
        {2,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {NO_EXIST,NO_EXIST}
    },

};
int AIFirstPriority::n4C3F4M[][ARRAY_ROW][ARRAR_COLOM]={
    {
        {FOURSB,MOVE,1,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SPACE},
        {SPACE,SPACE},
        {SPACE,SPACE}
    },
    {
        {FOURSB,MOVE,1,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SPACE,DIFF_COLOR},
        {SPACE,SPACE},
        {SPACE,SPACE}
    },//需5耗4
    {
        {FOURSB,MOVE,1,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SPACE},
        {SAME_COLOR,SPACE},
        {SPACE,SPACE}
    },
    {
        {FOURSB,MOVE,1,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SPACE,DIFF_COLOR},
        {SPACE,SAME_COLOR},
        {SPACE,SPACE}
    },
    {
        {FOURSB,MOVE,1,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SPACE,SAME_COLOR},
        {SPACE,DIFF_COLOR},
        {SPACE,SPACE}
    },
    {
        {FOURSB,MOVE,1,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SPACE},
        {DIFF_COLOR,SPACE},
        {SPACE,SPACE}
    },
    
    {
        {FOURSB,MOVE,2,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {SPACE,SPACE},
        {SPACE,SPACE}
    },
    {
        {FOURSB,MOVE,2,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {SPACE,SPACE},
        {SPACE,SPACE}
    },
    {
        {FOURSB,DEL,1,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {DIFF_COLOR,SPACE},
        {SAME_COLOR,SPACE}
    },
    {
        {FOURSB,DEL,1,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {SPACE,DIFF_COLOR},
        {SPACE,SAME_COLOR}
    },
    {
        {FOURSB,DEL,2,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {SPACE,DIFF_COLOR},
        {SPACE,SAME_COLOR}
    },
    {
        {FOURSB,DEL,2,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {DIFF_COLOR,SPACE},
        {SAME_COLOR,SPACE}
    },
    {
        {FOURSB,DEL,2,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {DIFF_COLOR,SPACE},
        {DIFF_COLOR,SPACE}
    },
    {
        {FOURSB,DEL,2,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SAME_COLOR},
        {SPACE,DIFF_COLOR},
        {SPACE,DIFF_COLOR}
    },
    {
        {FOURSB,DEL,1,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {SAME_COLOR,DIFF_COLOR},
        {IGNORE,SPACE}
    },
    {
        {FOURSB,DEL,1,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {DIFF_COLOR,SAME_COLOR},
        {SPACE,IGNORE}
    },
    {
        {FOURSB,DEL,1,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,DIFF_COLOR},
        {DIFF_COLOR,SAME_COLOR},
        {SAME_COLOR,SAME_COLOR}
    },
    {
        {FOURSB,DEL,1,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,DIFF_COLOR},
        {SAME_COLOR,DIFF_COLOR},
        {SAME_COLOR,SAME_COLOR}
    }
    
    
};
int AIFirstPriority::n4C4F4M[][ARRAY_ROW][ARRAR_COLOM]={
    {
        {FOURSB,MOVE,1,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SPACE,SPACE},
        {SPACE,SPACE},
        {NO_EXIST_OR_SPACE,NO_EXIST_OR_SPACE}
    },
    {
        {FOURSB,MOVETO,1,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {SPACE,SPACE},
        {NO_EXIST,NO_EXIST}
         
    },
    {
        {FOURSB,MOVETO,1,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {SPACE,SPACE},
        {NO_EXIST,NO_EXIST}
        
    },
    {
        {FOURSB,MOVE,2,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {SPACE,SPACE},
        {NO_EXIST,NO_EXIST}
        
    },
    {
        {FOURSB,MOVE,2,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,DIFF_COLOR},
        {SPACE,SPACE},
        {NO_EXIST,NO_EXIST}
        
    },
    {
        {FOURSB,MOVE,2,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SPACE},
        {DIFF_COLOR,SPACE},
        {NO_EXIST,NO_EXIST}
        
    },
    {
        {FOURSB,MOVE,1,0},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SPACE},
        {SAME_COLOR,SPACE},
        {NO_EXIST,NO_EXIST}
        
    },
    {
        {FOURSB,DEL,1,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SPACE,DIFF_COLOR},
        {SPACE,SAME_COLOR},
        {NO_EXIST,NO_EXIST}
        
    },
    {
        {FOURSB,DEL,2,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {SPACE,SAME_COLOR},
        {SPACE,DIFF_COLOR},
        {NO_EXIST,NO_EXIST}
        
    },
    {
        {FOURSB,DEL,1,1},
        {3,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,DIFF_COLOR},
        {SAME_COLOR,SAME_COLOR},
        {NO_EXIST,NO_EXIST}
        
    },//加几个其他的
    {
        {FOURSB,MOVE,1,0},
        {2,2,OFFSETI1,OFFSETJ0},
        {SPACE,SAME_COLOR},
        {NO_EXIST,NO_EXIST}
    },
    {
        {FOURSB,MOVE,1,1},
        {2,2,OFFSETI1,OFFSETJ0},
        {SAME_COLOR,SPACE},
        {NO_EXIST,NO_EXIST}
    },
    {
        {FOURSB,MOVE,1,0},
        {2,2,OFFSETI1,OFFSETJ0},
        {SPACE,SPACE},
        {NO_EXIST,NO_EXIST}
    },
    {
        {FOURSB,DEL,1,1},
        {2,2,OFFSETI1,OFFSETJ0},
        {SPACE,DIFF_COLOR},
        {NO_EXIST,NO_EXIST}
    },
    {
        {FOURSB,DEL,1,0},
        {2,2,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR,SAME_COLOR},
        {NO_EXIST,NO_EXIST}
    }
};
int AIFirstPriority::n1C1F2[][ARRAY_ROW][ARRAR_COLOM]={

    {
        {TWOSB,MOVE,2,0},
        {3,1,OFFSETI1,OFFSETJ0},
        {SAME_COLOR},
        {SPACE},
        {NO_EXIST_OR_SPACE}
    }
};

int AIFirstPriority::n2C1F2D[][ARRAY_ROW][ARRAR_COLOM]={
    {
        {TWOSB,DEL,1,0},
        {4,1,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR},
        {SAME_COLOR},
        {DIFF_COLOR},
        {SAME_COLOR}
    },{
        {TWOSB,DEL,1,0},
        {4,1,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR},
        {DIFF_COLOR},
        {SAME_COLOR},
        {SAME_COLOR}
    },
    {
        {TWOSB,DEL,2,0},
        {4,1,OFFSETI1,OFFSETJ0},
        {SAME_COLOR},
        {DIFF_COLOR},
        {DIFF_COLOR},
        {SAME_COLOR}
    }
    
};

int AIFirstPriority::n2C1F2M[][ARRAY_ROW][ARRAR_COLOM]={
    {
        {TWOSB,MOVE,3,0},
        {3,1,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR},
        {SAME_COLOR},
        {SPACE}
    },{
        {TWOSB,MOVE,3,0},
        {3,1,OFFSETI1,OFFSETJ0},
        {SAME_COLOR},
        {DIFF_COLOR},
        {SPACE}
    }
};

int AIFirstPriority::n2C2F2D[][ARRAY_ROW][ARRAR_COLOM]={
    {
        {TWOSB,DEL,-2,0},
        {2,1,OFFSETI1,OFFSETJ0},
        {SAME_COLOR},
        {NO_EXIST}
    },
    {
        {TWOSB,DEL,1,0},
        {2,1,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR},
        {SAME_COLOR},
        {NO_EXIST}
    },
    {
        {TWOSB,MOVETO,2,0},
        {3,1,OFFSETI1,OFFSETJ0},
        {SAME_COLOR},
        {DIFF_COLOR},
        {NO_EXIST}
    }
};
int AIFirstPriority::n2C2F2M[][ARRAY_ROW][ARRAR_COLOM]={
    {
        {TWOSB,MOVE,1,0},
        {3,1,OFFSETI1,OFFSETJ0},
        {SPACE},
        {SPACE},
        {NO_EXIST_OR_SPACE}
    }

};

int AIFirstPriority::n3C2F2D[][ARRAY_ROW][ARRAR_COLOM]={
    {
        {TWOSB,DEL,1,0},
        {4,1,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR},
        {DIFF_COLOR},
        {SAME_COLOR},
        {NO_EXIST_OR_SPACE}
    },{
        {TWOSB,DEL,1,0},
        {4,1,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR},
        {SAME_COLOR},
        {DIFF_COLOR},
        {NO_EXIST_OR_SPACE}
    },
    {
        {TWOSB,DEL,1,0},
        {4,1,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR},
        {SAME_COLOR},
        {DIFF_COLOR},
        {NO_EXIST_OR_SPACE}
    }
    
};
int AIFirstPriority::n3C2F2M[][ARRAY_ROW][ARRAR_COLOM]={
    {
        {TWOSB,MOVE,2,0},
        {3,1,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR},
        {SPACE},
        {SPACE}
    }
    
};
int AIFirstPriority::n3C3F2M[][ARRAY_ROW][ARRAR_COLOM]={
    {
        {TWOSB,MOVETO,1,0},
        {3,1,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR},
        {SPACE},
        {NO_EXIST}
    },
    {
        {TWOSB,MOVETO,0,0},
        {1,1,OFFSETI0,OFFSETJ0},
        {NO_EXIST}
    }
//    {
//        {TWOSB,MOVE,1,0},
//        {2,1,OFFSETI1,OFFSETJ0},
//        {SPACE},
//        {NO_EXIST}
//    }
   };
int AIFirstPriority::n3C3F2D[][ARRAY_ROW][ARRAR_COLOM]={
    {
        {TWOSB,DEL,-2,0},
        {2,1,OFFSETI1,OFFSETJ0},
        {SPACE},
        {NO_EXIST}
    }
};

int AIFirstPriority::n1C1F1M[][ARRAY_ROW][ARRAR_COLOM]={
    {
        {ONESB,MOVE,2,0},
        {4,1,OFFSETI1,OFFSETJ0},
        {SAME_COLOR},
        {SPACE},
        {NO_EXIST_OR_SPACE},
        {NO_EXIST_OR_SPACE},
    },
    {
        {ONESB,MOVE,0,-2},
        {3,2,OFFSETI0,OFFSETJ2},
        {SPACE,SAME_COLOR},
        {SPACE,IGNORE},
        {NO_EXIST_OR_SPACE,IGNORE},
    } ,
    {
        {ONESB,MOVE,0,2},
        {3,2,OFFSETI0,OFFSETJN1},
        {SAME_COLOR,SPACE},
        {IGNORE,SPACE},
        {IGNORE,NO_EXIST_OR_SPACE},
    }
    ,
    {
        {ONESB,MOVE,0,1},
        {3,2,OFFSETI0,OFFSETJN1},
        {SPACE,SAME_COLOR},
        {SPACE,IGNORE},
        {IGNORE,NO_EXIST_OR_SPACE},
    }
   

};

int AIFirstPriority::makeFastF1[][ARRAY_ROW][ARRAR_COLOM]={
    {
        {ONESB,MOVE,3,0},
        {3,1,OFFSETI1,OFFSETJ0},
        {SAME_COLOR},
        {DIFF_COLOR},
        {SPACE}
    },
    {
        {ONESB,MOVE,3,0},
        {3,1,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR},
        {SAME_COLOR},
        {SPACE}
    }
    ,
    {
        {ONESB,DEL,2,0},
        {3,1,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR},
        {DIFF_COLOR},
        {SAME_COLOR}
    }
    ,
    {
        {ONESB,DEL,2,0},
        {4,1,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR},
        {DIFF_COLOR},
        {SAME_COLOR},
        {SAME_COLOR}
    },
    {
        {ONESB,DEL,2,0},
        {4,1,OFFSETI1,OFFSETJ0},
        {SAME_COLOR},
        {DIFF_COLOR},
        {DIFF_COLOR},
        {SAME_COLOR}
    },
    {
        {ONESB,DEL,2,0},
        {4,1,OFFSETI1,OFFSETJ0},
        {SAME_COLOR},
        {DIFF_COLOR},
        {DIFF_COLOR},
        {NO_EXIST_OR_SPACE}
    },
    {
        {ONESB,MOVE,3,0},
        {4,1,OFFSETI1,OFFSETJ0},
        {SAME_COLOR},
        {DIFF_COLOR},
        {SPACE},
        {NO_EXIST_OR_SPACE}
    } ,
    {
        {ONESB,DEL,2,0},
        {4,1,OFFSETI1,OFFSETJ0},
        {SAME_COLOR},
        {DIFF_COLOR},
        {NO_EXIST_OR_SPACE},
        {NO_EXIST_OR_SPACE}
    } ,
    {
        {ONESB,MOVE,3,0},
        {4,1,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR},
        {SAME_COLOR},
        {SPACE},
        {NO_EXIST_OR_SPACE}
    },
    {
        {ONESB,DEL,1,0},
        {2,1,OFFSETI1,OFFSETJ0},
        {DIFF_COLOR},
        {SAME_COLOR}
    },
    {
        {ONESB,DEL,-1,0},
        {2,1,OFFSETI1,OFFSETJ0},
        {SAME_COLOR},
        {NO_EXIST}
    }
    

};

//扫描全部
#pragma mark ---扫描全部

 

#pragma mark -- 优先级选择
ReturnIndex AIFirstPriority::del1Get2(SGBattleMap *bm)
{
    int len=0;
    GET_ARRAY_LEN(del1Get2List, len)
    return AIFirstPriority::checkAllWithList(bm, del1Get2List,len);
}


ReturnIndex AIFirstPriority::del1Get1(SGBattleMap *bm)
{
    int len=0;
    GET_ARRAY_LEN(del1Get1List, len)
    CCLog("$扫描del1Get1List");
    return AIFirstPriority::checkAllWithList(bm, del1Get1List,len);
}

ReturnIndex AIFirstPriority::move1Get2(SGBattleMap *bm)
{
    int len=0;
    GET_ARRAY_LEN(move1Get2List, len)
    ReturnIndex rt_index;
    
      rt_index=AIFirstPriority::checkAllWithList(bm, move1Get2List,len);
    if (rt_index.type==rt_null) {
        GET_ARRAY_LEN(move1Get2F1List, len)
        CCLog("$扫描move1Get2F1List");
         
        return rt_index=AIFirstPriority::checkWithSBTypeAndList(bm, ksbone, move1Get2F1List,len);
    }
    //CCLog("扫描move1Get2List成功");
    return rt_index;
}

ReturnIndex AIFirstPriority::round1GetFour(SGBattleMap *bm){
    ReturnIndex rt_index;
    rt_index.type=rt_null;
    rt_index=AIFirstPriority::need1GetFour(bm);
    if (rt_index.type==rt_null) {
        rt_index=AIFirstPriority::need2GetFour(bm);
        if (rt_index.type==rt_null) {
            return rt_index=AIFirstPriority::need3GetFour(bm);
        }
    }
    
    return rt_index;
    
}
ReturnIndex AIFirstPriority::need1GetFour(SGBattleMap *bm)
{
    int len=0;
    GET_ARRAY_LEN(n1C1F4, len)
    ReturnIndex rt_index;
    rt_index.needAction=1;
    sgShiBingType sbType;
    sbType=ksbfour;
    rt_index=AIFirstPriority::checkWithSBTypeAndList(bm, sbType, n1C1F4,len);
    if (rt_index.type!=rt_null) {
          CCLog("$扫描n1C1F4成功");
        rt_index.needAction=1;
        return rt_index;
    }
  
    return rt_index;
}
ReturnIndex AIFirstPriority::need2GetFour(SGBattleMap *bm)
{
    ReturnIndex rt_index;
    rt_index.type=rt_null;
    rt_index.needAction=2;
    int len=0;
    GET_ARRAY_LEN(n2C1F4D, len)
    sgShiBingType sbType;
    sbType=ksbfour;
    rt_index= AIFirstPriority::checkWithSBTypeAndList(bm, sbType, n2C1F4D,len);
    if (rt_index.type!=rt_null)
    {
        rt_index.needAction=2;
        CCLog("$扫描n2C1F4D成功");
        return rt_index;
    }
    GET_ARRAY_LEN(n2C1F4M, len)
    rt_index= AIFirstPriority::checkWithSBTypeAndList(bm, sbType, n2C1F4M,len);
    if (rt_index.type!=rt_null)
    {
        rt_index.needAction=2;
        CCLog("$扫描n2C1F4M成功");
        return rt_index;
    }
    
        
    GET_ARRAY_LEN(n2C2F4M, len)
    rt_index= AIFirstPriority::checkWithSBTypeAndList(bm, sbType, n2C2F4M,len);
    if (rt_index.type!=rt_null) {
        CCLog("$扫描 n2C2F4M");
        rt_index.needAction=2;
        return rt_index;
    }
    
    return  rt_index;
    
    
}
ReturnIndex AIFirstPriority::need3GetFour(SGBattleMap *bm)
{
    ReturnIndex rt_index;
    rt_index.type=rt_null;
    rt_index.needAction=3;
    int len=0;

    GET_ARRAY_LEN(n3C2F4D, len)
    rt_index= AIFirstPriority::checkWithSBTypeAndList(bm, ksbfour, n3C2F4D,len);
    if (rt_index.type!=rt_null)
    {
        rt_index.needAction=3;
        CCLog("$扫描 n3C2F4D成功");
        return rt_index;
    }

    
    GET_ARRAY_LEN(n3C2F4M, len)
    rt_index= AIFirstPriority::checkWithSBTypeAndList(bm, ksbfour, n3C2F4M,len);
    if (rt_index.type!=rt_null)
    {
        rt_index.needAction=3;
        CCLog("$扫描 n3C2F4M成功");
        return rt_index;
    }

    
    
    GET_ARRAY_LEN(n3C3F4M, len)
    rt_index= AIFirstPriority::checkWithSBTypeAndList(bm, ksbfour, n3C3F4M,len);
    if (rt_index.type!=rt_null)
    {
        rt_index.needAction=3;
          CCLog("$扫描 n3C3F4M成功");
        return rt_index;
    }
    GET_ARRAY_LEN(n4C3F4M, len)
    rt_index.needAction=4;
    rt_index= AIFirstPriority::checkWithSBTypeAndList(bm, ksbfour, n4C3F4M,len);
    if (rt_index.type!=rt_null)
    {
        rt_index.needAction=4;
        CCLog("$扫描 n4C3F4M成功");
        return rt_index;
    }
    rt_index.needAction=4;
    GET_ARRAY_LEN(n4C4F4M, len)
    rt_index= AIFirstPriority::checkWithSBTypeAndList(bm, ksbfour, n4C4F4M,len);
    
    if (rt_index.type!=rt_null) {
        rt_index.needAction=4;
        CCLog("$扫描 n4C4F4M");
        return rt_index;
    }
    
    return rt_index;
}

#pragma mark 一回合内完成二格武将
ReturnIndex AIFirstPriority::round1GetTwo(SGBattleMap *bm)
{
    ReturnIndex rt_index;
    rt_index.type=rt_null;
    rt_index=AIFirstPriority::need1GetTwo(bm);
    if (rt_index.type==rt_null) {
        rt_index=AIFirstPriority::need2GetTwo(bm);
        if (rt_index.type==rt_null) {
            return rt_index=AIFirstPriority::need3GetTwo(bm);
        }
    }
    
    return rt_index;
}
ReturnIndex AIFirstPriority::need1GetTwo(SGBattleMap *bm)
{
    int len=0;
    GET_ARRAY_LEN(n1C1F2, len)
    ReturnIndex rt_index;
    rt_index.needAction=1;
    rt_index=AIFirstPriority::checkWithSBTypeAndList(bm, ksbtwo, n1C1F2,len);
    if (rt_index.type!=rt_null) {
         rt_index.needAction=1;
        CCLog("$扫描n1c1f2");
        return rt_index;
    }
    
    return rt_index;
}
ReturnIndex AIFirstPriority::need2GetTwo(SGBattleMap *bm)
{
    ReturnIndex rt_index;
    rt_index.type=rt_null;
  
    int len=0;
    GET_ARRAY_LEN(n2C1F2D, len)
    rt_index= AIFirstPriority::checkWithSBTypeAndList(bm, ksbtwo, n2C1F2D,len);
    if (rt_index.type!=rt_null)
    {
          rt_index.needAction=2;
        CCLog("$扫描 n2C1F2D成功");
        return rt_index;
    }

    
    GET_ARRAY_LEN(n2C1F2M, len)
    rt_index= AIFirstPriority::checkWithSBTypeAndList(bm, ksbtwo, n2C1F2M,len);
    if (rt_index.type!=rt_null)
    {
        rt_index.needAction=2;
        CCLog("$扫描 n2C1F2M成功");
        return rt_index;
    }

    
    GET_ARRAY_LEN(n2C2F2D, len)
    rt_index= AIFirstPriority::checkWithSBTypeAndList(bm, ksbtwo, n2C2F2D,len);
    if (rt_index.type!=rt_null)
    {
        rt_index.needAction=2;
        CCLog("$扫描 n2C2F2D成功");
        return rt_index;
    }

    
    GET_ARRAY_LEN(n2C2F2M, len)
    rt_index= AIFirstPriority::checkWithSBTypeAndList(bm, ksbtwo, n2C2F2M,len);
    if (rt_index.type!=rt_null) {
        CCLog("$扫描 n2C2F2M");
        rt_index.needAction=2;
        return rt_index;
    }
    
    return  rt_index;
    
    
}
ReturnIndex AIFirstPriority::need3GetTwo(SGBattleMap *bm)
{
    ReturnIndex rt_index;
    rt_index.type=rt_null;
    rt_index.needAction=3;
    int len=0;
    GET_ARRAY_LEN(n3C2F2D, len)
    rt_index= AIFirstPriority::checkWithSBTypeAndList(bm, ksbtwo, n3C2F2D,len);
    if (rt_index.type!=rt_null)
    {
        CCLog("$扫描 n3C2F2D成功");
        return rt_index;
    }

    
    GET_ARRAY_LEN(n3C2F2M, len)
    rt_index= AIFirstPriority::checkWithSBTypeAndList(bm, ksbtwo, n3C2F2M,len);
    if (rt_index.type!=rt_null)
    {
        CCLog("$扫描n3C2F2M成功");
        return rt_index;
    }

    
    GET_ARRAY_LEN(n3C3F2D, len)
    rt_index= AIFirstPriority::checkWithSBTypeAndList(bm, ksbtwo, n3C3F2D,len);
    if (rt_index.type!=rt_null) {
        CCLog("$扫描n3C3F2D成功");
        return rt_index;
    }
    
    GET_ARRAY_LEN(n3C3F2M, len)
    rt_index= AIFirstPriority::checkWithSBTypeAndList(bm, ksbtwo, n3C3F2M,len);
    if (rt_index.type!=rt_null) {
        CCLog("$扫描n3C3F2M成功");
        return rt_index;
    }
    
    return  rt_index;
    
}

#pragma mark --组成小兵阵列

ReturnIndex AIFirstPriority::move1Get1(SGBattleMap *bm){
    int len=0;
    GET_ARRAY_LEN(n1C1F1M, len)
    return AIFirstPriority::checkWithSBTypeAndList(bm, ksbone,n1C1F1M,len);

}

ReturnIndex AIFirstPriority::makeFastForOne(SGBattleMap *bm){
    int len=0;
    GET_ARRAY_LEN(makeFastF1, len)
    return AIFirstPriority::checkWithSBTypeAndList(bm, ksbone,makeFastF1,len);
    
}


#pragma mark -- 主逻辑
 ReturnIndex AIFirstPriority:: checkAllWithList(SGBattleMap *bm,int list[][ARRAY_ROW][ARRAR_COLOM],int listCount)
{
    ReturnIndex rt_index;
    rt_index.type=rt_null;
    rt_index=AIFirstPriority::checkWithSBTypeAndList(bm,ksbfour, list,listCount);
    if (rt_index.type==rt_null) {
        CCLog("$扫描四格的失败");
        rt_index=AIFirstPriority::checkWithSBTypeAndList(bm, ksbtwo, list,listCount);
        if (rt_index.type==rt_null) {
            CCLog("$扫描二格的失败");
            return rt_index=AIFirstPriority::checkWithSBTypeAndList(bm, ksbone, list,listCount);
        }
    }
    
    return rt_index;
}
ReturnIndex AIFirstPriority::checkWithSBTypeAndList(SGBattleMap *bm,sgShiBingType SBType,int list[][ARRAY_ROW][ARRAR_COLOM],int listCount){

    ReturnIndex rt_index;
    rt_index.type=rt_null;
    CCArray *listSb=CCArray::create();
    listSb->retain();
    

    
    if (SBType==FOURSB) {
        listSb=bm->fourList;
    }
    if (SBType==TWOSB) {
        listSb=bm->twoList;

    }
    if (SBType==ONESB) {
        listSb=bm->oneList;
    }
    
    if (listSb->count()) {
            for (int j=0; j<listCount; j++) {
                for (int i =0; i<listSb->count(); i++) {
                    ShiBing *baseSB=(ShiBing *)listSb->objectAtIndex(i);
                    ReturnIndex index;
                    index.type=rt_null;
                
                    index=AIFirstPriority::checkForSB(bm,list[j], baseSB,SBType);
                    if (index.type!=rt_null) {
                        CCLog("$扫描成功的在list 中的第%d个",j+1);
                        return index;
                    }
            }
        }
    }
    
    
   
    
    return rt_index;

}
ReturnIndex AIFirstPriority::checkForSB(SGBattleMap *bm,int list[][4],ShiBing *SB,sgShiBingType sbType_)
{
    ReturnIndex rt_index;
    rt_index.type=rt_null;
    int sbType=list[0][0];
    if (sbType==sbType_) {
       
        SGGridBase *sbGrid=NULL;
        if (sbType==ksbfour) {
            SGSuperPrinceBase *fourSB=(SGSuperPrinceBase *)SB;
            sbGrid=fourSB->getNumGrid(kleftdown);
            if (sbGrid->getIndex().i==4) {
                SGGridBase *base1=bm->myGrids[1][sbGrid->getIndex().j];
                SGGridBase *base2=bm->myGrids[1][sbGrid->getIndex().j+1];
                
                if (
                    (base1 &&
                     base1->getStype()==kattack &&
                     base1->getSbType()==ksbone) ||
                    (base2 &&
                     base2->getStype()==kattack &&
                     base2->getSbType()==ksbone)
                    ) {
                    return rt_index;
                }
            }
            
        }
        else if(sbType ==ksbtwo)
        {
            SGPrinceBase *twoSB=(SGPrinceBase *)SB;
            sbGrid=twoSB->getNumGrid(kdown);
            
            if (sbGrid->getIndex().i==4) {
                SGGridBase *base1=bm->myGrids[1][sbGrid->getIndex().j];
                
                if (base1 &&
                    base1->getStype()==kattack &&
                    base1->getSbType()==ksbone)
                {
                    return rt_index;
                }
            }

        }
        else
        {
            sbGrid=(SGGridBase *)SB->getPrinceGridArray()->objectAtIndex(0);
            if (sbGrid->getIndex().i>1) {//小兵前面有武将不组小兵
                SGGridBase *wjGrid=bm->myGrids[1][sbGrid->getIndex().j];
                if (wjGrid &&
                    sbGrid &&
                    wjGrid->getSbType()!=ksbone &&
                    wjGrid->getStype()==knormal &&
                    wjGrid->getSaveColorID()==sbGrid->getSaveColorID()) {
                    return rt_index;
                }
            }
        }
        GameIndex baseIndex=sbGrid->getIndex();
            
        int color= sbGrid->getSaveColorID();
        rt_index.colorId=color;
        int offsetI=list[1][2];
        int offsetJ=list[1][3];
        int fourArr[ARRAY_ROW-2][ARRAR_COLOM]={};//有机会把这个去掉吧，直接判断
                
       
        int rowNum=list[1][0];
        int colomNum=list[1][1];
        
        int noMove=-1;
        int noMove1=-1;
        
        for (int row=0; row<rowNum; row++) {
            for (int colom=0; colom<colomNum; colom++) {
                    SGGridBase *baseGrid=NULL;
                
                 
                GameIndex currentIndex=gameIndex(baseIndex.i+offsetI+row, baseIndex.j+offsetJ+colom);
                
                //不能移动判断阵列的行
                int noIndexj=baseIndex.j+offsetJ+colom;
                if (baseIndex.j!=noIndexj && noMove==-1) {
                    noMove=noIndexj;
                }
                if (baseIndex.j!=noIndexj && noMove!=-1 && noIndexj!=noMove && noMove1==-1) {
                    noMove1=noIndexj;
                }
                
                    if (currentIndex.j>=mapList  ||  currentIndex.j<0  ||currentIndex.i<0 ) {
                        return rt_index;
                    }
                    else{
                        
                        if (currentIndex.i>=mapRow) {//
                            fourArr[row][colom]=NO_EXIST;
                            continue;
                        }

                        baseGrid=bm->myGrids[currentIndex.i][currentIndex.j];
                        
                        
                        if (baseGrid && baseGrid->getSaveColorID()==color && baseGrid->getSbType()==ksbone && baseGrid->getStype()==knormal) {
                            fourArr[row][colom]=SAME_COLOR;
                        }
                        else if (baseGrid && baseGrid->getSaveColorID()!=color && baseGrid->getSbType()==ksbone && baseGrid->getStype()!=kattack) {
                            fourArr[row][colom]=DIFF_COLOR;
                        }
                        else if(!baseGrid)
                        { 
                            fourArr[row][colom]=SPACE;
                        }
                        else
                            fourArr[row][colom]=IGNORE;
                        
                    }
                    
                }
        }
            
            
        ///判断是否匹配
            
        for(int i =0;i<rowNum;i++)
            for (int j=0; j<colomNum; j++) {
                    int machValue=list[i+2][j];
                    int curValue=fourArr[i][j];
                    if (machValue==IGNORE && curValue!=NO_EXIST) {
                        
                        continue;
                    }
                    else if (machValue==SAME_COLOR) {
                        
                        if (curValue!=machValue) {
                            return rt_index;
                        }
                    }
                    else if (machValue==SPACE) {
                        if (curValue!=machValue) {
                            return rt_index;
                        }
                    }
                    
                    else if (machValue==DIFF_COLOR) {
                         
                        if (curValue!=machValue) {
                            return rt_index;
                        }
                    }
                    else if(machValue==NO_EXIST)
                    {
                        if (curValue!=machValue) {
                            return rt_index;
                        }
                    }else if(machValue==NO_EXIST_OR_SPACE)
                    {
                        if (curValue==NO_EXIST ||  curValue==SPACE) {
                            continue;
                        }
                        else
                        {
                            return rt_index;
                        }
                    }
                
                }
        
        ///匹配成功了,下面可以判断是执行什么样 的操作，返回不同的值，这里只是删除一个的情况
        
        int typeName=list[0][1];
        
        SGGridBase *acGrid=NULL;//防止卡死
        
        switch (typeName) {
            case DEL:
                
            {
                
                int OFFSETJi= list[0][2];
                int OFFSETJj= list[0][3];
                 rt_index.delIndex1=gameIndex(baseIndex.i+OFFSETJi, baseIndex.j+OFFSETJj);
                //特殊情况list7的第11个,删除的不是normal，或者不是一格的武将
                SGGridBase *grid1=bm->myGrids[rt_index.delIndex1.i][rt_index.delIndex1.j];
                if (OFFSETJi<0 || OFFSETJj <0) {
                    if (rt_index.delIndex1.i==2) {
                        SGGridBase *grid2=bm->myGrids[1][rt_index.delIndex1.j];
                        SGGridBase *grid3=bm->myGrids[0][rt_index.delIndex1.j];
                        if (grid1 &&
                            grid1->getStype()==knormal &&
                            grid1->getSbType()==ksbone &&
                            grid2 &&
                            grid2->getStype()==knormal &&
                            grid2->getSbType()==ksbone &&
                            grid3 &&
                            grid3->getStype()!=kattack &&
                            grid3->getSbType()==ksbone &&
                            grid1->getSaveColorID()==grid2->getSaveColorID()) {
                            rt_index.delIndex1=grid3->getIndex();
                            rt_index.type=rt_del1;
                            return rt_index;
                        }else if(grid1 &&
                                 grid1->getStype()==knormal &&
                                 grid1->getSbType()==ksbone &&
                                 grid2 &&
                                 grid2->getStype()!=kattack &&
                                 grid2->getSbType()==ksbone &&
                                 grid3 &&
                                 grid3->getStype()==knormal &&
                                 grid3->getSbType()==ksbone &&
                                 grid1->getSaveColorID()==grid3->getSaveColorID())
                        {
                            rt_index.delIndex1=grid2->getIndex();
                            rt_index.type=rt_del1;
                            return rt_index;
                        }
                            
                    }
                }
                
                
                if (grid1 &&
                    grid1->getStype()!=kattack &&
                    grid1->getSbType()==ksbone) {
                    acGrid=bm->myGrids[rt_index.delIndex1.i][rt_index.delIndex1.j];
                    if (acGrid) {
                        rt_index.type=rt_del1;
                        CCLog("$删除的是普通的小兵<%d,%d>,删除得到阵列的index",rt_index.delIndex1.i,rt_index.delIndex1.j);
                        CCLog("$<%d,%d>,主将的的index",baseIndex.i,baseIndex.j);
                        return rt_index;
                    }
                    
                   
                }
                if ((grid1  &&
                    grid1->getSbType()==ksbtwo) || (grid1 && grid1->getSbType()==ksbfour)) {
                    SGGridBase *baseGrid3=bm->myGrids[grid1->getIndex().i-2][grid1->getIndex().j];
                    if (baseGrid3 &&
                        baseGrid3->getSbType()==ksbone &&
                        baseGrid3->getSbType()!=kattack)//其实没有必要加最后一个判断
                    {
                        
                        rt_index.type=rt_del1;
                        rt_index.delIndex1=gameIndex(baseGrid3->getIndex().i, baseGrid3->getIndex().j);
                        CCLog("$删除的前面是二格武将<%d,%d>,删除得到阵列的index",rt_index.delIndex1.i,rt_index.delIndex1.j);
                        CCLog("$<%d,%d>,主将的的index",baseIndex.i,baseIndex.j);
                        return rt_index;
                        
                       
                    }
                    //删除武将的前面是空,则删除左边的
                    if (!baseGrid3) {
                        if (grid1->getIndex().j!=mapList-1) {
                            SGGridBase *baseGrid4=bm->myGrids[grid1->getIndex().i-2][grid1->getIndex().j+1];
                            if (baseGrid4 && baseGrid4->getSbType()==ksbfour &&
                                baseGrid4->getSaveColorID()==grid1->getSaveColorID()) {
                                SGGridBase *baseGrid5=bm->myGrids[baseGrid3->getIndex().i][baseGrid3->getIndex().j+1];
                                if (baseGrid5 &&
                                    baseGrid5->getSbType()==ksbone &&
                                    baseGrid5->getStype()!=kattack) {
                                        rt_index.type=rt_del1;
                                        rt_index.delIndex1=gameIndex(baseGrid5->getIndex().i, baseGrid5->getIndex().j);
                                        CCLog("$删除的前面是二格武将左边的<%d,%d>,删除得到阵列的index",rt_index.delIndex1.i,rt_index.delIndex1.j);
                                        CCLog("$<%d,%d>,主将的的index",baseIndex.i,baseIndex.j);
                                        return rt_index;
                                }
                            }
                        }
                        
                    }
                    
                    //删除武将的前面是空,则删除右边的
                    if (!baseGrid3) {
                        if (grid1->getIndex().j!=0) {
                            SGGridBase *baseGrid4=bm->myGrids[grid1->getIndex().i-2][grid1->getIndex().j-1];
                            if (baseGrid4 && baseGrid4->getSbType()==ksbfour &&
                                baseGrid4->getSaveColorID()==grid1->getSaveColorID()) {
                                SGGridBase *baseGrid5=bm->myGrids[baseGrid3->getIndex().i][baseGrid3->getIndex().j-1];
                                if (baseGrid5 &&
                                    baseGrid5->getSbType()==ksbone &&
                                    baseGrid5->getStype()!=kattack) {
                                    rt_index.type=rt_del1;
                                    rt_index.delIndex1=gameIndex(baseGrid5->getIndex().i, baseGrid5->getIndex().j);
                                    CCLog("$删除的前面是二格武将右边的<%d,%d>,删除得到阵列的index",rt_index.delIndex1.i,rt_index.delIndex1.j);
                                    CCLog("$<%d,%d>,主将的的index",baseIndex.i,baseIndex.j);
                                    return rt_index;
                                }
                            }
                        }
                        
                    }
                    
                }
                    
                    
            }
                break;
            case MOVE:
            {
               
                int OFFSETJi= list[0][2];
                int OFFSETJj= list[0][3];
                
                //组小兵的时候判断
                rt_index.to1=gameIndex(baseIndex.i+OFFSETJi, baseIndex.j+OFFSETJj);
                if (rt_index.to1.i!=0) {
                    SGGridBase *preGridBase=bm->myGrids[rt_index.to1.i-1][rt_index.to1.j];
                    if (!preGridBase) {
                         rt_index.type=rt_null;
                        return rt_index;
                    }
                }
                
                
                GameIndex index;
                
                //index= SGMov::getMoveIndex(bm, color, 1,baseIndex.j,baseIndex.j+1);
                //组小兵的时候判断

//                int noMove=-1;
//                if (sbType==ksbone  //不能移动判断的行列
//                    && colomNum==2)
//                {
//                    if(OFFSETJj==-2 ) {
//                        noMove=baseIndex.j-1;
//                    }else if (OFFSETJj==2){
//                        noMove=baseIndex.j+1;
//                    }else if(OFFSETJj==1)
//                    {
//                        noMove  =baseIndex.j+2;
//                    }
//                }
                
//                if (sbType==ksbone && colomNum>2) {
//                    
//                    
//                }
                index=AIFirstPriority::getFromIndexWithSB(bm, color, baseIndex, sbType_,noMove,noMove1);
                 
                if (index.i==-1) {
                    
                    ReturnIndex index =AIFirstPriority::getSBByDig(bm, color,baseIndex,sbType_);//挖出的情况
                    if (index.type!=rt_null) {
                        bool isRepeat=AIFirstPriority::isRepeatMove(index.from1, index.to1,bm);
                        if (!isRepeat) {
                            CCLog("$挖出的情况");
                            CCLog("$<%d,%d>,主将的的index", baseIndex.i,baseIndex.j);
                            CCLog("$<%d,%d>,移动到的阵列的index", index.to1.i,index.to1.j);
                            CCLog("$<%d,%d>,移动的阵列的index", index.from1.i,index.from1.j);
                            return index;
                        }
                        
                    }
                    rt_index.type=rt_null;
                }
                else
                {
                    rt_index.from1=index;
                    if (rt_index.to1.j==rt_index.from1.j) {//防止移动到同一列
                        rt_index.type=rt_null;
                        
                    }else
                    {
                        acGrid=bm->myGrids[rt_index.to1.i][rt_index.to1.j];
                        if (!acGrid) {
                            
                            bool isRepeat=AIFirstPriority::isRepeatMove(rt_index.from1, rt_index.to1,bm);
                            if (!isRepeat) {
                                rt_index.type=rt_from1to1;
                                CCLog("$正常移动:<%d,%d>,主将的的index", baseIndex.i,baseIndex.j);
                                CCLog("$<%d,%d>,移动到的阵列的index", rt_index.to1.i,rt_index.to1.j);
                                CCLog("$<%d,%d>,移动的阵列的index", rt_index.from1.i,rt_index.from1.j);
                            }else
                                rt_index.type=rt_null;
                        }else
                            rt_index.type=rt_null;
                        
                        
                    }
                        
                    return rt_index;
                }
                
            }
                break;
                
                case MOVETO:
            {
                
                int OFFSETJi= list[0][2];
                int OFFSETJj= list[0][3];
                rt_index.from1=gameIndex(baseIndex.i+OFFSETJi, baseIndex.j+OFFSETJj);
                GameIndex index;
                
                SGGridBase *basegrid=bm->myGrids[rt_index.from1.i][rt_index.from1.j];//移动的是组成的防御阵列
                
                if ((basegrid &&
                    basegrid->getStype()!=knormal) || !basegrid) {
                    rt_index.type=rt_null;
                    return rt_index;
                }
                
                
                //index= SGMov::getMoveIndex(bm, color, 1,baseIndex.j,baseIndex.j+1);
                index=AIFirstPriority::getToIndexWithSB(bm, basegrid->getSaveColorID(), baseIndex, sbType_);
                
                if (index.i==-1) {
                     rt_index.type=rt_null;
                }
                else
                {
                    rt_index.to1=index;
                    bool isRepeat=AIFirstPriority::isRepeatMove(rt_index.from1, rt_index.to1,bm);
                    if (!isRepeat) {
                        rt_index.type=rt_from1to1;
                        CCLog("$移动武将后面的<%d,%d>,主将的的index", baseIndex.i,baseIndex.j);
                        CCLog("$<%d,%d>,移动到的阵列的index", rt_index.to1.i,rt_index.to1.j);
                        CCLog("$<%d,%d>,移动的阵列的index", rt_index.from1.i,rt_index.from1.j);
                        return rt_index;
                    }
                }
                
            }
                break;
            default:
                break;
        }
        
            
           
    }
    
     
    return rt_index;
}



#pragma mark 快速组武将的逻辑

ReturnIndex AIFirstPriority::fastMakeAttackForSB(SGBattleMap *bm, sgShiBingType SBType){
    
    ReturnIndex rt_index;
    rt_index.type=rt_null;
    CCArray *listSb=CCArray::create();
    listSb->retain();
    
    
    
    if (SBType==ksbfour) {
        listSb=bm->fourList;
    }
    if (SBType==ksbtwo) {
        listSb=bm->twoList;
        
    }
   
    for (int i=0; i<listSb->count(); i++) {
        ShiBing *SB=(ShiBing *)listSb->objectAtIndex(i);
         SGGridBase *sbGrid=NULL;
        if (SBType==ksbfour) {
            SGSuperPrinceBase *fourSB=(SGSuperPrinceBase *)SB;
            sbGrid=fourSB->getNumGrid(kleftdown);
        }
        else if(SBType ==ksbtwo)
        {
            SGPrinceBase *twoSB=(SGPrinceBase *)SB;
            sbGrid=twoSB->getNumGrid(kdown);
        }
        else
        {
            continue;
        }
        rt_index=checkForDeletePreSB(bm, sbGrid->getIndex(), SBType);//删除前面的
        
        if (rt_index.type!=rt_null) {
            CCLog("$删除前面的");
              CCLog("$<%d,%d>",rt_index.delIndex1.i,rt_index.delIndex1.j);
            return rt_index;
            
        }
         rt_index= AIFirstPriority::checkForMoveLastSB(bm, sbGrid->getIndex(), SBType, sbGrid->getSaveColorID());
        if (rt_index.type!=rt_null) {//移走后面的
            CCLog("$移走后面的");
              CCLog("$<%d,%d>",rt_index.from1.i,rt_index.from1.j);
                 CCLog("$<%d,%d>",rt_index.to1.i,rt_index.to1.j);
            return rt_index;
        }
        
        rt_index= AIFirstPriority::checkForDeleteLastSB(bm, sbGrid->getIndex(), SBType, sbGrid->getSaveColorID());
        if (rt_index.type!=rt_null) {//删除后面的
            CCLog("$删除后面的");
            CCLog("$<%d,%d>",rt_index.delIndex1.i,rt_index.delIndex1.j);
            return rt_index;
        }
        
    }
    return rt_index;
}
ReturnIndex AIFirstPriority::checkForDeleteLastSB(SGBattleMap *bm, GameIndex SBIndex, sgShiBingType sbType,int colorID){
    ReturnIndex rt_index;
    rt_index.type=rt_null;
    
    if (sbType==ksbfour) {
        GameIndex gameIndex1=AIFirstPriority::getDeleteIndex(bm, SBIndex, colorID);
        GameIndex sbIndex1;
        sbIndex1.i=SBIndex.i;
        sbIndex1.j=SBIndex.j+1;
        GameIndex gameIndex2=AIFirstPriority::getDeleteIndex(bm, sbIndex1, colorID);
        if (gameIndex1.i!=-1 && gameIndex2.i!=-1) {
             rt_index.type=rt_del1;
            if (gameIndex1.i<gameIndex2.i) {
                 rt_index.delIndex1=gameIndex1;
            }else
                rt_index.delIndex1=gameIndex2;

            
        }
        else if(gameIndex1.i!=-1)
        {
            rt_index.type=rt_del1;
            rt_index.delIndex1=gameIndex1;
        }
        else if (gameIndex2.i!=-1)
        {
            rt_index.type=rt_del1;
            rt_index.delIndex1=gameIndex2;

        }
            
        
    }
    else if (sbType==ksbtwo)
    {
        GameIndex gameIndex1=AIFirstPriority::getDeleteIndex(bm, SBIndex, colorID);
        if(gameIndex1.i!=-1)
        {
            rt_index.type=rt_del1;
            rt_index.delIndex1=gameIndex1;
        }
    }
    
    
    
    return rt_index;
}
GameIndex AIFirstPriority::getDeleteIndex(SGBattleMap *bm, GameIndex sbIndex,int colorID){
    GameIndex gameIndex;
    gameIndex.i=-1;
    gameIndex.j=-1;
    
    int k=0;
    for (int i=sbIndex.i+1; i<mapRow-2; i++) {
        SGGridBase *baseGrid=bm->myGrids[i][sbIndex.j];
        if (baseGrid && baseGrid->getStype()!=kattack && baseGrid->getSbType()==ksbone && baseGrid->getSaveColorID()!=colorID) {
            gameIndex.i=i;
            gameIndex.j=sbIndex.j;
            k++;
           // return gameIndex;
        }
    }
    if (k>1) {
        return gameIndex;
    }else
    {
        gameIndex.i=-1;
        gameIndex.j=-1;
        return gameIndex;
    }
}

ReturnIndex AIFirstPriority::checkForDeletePreSB(SGBattleMap *bm, GameIndex SBIndex, sgShiBingType SBType){
    ReturnIndex rt_index;
    rt_index.type=rt_null;
    
    
    if (SBIndex.i<2) {
        return rt_index;
    }
    
    if (SBIndex.i<mapRow-2) {
        
        
        if (SBType==ksbfour) {
            SGGridBase *nexBase=bm->myGrids[SBIndex.i+2][SBIndex.j];
            SGGridBase *nexBase2=bm->myGrids[SBIndex.i+2][SBIndex.j+1];
            if ((nexBase && nexBase->getSbType()!=ksbone) || (nexBase2 && nexBase2->getSbType()!=ksbone)) {
                
            }else
                return rt_index;
            
        }
        else if (SBType==ksbtwo) {
            SGGridBase *nexBase=bm->myGrids[SBIndex.i+2][SBIndex.j];
            if (nexBase && nexBase->getSbType()!=ksbone) {
                
            }else
                return rt_index;
            
        }
    }
    
    if (SBType==ksbfour) {
        bool oneAttack=AIFirstPriority::checkIsHasAttack(bm, SBIndex.j, SBIndex.i-2);
        bool twoAttack=AIFirstPriority::checkIsHasAttack(bm, SBIndex.j+1, SBIndex.i-2);
        if (oneAttack && twoAttack) {
            SGGridBase *baseGrid=bm->myGrids[SBIndex.i-2][SBIndex.j];
            if (baseGrid) {
                rt_index.type=rt_del1;
                rt_index.delIndex1=gameIndex(SBIndex.i-2, SBIndex.j);
                return rt_index;
            }
            else
            {
                rt_index.type=rt_del1;
                rt_index.delIndex1=gameIndex(SBIndex.i-2, SBIndex.j+1);
                return rt_index;
            }
        }
    }else if (SBType==ksbtwo)
    {
        if (AIFirstPriority::checkIsHasAttack(bm, SBIndex.j, SBIndex.i-2)) {
            rt_index.type=rt_del1;
            rt_index.delIndex1=gameIndex(SBIndex.i-2, SBIndex.j);
            return rt_index;
        }
        
    }
    
    
    return rt_index;
}
bool AIFirstPriority::checkIsHasAttack(SGBattleMap *bm, int indexJ, int endI){

    for (int i=0;i<=endI; i++) {
        SGGridBase *baseGrid=bm->myGrids[i][indexJ];
        if (baseGrid &&
            baseGrid->getSbType()==ksbone &&
            baseGrid->getStype()!=kattack) {
            continue;
        }
        else if(!baseGrid)
            continue;
        return false;
            
    }
    return true;
}
ReturnIndex AIFirstPriority::checkForMoveLastSB(SGBattleMap *bm, GameIndex SBIndex, sgShiBingType SBType,int colorID){
    ReturnIndex rt_index;
    rt_index.type=rt_null;
    int checkLastJ=SBIndex.j;
    
    if (SBType==ksbfour) {
        checkLastJ=SBIndex.j+1;
    }
    
    for (int j=SBIndex.j; j<=checkLastJ; j++) {
        for (int i=mapRow-1; i>SBIndex.i; i--) {
            SGGridBase *baseGrid=bm->myGrids[i][j];
            if (baseGrid) {
                
                if (i-SBIndex.i>2) {//移动的后面有武将的情况
                    SGGridBase *baseGird2=bm->myGrids[i-2][j];
                    if (baseGird2  && baseGird2->getSbType()!=ksbone) {
                        break;
                    }
                }
                
                if (baseGrid->getStype()==knormal && baseGrid->getSbType()==ksbone && baseGrid->getSaveColorID()!=colorID) {
                    GameIndex toIndex=AIFirstPriority::getToIndexWithSB(bm, baseGrid->getSaveColorID(), SBIndex, SBType);
                    if (toIndex.i!=-1) {
                        bool isRepeat=AIFirstPriority::isRepeatMove(gameIndex(i, j), toIndex,bm);
                        if (!isRepeat) {
                            rt_index.from1=gameIndex(i, j);
                            rt_index.to1=gameIndex(toIndex.i, toIndex.j);
                            rt_index.type=rt_from1to1;
                            return rt_index;
                        }
                       
                    }
                    
                }
                break;
            }
        }
    }
    return rt_index;
}

GameIndex AIFirstPriority::getToIndexWithSB(SGBattleMap *bm, int colorID, GameIndex SBIndex, sgShiBingType SBType)
{
    GameIndex index;
    index.i=-1;
    index.j=-1;
    
   
    int noIndexj1=-1;
    int  noIndexj0=SBIndex.j;
    if (SBType==ksbfour) {
       
        noIndexj1=SBIndex.j+1;
    }
    
    int sameColor[8][2]={-1};// 检查最后是否有两个相同颜色的，有空位，相同的个数
    for (int m =0; m<8; m++) {
        sameColor[m][0]=-4;
         sameColor[m][1]=-4;
    }
    
    
    for (int j=0; j<=mapList-1; j++) {
        bool exist=false;
        bool hasSPace=false;
        if (noIndexj0==j || noIndexj1==j) {
            continue ;
        }
        int firstI=-1;
         
        for (int i=mapRow-1; i>=0; i--) {
             
            SGGridBase *baseGrid=bm->myGrids[i][j];

            if (!baseGrid && !exist ) {// 最后一次找到空格
                sameColor[j][0]=i;
                hasSPace=true;
                firstI=i-1;
                sameColor[j][1]=sameColor[j][1]+1;
                continue;
            }
            
          
            
            if (!hasSPace) {//该行已经没有了可放置的位置了
                break;
            }
            
            if (baseGrid || exist)// 找到非空格，或者 找到非空格，之后的一个位置是空格
            {
                exist=true;
                
            }
//            
            if (baseGrid &&
                baseGrid->getStype()==knormal &&
                baseGrid->getSaveColorID()==colorID &&
                baseGrid->getSbType()==ksbone) {
                sameColor[j][1]=sameColor[j][1]+i+3;
            }
            
            if (baseGrid &&
                baseGrid->getStype()==knormal &&
                baseGrid->getSaveColorID()==colorID &&
                baseGrid->getSbType()==ksbfour) {
                sameColor[j][1]=sameColor[j][1]+i*3;
            }
            if (baseGrid &&
                baseGrid->getStype()==knormal &&
                baseGrid->getSaveColorID()==colorID &&
                baseGrid->getSbType()==ksbtwo) {
                sameColor[j][1]=sameColor[j][1]+i*2;
            }
            
            if (baseGrid &&
                baseGrid->getSbType()==ksbfour &&
                baseGrid->getStype()==knormal &&
                baseGrid->getSaveColorID()!=colorID) {
                sameColor[j][1]=-2;
                break;
            }
            if (baseGrid &&
                baseGrid->getSbType()==ksbtwo&&
                baseGrid->getStype()==knormal &&
                baseGrid->getSaveColorID()!=colorID) {
                sameColor[j][1]=-1;
                break;
            }
            if (baseGrid &&
                baseGrid->getSbType()==ksbtwo&&
                baseGrid->getStype()==kattack) {
                sameColor[j][1]=sameColor[j][1]-1;
                break;
            }if (baseGrid &&
                 baseGrid->getSbType()==ksbfour&&
                 baseGrid->getStype()==kattack) {
                sameColor[j][1]=sameColor[j][1]-2;
                break;
            }
           // int len=firstI-i;
            
            
             /*   if (len>2) {
                    break;
                }
                
                
                if (baseGrid &&
                    baseGrid->getSaveColorID()==colorID &&
                    baseGrid->getStype()==knormal &&
                    baseGrid->getSbType()==ksbone )
                {
                    
                    sameColor[j][1]=sameColor[j][1]+40;
                    
                }
                else if(baseGrid &&
                        baseGrid->getSaveColorID()==colorID &&
                        baseGrid->getStype()==knormal &&
                        baseGrid->getSbType()==ksbfour){
                    sameColor[j][1]=sameColor[j][1]+40;
                }else if(baseGrid &&
                         baseGrid->getSaveColorID()!=colorID &&
                         baseGrid->getStype()==knormal &&
                         baseGrid->getSbType()==ksbone){
                    sameColor[j][1]=sameColor[j][1]+0;
                }else if(baseGrid &&
                         baseGrid->getSaveColorID()==colorID &&
                         baseGrid->getStype()==knormal &&
                         baseGrid->getSbType()==ksbtwo){
                    sameColor[j][1]=sameColor[j][1]+20;
                }else if(baseGrid &&
                         baseGrid->getSaveColorID()!=colorID &&
                         baseGrid->getStype()==knormal &&
                         baseGrid->getSbType()==ksbtwo){
                    sameColor[j][1]=sameColor[j][1]+4;
                }else if(baseGrid &&
                         baseGrid->getSaveColorID()!=colorID &&
                         baseGrid->getStype()==knormal &&
                         baseGrid->getSbType()==ksbfour){
                    sameColor[j][1]=sameColor[j][1]+0;
                }
                if (len==1) {
                    if (baseGrid &&
                        baseGrid->getSaveColorID()==colorID &&
                        baseGrid->getStype()==knormal &&
                        baseGrid->getSbType()==ksbone) {
                        sameColor[j][1]=sameColor[j][1]+12;
                    }
                    else
                    {
                        sameColor[j][1]=sameColor[j][1]+8;
                    }
                    
                }
                else if(len==2)
                {
                    if (baseGrid &&
                        baseGrid->getSaveColorID()==colorID &&
                        baseGrid->getStype()==knormal &&
                        baseGrid->getSbType()==ksbone) {
                        sameColor[j][1]=sameColor[j][1]+12;
                    }
                    else
                    {
                        sameColor[j][1]=sameColor[j][1]+10;
                    }
                }
                else if(len==3)
                {
                    
                    if (baseGrid &&
                        baseGrid->getSaveColorID()==colorID &&
                        baseGrid->getStype()==knormal &&
                        baseGrid->getSbType()==ksbone) {
                        sameColor[j][1]=sameColor[j][1]-6;
                    }
                    else
                    {
                        sameColor[j][1]=sameColor[j][1]-4;
                    }
                }
                */
              
                       
        }
    }
    
    int max=-4;
    int maxj=-4;//阈值最大的
    for (int k=0; k<mapList; k++) {
        if (sameColor[k][0]>-4) {
            if (max<sameColor[k][1]) {
                max=sameColor[k][1];
                maxj=k;
            }
        }
    }
    CCLog("$最大的时%d",max);
    CCLog("$index.i=%d",sameColor[maxj][0]);
    CCLog("$index.j=%d",maxj);
    if (max>-4) {
        index.i=sameColor[maxj][0];
        index.j=maxj;
    }
    return  index ;
}

GameIndex AIFirstPriority::getFromIndexWithSB(SGBattleMap *bm, int colorID,GameIndex SBIndex , sgShiBingType SBType,int nMove,int nMove1){

    GameIndex gameIndex;
    gameIndex.i=-1;
    gameIndex.j=-1;
    int noindexj1=SBIndex.j;
    int noindexj2=-1;
    if (SBType==ksbfour) {
        noindexj2=SBIndex.j+1;
    }
    
     bool hasSameColor=false;
    int sameColor[8][2]={-1};//-1代表没有同颜色的其他的代表row,第二个值代表计算的值。
    for (int i=0; i<8; i++) {
        sameColor[i][0]=-1;
        sameColor[i][1]=-1;
    }
    
    for (int j=0; j<mapList; j++) {
        if (j==noindexj1 || j==noindexj2 || nMove==j || nMove1==j) {
            continue;
        }
        bool first=false;
        int firstI=-1;
        int hasWJ=false;
        
        for (int k=mapRow-1; k>=0; k--) {
            
            SGGridBase *baseGrid=bm->myGrids[k][j];
            if (baseGrid && !first)//第一次得到不为空的位置
            {
                first=true;
                if (baseGrid->getSaveColorID()==colorID &&
                    baseGrid->getStype()==knormal &&
                    baseGrid->getSbType()==ksbone )
                {
                    hasSameColor=true;
                    sameColor[j][0]=k;
                    firstI=k;
                    sameColor[j][1]=6;
                    continue;
                }else
                    break;
                 
            
            }
            if (baseGrid &&
                baseGrid->getSaveColorID()==colorID &&
                baseGrid->getStype()==knormal &&
                baseGrid->getSbType()==ksbone) {
                sameColor[j][1]=sameColor[j][1]+k+1;
            }
            int len=firstI-k;
         /*
            if (!hasWJ) {
                if (len>2) {
                    break;
                }
                
                
                if (baseGrid &&
                    baseGrid->getSaveColorID()==colorID &&
                    baseGrid->getStype()==knormal &&
                    baseGrid->getSbType()==ksbone )
                {
                    sameColor[j][1]=sameColor[j][1]+40;
                    
                }
                else if(baseGrid &&
                        baseGrid->getSaveColorID()==colorID &&
                        baseGrid->getStype()==knormal &&
                        baseGrid->getSbType()==ksbfour){
                    sameColor[j][1]=sameColor[j][1]+40;
                }else if(baseGrid &&
                         baseGrid->getSaveColorID()!=colorID &&
                         baseGrid->getStype()==knormal &&
                         baseGrid->getSbType()==ksbone){
                    sameColor[j][1]=sameColor[j][1]+0;
                }else if(baseGrid &&
                         baseGrid->getSaveColorID()==colorID &&
                         baseGrid->getStype()==knormal &&
                         baseGrid->getSbType()==ksbtwo){
                    sameColor[j][1]=sameColor[j][1]+20;
                }else if(baseGrid &&
                         baseGrid->getSaveColorID()!=colorID &&
                         baseGrid->getStype()==knormal &&
                         baseGrid->getSbType()==ksbtwo){
                    sameColor[j][1]=sameColor[j][1]+4;
                }else if(baseGrid &&
                         baseGrid->getSaveColorID()!=colorID &&
                         baseGrid->getStype()==knormal &&
                         baseGrid->getSbType()==ksbfour){
                    sameColor[j][1]=sameColor[j][1]+0;
                }
                 
                if (len==1) {
                    if (baseGrid &&
                        baseGrid->getSaveColorID()==colorID &&
                        baseGrid->getStype()==knormal &&
                        baseGrid->getSbType()==ksbone) {
                        sameColor[j][1]=sameColor[j][1]+12;
                    }
                    else
                    {
                        sameColor[j][1]=sameColor[j][1]+8;
                    }
                    
                }
                else if(len==2)
                {
                    if (baseGrid &&
                        baseGrid->getSaveColorID()==colorID &&
                        baseGrid->getStype()==knormal &&
                        baseGrid->getSbType()==ksbone) {
                        sameColor[j][1]=sameColor[j][1]+12;
                    }
                    else
                    {
                        sameColor[j][1]=sameColor[j][1]+10;
                    }
                }
                else if(len==3)
                {
                    
                    if (baseGrid &&
                        baseGrid->getSaveColorID()==colorID &&
                        baseGrid->getStype()==knormal &&
                        baseGrid->getSbType()==ksbone) {
                        sameColor[j][1]=sameColor[j][1]-6;
                    }
                    else
                    {
                        sameColor[j][1]=sameColor[j][1]-4;
                    }
                }

            }*/
                
            
        }
    }
    
   
    if (hasSameColor) {
        
        int min=1000;//阈值最小的
        int getIndexj;
        for (int j=0; j<mapList; j++) {
            if (sameColor[j][0]>-1) {
                if (sameColor[j][1]<min) {
                    min=sameColor[j][1];
                    getIndexj=j;
                }
            }
        }
        
        gameIndex.j=getIndexj;
        gameIndex.i=sameColor[getIndexj][0];
        return gameIndex;
        
    }
    
    
    return gameIndex;
    
}

ReturnIndex AIFirstPriority::getSBByDig(SGBattleMap *bm, int colorID,GameIndex sbIndex,sgShiBingType sbType){
    ReturnIndex rt_index;
    rt_index.type=rt_null;
    
    int noindexj0=sbIndex.j;
    int noindexj1=-1;
    if (sbType==ksbfour) {
        noindexj1=sbIndex.j+1;
    }
    
    //检查攻击阵列挖出情况
    for (int j=0; j<mapList; j++) {
        int sameColor;
        if (noindexj0==j || noindexj1==j) {
            continue;
        }
        int hasSpace =false;
        for (int i = mapRow-1; i>=0; i--) {
            SGGridBase *baseGrid=bm->myGrids[i][j];
            if (!baseGrid) {
                hasSpace=true;
                continue;
            }
            
            if (hasSpace && baseGrid && i>1  && i<mapRow-1) {//第一次
                 
                if (baseGrid->getStype()==knormal &&
                    baseGrid->getSbType()==ksbone) {
                    sameColor=baseGrid->getSaveColorID();
                    
                    SGGridBase *baseGrid1=bm->myGrids[i-1][j];
                    SGGridBase *baseGrid2=bm->myGrids[i-2][j];
                    
                    if (baseGrid1 &&
                        baseGrid1->getSaveColorID()==sameColor &&
                        baseGrid1->getStype()==knormal &&
                        baseGrid1->getSbType()==ksbone &&
                        baseGrid2 &&
                        baseGrid2->getStype()==knormal &&
                        baseGrid2->getSaveColorID()==colorID &&
                        baseGrid2->getSbType()==ksbone) {
                        GameIndex fromIndex=AIFirstPriority::getFromIndexWithSB(bm, sameColor, baseGrid->getIndex(), ksbone);
                        if (fromIndex.i!=-1) {
                            bool isRepeat=AIFirstPriority::isRepeatMove(fromIndex, gameIndex(baseGrid->getIndex().i+1, baseGrid->getIndex().j), bm);
                            if (!isRepeat) {
                                CCLog("$阵列挖出");
                                rt_index.type=rt_from1to1;
                                rt_index.to1=gameIndex(baseGrid->getIndex().i+1, baseGrid->getIndex().j);
                                rt_index.from1=fromIndex;
                                return rt_index;
                            }
                           
                        }
                    }
                     
                }
                else
                    break;
            }else
                break;
                
            
            
            
        }
    }
    
    for (int j=0; j<mapList; j++) {
        if (noindexj0==j || noindexj1==j) {
            continue;
        }
        for (int i=mapRow-1; i>0; i--) {
            SGGridBase *baseGrid=bm->myGrids[i][j];
            SGGridBase *targetGrid=bm->myGrids[i-1][j];
            if (baseGrid) {//只检查第一个和需要的颜色是不是不同
                if (baseGrid &&
                    baseGrid->getStype()==knormal &&
                    baseGrid->getSbType()==ksbone &&
                    baseGrid->getSaveColorID()!=colorID &&
                    targetGrid &&
                    targetGrid->getStype()==knormal &&
                    targetGrid->getSbType()==ksbone &&
                    targetGrid->getSaveColorID()==colorID
                    ) {
                    GameIndex toIndex=AIFirstPriority::getToIndexWithSB(bm, baseGrid->getSaveColorID(), baseGrid->getIndex(), ksbone);
                    if (toIndex.i!=-1) {
                        bool isRepeat=AIFirstPriority::isRepeatMove(baseGrid->getIndex(), toIndex, bm);
                        if (!isRepeat) {
                            CCLog("$移动挖出");
                            rt_index.type=rt_from1to1;
                            rt_index.to1=toIndex;
                            rt_index.from1=baseGrid->getIndex();
                            return rt_index;

                        }
                        
                    }
                }
                else
                    break;

            }
            
        }
    }
    
    
    //检查防御阵列挖出情况
    return rt_index;
}
#pragma mark 移动四格二格武将的情况
 ReturnIndex AIFirstPriority::moveWJ(SGBattleMap *bm){
    ReturnIndex rt_index;
    rt_index.type=rt_null;
    
    SBInfo preWJInfo;
     //检查普通攻击阵列后面有没有可移动的武将,暂不考虑武将攻击阵列和墙的情况
     for (int i=0; i<bm->getAttackList()->count(); i++) {
         SGAttackList *attackList=(SGAttackList *)bm->getAttackList()->objectAtIndex(i);
         if (attackList->getAttackType() == kattacknormal && attackList->getRoundV()>1) {//当攻击阵列是普通的，而且攻击回合还有两个以上
             SGNormalAttackList *normalAttackList=(SGNormalAttackList *)attackList;
             GameIndex lastIndex=normalAttackList->getLastIndex();
             if (lastIndex.i<mapRow-2) {
                 SGGridBase *baseGrid1=bm->myGrids[lastIndex.i+1][lastIndex.j];
                 SGGridBase *baseGrid2=bm->myGrids[lastIndex.i+2][lastIndex.j];
                 SGGridBase *baseGrid;
                 if (baseGrid1 &&
                     baseGrid2 &&
                     baseGrid1->getSbType()==baseGrid2->getSbType()) {
                     baseGrid =baseGrid2;
                 }else if(baseGrid1 &&
                          baseGrid2 &&
                          baseGrid1->getSbType()==ksbone &&
                          baseGrid2->getSbType()!=ksbfour)
                     baseGrid =bm->myGrids[lastIndex.i+3][lastIndex.j];
                 else
                     break;
                 
                 if (baseGrid &&
                     baseGrid->getStype()==knormal &&
                     baseGrid->getSbType()==ksbfour) {
                     
                     CCLog("$武将后面有武将");
                     GameIndex baseIndex=AIFirstPriority::getBaseIndex(bm, baseGrid->getIndex());
                     bool isNUll=AIFirstPriority::checkWJNextISNULL(bm,baseIndex);
                     bool isNUll2=AIFirstPriority::checkWJNextISNULL(bm,gameIndex(baseIndex.i, baseIndex.j+1));
                     if (isNUll && isNUll2) {
                         
                         preWJInfo.sbIndex=lastIndex;
                         preWJInfo.sbtype=ksbone;
                         SBInfo nextWJInfo;
                         nextWJInfo.sbIndex=baseIndex;
                         nextWJInfo.sbtype=ksbfour;
                         
                         GameIndex toIndex=AIFirstPriority::getToIndexForWJ(bm, preWJInfo, nextWJInfo);
                         if (toIndex.i!=-1) {
                             bool isRepeat=AIFirstPriority::isRepeatMove(baseIndex, toIndex,bm);
                             if (!isRepeat) {

                                 CCLog("$移动四格武将，前面有攻击阵列");
                                 rt_index.type=rt_from1to1;
                                 rt_index.to1=toIndex;
                                 rt_index.from1=baseIndex;
                                 return rt_index;
                             }
                         }

                     }
                 }
                 
                 if (baseGrid &&
                     baseGrid->getStype()==knormal &&
                     baseGrid->getSbType()==ksbtwo) {
                     GameIndex baseIndex=baseGrid->getIndex();
                     bool isNUll=AIFirstPriority::checkWJNextISNULL(bm,baseIndex);
                     if (isNUll) {
                         
                         preWJInfo.sbIndex=lastIndex;
                         preWJInfo.sbtype=ksbone;
                         SBInfo nextWJInfo;
                         nextWJInfo.sbIndex=baseIndex;
                         nextWJInfo.sbtype=ksbtwo ;
                         
                         GameIndex toIndex=AIFirstPriority::getToIndexForWJ(bm, preWJInfo, nextWJInfo);
                         if (toIndex.i!=-1) {
                             bool isRepeat=AIFirstPriority::isRepeatMove(baseIndex, toIndex,bm);
                             if (!isRepeat) {

                                 CCLog("$移动格武将，前面有攻击阵列");
                                 rt_index.type=rt_from1to1;
                                 rt_index.to1=toIndex;
                                 rt_index.from1=baseIndex;
                                 return rt_index;
                             }
                         }
                         
                     }

                     
                 }
             }
             
             
             
         }
     }
 
     
     //检查武将后面有没有武将，移动武将的情况
    for (int i=0; i<bm->fourList->count(); i++) {
        SGGridBase *sbGrid=NULL;
        
        SGSuperPrinceBase *fourSB=(SGSuperPrinceBase *)bm->fourList->objectAtIndex(i);
        sbGrid=fourSB->getNumGrid(kleftdown);
        
        
        preWJInfo.sbIndex=sbGrid->getIndex();
        preWJInfo.sbtype=ksbfour;
        
        
        SBInfo nextWJInfo=AIFirstPriority::getNextWJ(bm, preWJInfo);
        if (nextWJInfo.canMove) {
            CCLog("$武将可移动");
            GameIndex toIndex=AIFirstPriority::getToIndexForWJ(bm, preWJInfo, nextWJInfo);
            if (toIndex.i!=-1) {
                
                bool isRepeat=AIFirstPriority::isRepeatMove(nextWJInfo.sbIndex, toIndex,bm);
                if (!isRepeat) {
                    CCLog("$移动武将，前面是四格武将");
                    
                    rt_index.type=rt_from1to1;
                    rt_index.to1=toIndex;
                    rt_index.from1=nextWJInfo.sbIndex;
                    return rt_index;
                }
               
            }
            
        }
        
    }
     
     for (int i=0; i<bm->twoList->count(); i++) {
         SGGridBase *sbGrid=NULL;
         
         SGPrinceBase *twoSB=(SGPrinceBase *)bm->twoList->objectAtIndex(i);
         sbGrid=twoSB->getNumGrid(kdown);
         
         preWJInfo.sbIndex=sbGrid->getIndex();
         preWJInfo.sbtype=ksbtwo;
         
         SBInfo nextWJInfo=AIFirstPriority::getNextWJ(bm, preWJInfo);
         if (nextWJInfo.canMove) {
             GameIndex toIndex=AIFirstPriority::getToIndexForWJ(bm, preWJInfo, nextWJInfo);
             if (toIndex.i!=-1) {
                 bool isRepeat=AIFirstPriority::isRepeatMove(nextWJInfo.sbIndex, toIndex,bm);
                 if (!isRepeat) {

                     CCLog("$移动武将，前面是二格武将");
                     rt_index.type=rt_from1to1;
                     rt_index.to1=toIndex;
                     rt_index.from1=nextWJInfo.sbIndex;
                     return rt_index;
                 }
             }
             
         }
         
     }
    return  rt_index;
    
}
 

SBInfo AIFirstPriority::getNextWJ(SGBattleMap *bm, SBInfo sbInfo)
{
    SBInfo nextsbInfo;
    nextsbInfo.canMove=false;
    
    int countJ=sbInfo.sbIndex.j;
    if (sbInfo.sbtype==ksbfour) {
        countJ=sbInfo.sbIndex.j+1;
    }
    
    for (int j=sbInfo.sbIndex.j; j<=countJ; j++) {
        for (int i=mapRow-1; i>sbInfo.sbIndex.i; i--) {
            SGGridBase *baseGrid=bm->myGrids[i][j];
            if (!baseGrid) {//
                continue;
            }
            
            if (baseGrid &&
                baseGrid->getSbType()==ksbfour &&
                baseGrid->getStype()==knormal) {
                GameIndex baseIndex=AIFirstPriority::getBaseIndex(bm, baseGrid->getIndex());
                bool isNUll=AIFirstPriority::checkWJNextISNULL(bm,baseIndex);
                bool isNUll2=AIFirstPriority::checkWJNextISNULL(bm,gameIndex(baseGrid->getIndex().i, baseGrid->getIndex().j+1));
                if (isNUll && isNUll2) {
                    CCLog("$四个武将武将后面是空得");
                    sbInfo.canMove=true;
                    sbInfo.sbIndex=baseIndex;
                    sbInfo.sbtype=ksbfour;
                    return sbInfo;
                }
            } else if(baseGrid &&
                      baseGrid->getSbType()==ksbtwo &&
                      baseGrid->getStype()==knormal){
                    GameIndex baseIndex=AIFirstPriority::getBaseIndex(bm, baseGrid->getIndex());
                    bool isNUll=AIFirstPriority::checkWJNextISNULL(bm,baseIndex);
                if (isNUll) {
                    CCLog("$武将后面是空得");
                    sbInfo.canMove=true;
                    sbInfo.sbIndex=baseGrid->getIndex();
                    sbInfo.sbtype=ksbtwo;
                    return sbInfo;
                }
                
                
               
            
            }
            else
                break;
            
        }
    }
    
    
    
    CCLog("$*****************************");
    
    return nextsbInfo;
    
}
//特定函数，不可复用
GameIndex AIFirstPriority::getToIndexForWJ(SGBattleMap *bm, SBInfo preSBInfo,SBInfo nextSBInfo){
    GameIndex index;
    index.i=-1;
    index.j=-1;
    
    
    
    int noIndexj0=preSBInfo.sbIndex.j;
    int noIndexj1=-1;
    int noIndexj2=nextSBInfo.sbIndex.j;
    int noIndexj3=-1;
    
    if (preSBInfo.sbtype==ksbfour) {
        noIndexj1=preSBInfo.sbIndex.j+1;
        
    }
    if (nextSBInfo.sbtype==ksbfour) {
        noIndexj3=nextSBInfo.sbIndex.j+1;
    }
    
    int noIndexJ[10]={-1};
    for (int m=0; m<10; m++) {
        noIndexJ[m]=-1;
    }
    
    int k=0;
   
    if (bm->fourList) {
        for (int i=0; i<bm->fourList->count(); i++) {
            SGSuperPrinceBase *superBase=(SGSuperPrinceBase *)bm->fourList->objectAtIndex(i);
            SGGridBase *baseGrid=superBase->getNumGrid(kleftdown);
            noIndexJ[i*2]=baseGrid->getIndex().j;
            noIndexJ[i*2+1]=baseGrid->getIndex().j+1;
        }
    }

    
    if (bm->fourList && bm->twoList) {
        k=bm->fourList->count();
        for (int i=0; i<bm->twoList->count(); i++) {
            SGPrinceBase *superBase=(SGPrinceBase *)bm->twoList->objectAtIndex(i);
            SGGridBase *baseGrid=superBase->getNumGrid(kdown);
            noIndexJ[k*2+i]=baseGrid->getIndex().j;
        }
    }

    
    
    //移动到所有的没有武将的地方
    
        for (int i=0; i<mapRow-1; i++) {
            
            for (int j=0; j<mapList; j++) {
                if (noIndexj0==j || noIndexj1==j || noIndexj2==j || noIndexj3==j) {
                    continue;
                }
                int hasWj=false;
                
                
                
                for (int k=0; k<10; k++) {
                    if (j==noIndexJ[k]) {
                        hasWj=true;
                    }
                }
                
                
                if (hasWj) {
                    hasWj=false;
                    continue;
                }
            
            SGGridBase *baseGrid1=bm->myGrids[i][j];
            SGGridBase *baseGrid2=bm->myGrids[i][j+1];
            SGGridBase *baseGrid3=bm->myGrids[i+1][j];
            SGGridBase *baseGrid4=bm->myGrids[i+1][j+1];
            
            if (!baseGrid1 &&
                !baseGrid2 &&
                !baseGrid4 &&
                !baseGrid3 &&
                nextSBInfo.sbtype==ksbfour &&
                j<mapList-1) {
                
                bool isNull=true;
                for (int kj=j; kj<=j+1; kj++) {//后面是空
                    for (int k=mapRow-1; k>i+1; k--) {
                    SGGridBase *basegride5=bm->myGrids[k][kj];
                        if (basegride5) {
                            isNull=false;
                            break;
                        }
                    }
                }
                for (int kj=j; kj<=j+1; kj++) {//前面没有攻击阵列或武将
                    for (int k=0; k<=i+1; k++) {
                        SGGridBase *basegride5=bm->myGrids[k][kj];
                        if ((basegride5 && basegride5->getSbType()!=ksbone) ||(basegride5 && basegride5->getStype()==kattack)) {
                            isNull=false;
                            break;
                        }
                    }
                }
                
                if (isNull) {
                    return gameIndex(i, j);
                }
                
            }
            if (!baseGrid1 &&
                !baseGrid3 &&
                nextSBInfo.sbtype==ksbtwo) {
                
                bool isNull=true;
                
                    for (int k=mapRow-1; k>i+1; k--) {
                        SGGridBase *basegride5=bm->myGrids[k][j];
                        if (basegride5) {
                            isNull=false;
                            break;
                        }
                    }
                for (int k=0; k<=i+1; k++) {
                    SGGridBase *basegride5=bm->myGrids[k][j];
                    if ((basegride5 && basegride5->getSbType()!=ksbone) ||(basegride5 && basegride5->getStype()==kattack)) {
                        isNull=false;
                        break;
                    }
                }
                if (isNull) {
                    return gameIndex(i, j);

                }
                
            }
        }
        
        
        
    }
    
    
    
    return index;

}

//特定函数，不可复用
GameIndex AIFirstPriority::getBaseIndex(SGBattleMap *bm, GameIndex sbIndex){
    SGGridBase *baseGrid1=bm->myGrids[sbIndex.i][sbIndex.j];
    SGGridBase *baseGrid2 = NULL;
    SGGridBase *baseGrid3 = NULL;
    if (sbIndex.j>0) {
        baseGrid3=bm->myGrids[sbIndex.i][sbIndex.j-1];
    }
    if (sbIndex.j<mapList-1) {
        baseGrid2=bm->myGrids[sbIndex.i][sbIndex.j+1];
    }
    
    if (baseGrid2 &&
        baseGrid2->getSbType()==ksbfour &&
        baseGrid2->getStype()==knormal &&
        baseGrid2->getSaveColorID()==baseGrid1->getSaveColorID()) {
        return sbIndex;
    }
      
    if (baseGrid3 &&
        baseGrid3->getSbType()==ksbfour &&
        baseGrid3->getStype()==knormal &&
        baseGrid3->getSaveColorID()==baseGrid1->getSaveColorID()) {
        return baseGrid3->getIndex();
    }
    return sbIndex;
}

//特定的函数，不能复用
bool AIFirstPriority::checkWJNextISNULL(SGBattleMap *bm, GameIndex sbIndex){
   
    
//    for (int j=sbIndex.j; j<=sbIndex.j+1; j++) {
        for (int i=mapRow-1; i>sbIndex.i; i--) {
            SGGridBase *baseGrid=bm->myGrids[i][sbIndex.j];
            if (baseGrid) {
                return false;
            }
//        }
    }
    return true;
}
#pragma mark 为三格做准备
ReturnIndex AIFirstPriority ::fastMake3(SGBattleMap *bm)
{
    ReturnIndex index;
    index.type = rt_null;
    
    int rednum = 0;
    int greennum = 0;
    int bluenum = 0;
    
    
    for (int j = 0; j<mapList; j++)
    {
        for (int i = mapRow-1; i>=0; i--)
        {
            SGGridBase *grid = bm->myGrids[i][j];
            if (grid)
            {
                if (grid->getStype() == knormal &&
                    grid->getSbType() == ksbone)
                {
                    if (grid->getSaveColorID() == 0)
                    {
                        rednum ++;
                    }
                    if (grid->getSaveColorID() == 1)
                    {
                        greennum ++;
                    }
                    if (grid->getSaveColorID() == 2)
                    {
                        bluenum ++;
                    }
                }
                break;
            }
        }
    }
    int scolor = -1;
    if (rednum>greennum)
    {
        if (rednum>bluenum)
        {
            scolor = 0;
        }
        else
        {
            scolor = 2;
        }
    }
    else
    {
        if (greennum>bluenum)
        {
            scolor = 1;
        }
        else
        {
            scolor = 2;
        }
    }
    if (scolor !=-1)
    {
       GameIndex toIndex=AIFirstPriority::getToIndexWithSB(bm, scolor, gameIndex(-1, -1), ksbone);
        if (toIndex.i!=-1) {
            SGGridBase *delGrid=bm->myGrids[toIndex.i-1][toIndex.j];
            
            if (toIndex.i!=0) {
                if (delGrid &&
                    delGrid->getSbType()==ksbone &&
                    delGrid->getStype()==knormal &&
                    delGrid->getSaveColorID()!=scolor) {
                    index.type=rt_del1;
                    index.delIndex1=delGrid->getIndex();
                    return index;
                }
            }
            
            
            GameIndex fromIndex=AIFirstPriority::getFromIndexWithSB(bm, scolor, toIndex, ksbone);
            if (fromIndex.i!=-1) {
                bool isRepeat=AIFirstPriority::isRepeatMove(fromIndex, toIndex,bm);
                if (!isRepeat) {
                    index.type=rt_from1to1;
                    index.to1=toIndex;
                    index.from1=fromIndex;
                    
                }
            }
        }
    }
    return index;
}

#pragma mark 防止重复移动
bool AIFirstPriority::isRepeatMove(GameIndex fromIndex,GameIndex toIndex,SGBattleMap *bm){
    SGGridBase *fromGrid=bm->myGrids[fromIndex.i][fromIndex.j];
    if (!fromGrid) {//移动的格子不存在
        return true;
    }
  
    CCLog("$上一次移动到的位置是：INdexx(%d,%d)",SBMOVEGRID->getIndex().i,SBMOVEGRID->getIndex().j);
    CCLog("$本次要移动的位置是：FromIndexx(%d,%d)",fromIndex.i,fromIndex.j);
    CCLog("$本次要移动到位置是：ToIndexx(%d,%d)",toIndex.i,toIndex.j);
    if (SBMOVEGRID->getIndex().j==fromIndex.j &&
              SBMOVEGRID->getIndex().i==fromIndex.i &&
              SBMOVEGRID->getSaveColorID()==fromGrid->getSaveColorID() &&
              SBMOVEGRID->getSbType()==fromGrid->getSbType()){
        CCLog("$重复移动同一个");
        return true;
    }else
    {
        SBMOVEGRID->setIndex(toIndex);
        SBMOVEGRID->setSaveColorID(fromGrid->getSaveColorID());
        SBMOVEGRID->setSbType(fromGrid->getSbType());
        return false;
    }
    
}
