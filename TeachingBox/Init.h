/*!
 * \File Name: init.h
 * \Date: 2015/11/23 16:05
 * \Version:
 *
 * \Author: 刘巍
 * Contact: 6vwei@163.com
 *
 * \Description: 定义部分基本参数，包括示教盒大小，屏幕分辨率等
 * 
 * \Others:
 *
 * \History:
 *	  1.Date:
 *		Version:
 *		Author:
 *		Modification:
*/

#ifndef _TEACHINT_BOX_INIT_H_
#define _TEACHINT_BOX_INIT_H_

/*示教盒各部分宽高占比
  设屏幕高为20
  设屏幕宽为20
  不设值表明充满剩余控件*/
#define SCREEN_WIDTH_RATIO 20		/*屏幕宽度*/
#define SCREEN_HEIGHT_RATIO 20		/*屏幕高度*/
#define BOX_LEFT_WIDTH_RATIO 4		/*示教盒左侧宽度*/
#define BOX_RIGHT_WIDTH_RATIO 4		/*示教盒右侧宽度*/
#define BOX_TOP_HEIGHT_RATIO 2		/*示教盒上侧高度*/
#define BOX_BOTTOM_HEIGHT_RATIO 2	/*示教盒下侧高度*/

/*屏幕各部分宽高占比
  设屏幕高为20
  设屏幕宽为20
  不设值表明充满剩余控件*/
#define SCREEN_CENTER_WIDTH_RATIO 18	/*屏幕中央界面宽度*/
#define SCREEN_CENTER_HEIGHT_RATIO 15	/*屏幕中央界面高度*/
#define SCREEN_WARING_HEIGHT_RATIO 1	/*屏幕警告部分高度*/
#define SCREEN_TOP_HEIGHT_RATIO 2		/*屏幕上侧高度*/
#define SCREEN_BOTTOM_HEIGHT_RATIO 2	/*屏幕下侧高度*/
#define SCREEN_RIGHT_WIDTH_RATIO 2		/*屏幕右侧宽度*/

/*示教盒及屏幕实际宽高*/
#define SCREEN_WIDTH 800	/*屏幕实际宽度*/
#define SCREEN_HEIGHT 600	/*屏幕实际高度*/
#define BOX_WIDTH (SCREEN_WIDTH*(BOX_LEFT_WIDTH_RATIO+BOX_RIGHT_WIDTH_RATIO+SCREEN_WIDTH_RATIO)/SCREEN_WIDTH_RATIO)		/*示教盒实际宽度*/
#define BOX_HEIGHT (SCREEN_HEIGHT*(BOX_TOP_HEIGHT_RATIO+BOX_BOTTOM_HEIGHT_RATIO+SCREEN_HEIGHT_RATIO)/SCREEN_HEIGHT_RATIO)	/*示教盒实际高度*/
#define SCREEN_CENTER_WIDTH (SCREEN_WIDTH*SCREEN_CENTER_WIDTH_RATIO/SCREEN_WIDTH_RATIO)		/*屏幕中央界面实际宽度*/
#define SCREEN_CENTER_HEIGHT (SCREEN_HEIGHT*SCREEN_CENTER_HEIGHT_RATIO/SCREEN_HEIGHT_RATIO)	/*屏幕中央界面实际高度*/

/*存储路径*/
#define PROJECT_FILE_PATH (QDir::currentPath() + "/Projects")
#define SQL_PATH (QDir::currentPath() + "/Sql")
#define GRAMMAR_FILE_NAME ("E:/Project/QT/TeachPendant_qt5/VS/TeachingBox/TeachingBox/Grammar.xml")


#define VAR_NAME_MAX_LENGTH 64	/*变量名最大长度*/
#define STRING_MAX_LENGTH	128	/*字符串最大长度*/

/*用户相关*/
#define USER_NAME_MAX_LENGTH 64	/*用户名最大长度*/
#define USER_PASSWORD_MAX_LENGTH 20	/*用户密码最大长度*/
#define USER_PASSWORD_MIN_LENGTH 8  /*用户密码最小长度*/
#define USER_AUTHORITY_MAX 16	/*用户最大权限*/

#endif