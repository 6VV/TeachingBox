#ifndef _TEACHING_BOX_DATA_STRUCT_H_
#define _TEACHING_BOX_DATA_STRUCT_H_

enum CmdAttributeType
{
	CMD_ATTRIBUTE_NORMAL_COMMAND,
	CMD_ATTRIBUTE_NORMAL_COMMAND_FEEDBACK,
	CMD_ATTRIBUTE_SPECIAL_COMMAND,
	CMD_ATTRIBUTE_SPECIAL_COMMAND_FEEDBACK,
	CMD_ATTRIBUTE_ROBOT_STATE,
};

//单条指令的属性
typedef struct
{
	int m_length;
	int m_ID;//命令ID
	int m_type;	/*类型*/
	int m_LineNumber;//行号
	long long m_programAddress;	/*地址*/
	//	tTeachCmdState m_State;//命令状态
}tTeachCmdAttribute;

const int LOCK_LENGTH = 1;
const int COMMAND_HEAD_LENGTH = LOCK_LENGTH + sizeof(tTeachCmdAttribute);
const int COMMAND_MAX_LENGTH = 1024;
const int AXIS_SIZE = 6;

/*命令Id*/
enum COMMAND_ID
{
	MOVL = 2000,
	MOVX = 2001,
	MOVY = 2002,
	MOVZ = 2003,
	MOVC = 2004,
	END = 4000,

	MOVE_AXIS_PLUS_1 = 5007,
	MOVE_AXIS_PLUS_2 = 5008,
	MOVE_AXIS_PLUS_3 = 5009,
	MOVE_AXIS_PLUS_4 = 5010,
	MOVE_AXIS_PLUS_5 = 5011,
	MOVE_AXIS_PLUS_6 = 5012,
	MOVE_AXIS_PLUS_7 = 5013,
	MOVE_AXIS_PLUS_8 = 5014,
	MOVE_AXIS_PLUS_9 = 5015,
	MOVE_AXIS_PLUS_10 = 5016,

	MOVE_AXIS_MINUS_1 = 5017,
	MOVE_AXIS_MINUS_2 = 5018,
	MOVE_AXIS_MINUS_3 = 5019,
	MOVE_AXIS_MINUS_4 = 5020,
	MOVE_AXIS_MINUS_5 = 5021,
	MOVE_AXIS_MINUS_6 = 5022,
	MOVE_AXIS_MINUS_7 = 5023,
	MOVE_AXIS_MINUS_8 = 5024,
	MOVE_AXIS_MINUS_9 = 5025,
	MOVE_AXIS_MINUS_10 = 5026,

	STOP_MOVE = 5027,

	STOP_EXECUTE=5031,

	ROBOT_POSITION=6001,

	NORMAL_COMMAND_FEEDBACK = 6009,

	NORMAL_COMMAND = 9001,
};

/*位置变量*/
typedef struct
{
	double m_AxisPosition[AXIS_SIZE];

}tAxesAllPositions;

/*速度约束*/
typedef struct {
	//位置约束
	double m_Velocity;
	double m_Acceleration;
	double m_Deceleration;

	//姿态约束
	double m_PostureVelocity;
	double m_PostureAcceleration;
	double m_PostureDeceleration;
}tDynamicConstraint;

/*相邻运动指令间的过度模式*/
typedef enum
{
	kTransitionModeAbsolute = 0,
	kTransitionModeRelative = 1,

}tTransitionMode;

/*过渡区域约束*/
typedef struct {
	//过渡模式和参数
	int m_TransitionMode;	/*此处采用int而非枚举，是为了在数据库存储时方便*/
	double m_TransitionParameter;
}tOverlapConstraint;

/*三维坐标点位置*/
typedef struct
{
	double m_X;
	double m_Y;
	double m_Z;

}tPositionCartesian;

/*姿态旋转矩阵*/
typedef struct
{
	double m_R[3][3];

}tPostureRotary;

/*点的位置和姿态*/
typedef struct
{
	tPostureRotary m_PostureRotary;
	tPositionCartesian m_PositionCartesian;

}tPose;

/*位置齐次变换*/
typedef struct
{
	double m_T[4][4];

}tPoseHomogeneous;

/////////////////////////////////////////////////////
//velocity

/*6个轴的速度*/
typedef struct
{
	double m_AxisVelocity[6];

}tAxesAllVelocities;

/*三维坐标点速度*/
typedef struct
{
	double m_Vx;
	double m_Vy;
	double m_Vz;

}tVelocityCartesian;

/*速度旋转矩阵*/
typedef struct
{
	double m_R[3][3];

}tVelocityRotary;

