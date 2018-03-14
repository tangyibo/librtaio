#ifndef _ERROR_CODE_H_
#define _ERROR_CODE_H_

#define	ERR_OK								0x00000000		//操作成功
#define	ERR_FALSE							0xFFFFFFFE		//操作失败，具体错误看小错误码
#define	ERR_UNKNOWN_ERROR						0xFFFFFFFF		//未知错误 (-1)
#define	ERR_WINDOWS_MIN_ERROR						0xFFF00000		//Windows错误的起始值
#define	ERR_WINDOWS_MAX_ERROR						0xFFFF0000		//Windows错误的最大值

#define	ERR_ENCODE_ERROE						0xFFFF0001		//编包错误
#define	ERR_DECODE_ERROE						0xFFFF0002		//解包错误
#define	ERR_CHECK_CRC_ERROR						0xFFFF0003		//CRC校验错误
#define	ERR_UNEXPECTIED_METHOD						0xFFFF0004		//未知的函数方法(method)
#define	ERR_WRONG_PASSWORD						0xFFF00005		//密码错误
#define	ERR_USER_NOT_EXIST						0xFFF00006		//用户不存在
#define	ERR_USER_FULL							0xFFF00007		//用户已经满了
#define	ERR_USER_ALREADY_EXIST						0xFFF00008		//用户已经存在
#define	ERR_BATCH_END							0xFFF00009		//分段数据返回结束

#define	ERR_INVALID_HANDLE						0xFFFF0010		//无效的句柄
#define	ERR_INVALID_PARAMETER						0xFFFF0011		//无效的参数
#define	ERR_INVALID_POINT_TYPE						0xFFFF0012		//无效的标签点数据类型
#define	ERR_SEND_PACKET_ERROR						0xFFFF0013		//发送报文错误
#define	ERR_RECV_PACKET_ERROR						0xFFFF0014		//接收报文错误
#define	ERR_ACCESS_IS_DENIED						0xFFFF0015		//访问被拒绝，请查看是否有足够的权限
#define	ERR_ANOTHER_SDK_IS_RUNNING					0xFFFF0016		//另一个API函数正在被调用

#define	ERR_POINT_ALREADY_EXISTED					0xFFFF0020		//标签点已经存在
#define	ERR_TABLE_ALREADY_EXISTED					0xFFFF0021		//表已经存在
#define	ERR_POINT_NOT_EXISTED						0xFFFF0022		//标签点不存在
#define	ERR_TABLE_NOT_EXISTED						0xFFFF0023		//表不存在
#define	ERR_NO_FREE_TABLE_POS						0xFFFF0024		//没有空闲的BASE_TABLE块了
#define	ERR_NO_FREE_POINT_POS						0xFFFF0025		//没有空闲的BASE_POINT块了
#define	ERR_SYNC_DATA2FILE_ERROR					0xFFFF0026		//将数据同步到文件系统异常
#define	ERR_IPC_ACCESS_EXCEPTION					0xFFFF0027		//共享内存访问异常
#define	ERR_POINT_UPTO_FULL						0xFFFF0028		//标签点已经达到了最大数目
#define	ERR_DISABLED_FILE_NAME						0xFFFF0029		//数据文件名无效
#define	ERR_LOAD_MEM_FILE_ERROR						0xFFFF002A		//加载内存文件镜像失败
#define	ERR_OUT_OF_MEMORY						0xFFFF002B		//内存不足
#define	ERR_BIG_JOB_NOT_DONE						0xFFFF002C		//大任务工作还没有完成
#define	ERR_SNAPSHOT_SHUTTDOWN						0xFFFF002D		//实时服务正在停止
#define	ERR_INVALID_IPC_POSITION					0xFFFF002F		//无效的IPC共享内存位置
#define	ERR_WRONG_IPC_POSITION						0xFFFF0030		//错误的IPC共享内存位置
#define	ERR_INVALID_TIMESTAMP						0xFFFF0031		//无效的时间戳
#define	ERR_INVALID_POSITION_FORM_POINT_ID				0xFFFF0032		//依据标签点ID获取的位置无效
#define	ERR_INVALID_PAGE						0xFFFF0033		//无效的数据页，可能是未加载
#define	ERR_PAGE_IS_EMPTY						0xFFFF0034		//数据页的内容为空
#define	ERR_UNSUPPORT_DATA_TYPE						0xFFFF0035		//不支持的标签点数据类型
#define	ERR_ARV_PAGE_NOTA_READY						0xFFFF0036		//没有空闲的历史数据缓存页
#define	ERR_ARVEX_PAGE_NOT_READY					0xFFFF0037		//没有空闲的补历史数据缓存页
#define	ERR_INVALID_ARV_PAGE_ALLOCATE					0xFFFF0038		//刚分配的历史数据缓存页ID与标签点事件对象ID不匹配
#define	ERR_INVALID_ARVEX_PAGE_ALLOCATE					0xFFFF0039		//刚分配的补历史数据缓存页ID与标签点事件对象ID不匹配

#define	ERR__CANT_LOAD_SNAPSHOT						0xFFFF0040		//无法与SnapshotServer取得联系
#define	ERR__CANT_LOAD_BASE						0xFFFF0041		//无法与BaseServer取得联系
#define	ERR__CANT_LOAD_HISTORIAN					0xFFFF0042		//无法与HistorianServer取得联系

#define	ERR_FILE_NOT_OPENED						0xFFFF0051		//视图访问尚未打开的数据文件
#define	ERR_MAP_IS_NOT_READY						0xFFFF0052		//内存映射尚未就绪
#define	ERR_ARCHIVE_INFO_NOT_MATCH					0xFFFF0053		//历史存档文件信息头与实际不符
#define	ERR_FILE_IS_FULL						0xFFFF0054		//存档文件已满
#define	ERR_ARCHIVE_FILE_IS_TOO_SMALL					0xFFFF0055		//要创建的存档文件或附属文件太小
#define	ERR_ARCHIVE_FILE_HAS_REGISTERED					0xFFFF0056		//存档文件已经被注册了(已存在)
#define	ERR_ARCHIVE_FILE_NOT_REGISTERED					0xFFFF0057		//该存档文件还未注册(不存在)
#define	ERR_ACTIVE_ARCHIVEZ_FILE_FORBID_OPT				0xFFFF0058		//不允许操作活动存档文件
#define	ERR_NO_ARCHIVE_FILE						0xFFFF0059		//当前还没有存档文件
#define	ERR_CANT_SHIFT_TO_ACTIVE_ARCHIVE				0xFFFF005A		//找不到合适的存档文件用于切换成活动文档
#define	ERR_NEED_MORE_WRITABLE_ARCHIVE_FILE				0xFFFF005B		//当前可用的存档文件的个数太少(小于2个)
#define	ERR_INVALID_DATA_TYPE						0xFFFF005C		//无效的标签点属性
#define	ERR_INDEX_NOT_READY						0xFFFF005D		//历史存档文件对应的索引尚未就绪
#define	ERR_NO_ACTIVE_PAGE_FOR_ALLOCATOR				0xFFFF005E		//页分配器中已经没有可分配的页面了
#define	ERR_INVALID_INDEX_NODE						0xFFFF005F		//无效的索引节点
#define	ERR_INDEX_NODE_NOT_MATCH					0xFFFF0060		//索引节点与指向的内容不匹配
#define ERR_DATA_NOT_FOUND						0xFFFF0061		//没有找到数据值
#define	ERR_UPDATE_REMOVE_SANPSHOT_DENIED				0xFFFF0062		//禁止更新或删除快照值

#endif