/*点的速度和姿态速度*/
typedef struct
{
	tVelocityRotary m_PostureRotary;
	tVelocityCartesian m_PositionCartesian;

}tPoseVelocity;

/*速度齐次变换*/
typedef struct
{
	double m_T[4][4];

}tVelocityHomogeneous;


/////////////////////////////////////////////////////
//Acceleration

/*6个轴的加速度*/
typedef struct
{
	double m_AxisAcceleration[6];

}tAxesAllAccelerations;

/*三维坐标点加速度*/
typedef struct
{
	double m_Vx;
	double m_Vy;
	double m_Vz;

}tAccelerationCartesian;

/*加速度旋转矩阵*/
typedef struct
{
	double m_R[3][3];

}tAccelerationRotary;

/*点的加速度和姿态加速度*/
typedef struct
{
	tAccelerationRotary m_PostureRotary;
	tAccelerationCartesian m_PositionCartesian;

}tPoseAcceleration;

/*加速度齐次变换*/
typedef struct
{
	double m_T[4][4];

}tAccelerationHomogeneous;


/////////////////////////////////////////////////////////////////
//用户指令参数

/*直线运动规划数据结构*/
typedef struct
{
	tAxesAllPositions m_Destination;//目标位置

	tDynamicConstraint m_Dynamic;

	tOverlapConstraint m_Overlap;
}tMovLParam;


typedef struct
{
	tAxesAllPositions m_Destination;//目标位置
	tDynamicConstraint m_Dynamic;
	tOverlapConstraint m_Overlap;

}tMovXParam;

typedef struct
{
	tAxesAllPositions m_Destination;//目标位置
	tDynamicConstraint m_Dynamic;
	tOverlapConstraint m_Overlap;
}tMovYParam;

typedef struct
{
	tAxesAllPositions m_Destination;//目标位置
	tDynamicConstraint m_Dynamic;
	tOverlapConstraint m_Overlap;
}tMovZParam;



typedef enum
{
	kCircModeBorder = 0,//设定圆弧边上三点
	kCircModeCenter = 1,//设定圆心和末端点
	kCircModeRadius = 2,//设定圆弧平面、半径和末端点

}tCircMode;

typedef enum
{
	kPathChoiceClockwise = 0,//顺时针
	kPathChoioceCounterClockwise = 1,//逆时针

}tPathChoice;

typedef struct
{
	tAxesAllPositions m_AuxPoint;//参数位置
	tAxesAllPositions m_EndPoint;//目标位置
	tPathChoice m_PathChoice;//顺时针，逆时针

	tDynamicConstraint m_Dynamic;
	tOverlapConstraint m_Overlap;
}tMovCParam;


//////////////////////////////////////////////////////////
//用于通信

//typedef enum
//{
//	kTeachCmdStateWaiting = 0,
//	kTeachCmdStateRunning = 1,
//	kTeachCmdStateFinished = 2,
//	kTeachCmdStateError = 10,
//
//}tTeachCmdState;//单条指令的状态，已废弃
//
//typedef enum
//{
//	kTeachBufferStateIdle = 0,
//	kTeachBufferStateFilled = 1,//已填充
//	kTeachBufferStateRunning = 2,//正在运行
//	kTeachBufferStateHolding = 3,//暂停
//	kTeachBufferStateError = 10,
//
//}tTeachBufferState;//整个buffer的状态,已废弃


////整个buffer的属性
//typedef struct
//{
//	tTeachBufferState m_State;
//	int m_LengthAll;	//buffer的总长
//	int m_LengthEmpty;	//空的长度
//	int m_CurrentCmd;	//当前在执行的指令
//
//
//}tTeachBufferAttribute;

//示教器要设置的数据
typedef enum
{
	JLimitPos = 0,//关节角正限位
	JLimitNeg = 1,//关节角负限位
	velLimit = 2,//关节速度
	accLimit = 3,//加速度
	jerkLimit = 4,//加加速度


}tTeachSetParam;



typedef enum
{
	//TODO:待定义

}tControllerError;

typedef enum
{
	//TODO:待定义

}tTeachBoxError;



//读取数字IO
typedef struct
{
	char m_DigitalIOData;

}tReadDigitalInputParam;



//读取模拟IO
typedef struct
{
	double m_AnalogIOData;

}tReadAnalogInputParam;


typedef enum
{
	kRobotStateDisabled = 0,
	kRobotStateStandby = 1,
	kRobotStateMoving = 2,
	kRobotStateStopping = 4,
	kRobotStateErrorStop = 5,
	kRobotStateHoming = 6,

}tRobotState;


//机器人当前状态信息
typedef struct
{
	tRobotState m_RobotState;

}tRobotStateInfo;


#endif