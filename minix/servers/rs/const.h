/* Global constants(常量) used in RS.*/

#ifndef RS_CONST_H
#define RS_CONST_H

#define DEBUG_DEFAULT      0
#define PRIV_DEBUG_DEFAULT 0

#ifndef DEBUG
#define DEBUG DEBUG_DEFAULT
#endif

#ifndef PRIV_DEBUG
#define PRIV_DEBUG PRIV_DEBUG_DEFAULT
#endif

/* Space reserved for program and arguments.(为程序和参数保留的空间。) */
#define MAX_COMMAND_LEN     512         /* maximum argument string length(最大参数字符串长度) */
#define MAX_SCRIPT_LEN      256         /* maximum restart script name length(最大重启脚本名称长度) */
#define MAX_NR_ARGS          10         /* maximum number of arguments */

#define MAX_IPC_LIST        256         /* Max size of list for IPC target
                                         * process names
                                         */
#define MAX_DET_RESTART      10         /* maximum number of detached restarts(分离重启的最大次数). */

/* Flag values.(标记值) */
#define RS_IN_USE       0x001    /* set when process slot(进程槽) is in use */
#define RS_EXITING      0x002    /* set when exit is expected(期待/预计退出) */
#define RS_REFRESHING   0x004    /* set when refresh(刷新) must be done(设置必须进行刷新的时间) */
#define RS_NOPINGREPLY  0x008    /* service failed to reply(回复) to a ping request(服务未能回复ping请求) */
#define RS_TERMINATED   0x010    /* service has terminated(终止) */
#define RS_LATEREPLY    0x020    /* no reply(回复) sent to RS_DOWN caller yet */
#define RS_INITIALIZING 0x040    /* set when init is in progress */
#define RS_UPDATING     0x080    /* set when update is in progress */
#define RS_PREPARE_DONE 0x100    /* set when updating and preparation(准备) is done */
#define RS_INIT_DONE    0x200    /* set when updating and init is done */
#define RS_INIT_PENDING 0x400    /* set when updating and init is pending(待定；待决；悬而未决的) */
#define RS_ACTIVE       0x800    /* set for the active instance of a service */
#define RS_DEAD         0x1000   /* set for an instance ready to be cleaned up(清理；清除) */
#define RS_CLEANUP_DETACH 0x2000   /* detach at cleanup time(在清理时分离) */
#define RS_CLEANUP_SCRIPT 0x4000   /* run script at cleanup time(清理时间) (REINCARNATE:使再生) */
#define RS_REINCARNATE    0x8000   /* after exit, restart with a new endpoint(退出后，使用新端点重新启动) */

#define RS_SRV_IS_IDLE(S) (((S)->r_flags & RS_DEAD) || ((S)->r_flags & ~(RS_IN_USE|RS_ACTIVE|RS_CLEANUP_DETACH|RS_CLEANUP_SCRIPT)) == 0)

/* Constants determining(确定) RS period and binary exponential backoff.(确定RS周期和二进制指数退避的常数。) */
#define RS_INIT_T	(system_hz * 10)	/* allow T ticks for init */
#define RS_DELTA_T	(system_hz)		/* check every T ticks */
#define BACKOFF_BITS    (sizeof(long)*8)        /* bits in backoff field(退避字段中的位) */
#define MAX_BACKOFF      30                     /* max backoff in RS_DELTA_T */

/* Magic process table addresses.(魔术进程表地址) */
#define BEG_RPROC_ADDR  (&rproc[0])
#define END_RPROC_ADDR  (&rproc[NR_SYS_PROCS])

/* Constants for live update.(实时更新的常量) */
#define RS_DEFAULT_PREPARE_MAXTIME 2*RS_DELTA_T   /* default prepare(准备) max time(默认最大准备时间) */

/* Definitions for boot info tables.(启动信息表的定义) */
#define NULL_BOOT_NR    NR_BOOT_PROCS        /* marks a null boot entry(标记一个空启动项) */
#define DEFAULT_BOOT_NR NR_BOOT_PROCS        /* marks the default boot entry(标记默认启动项) */

/* Define sys flags for the various process types. */
#define SRV_SF   (SF_CORE_SRV)                 /* system services */
#define SRVR_SF  (SRV_SF | SF_NEED_REPL)       /* services needing a replica(需要副本的服务) (replica:复制品)*/
#define DSRV_SF  (0)                           /* dynamic system services(动态系统服务) */
#define VM_SF    (SRVR_SF)     			/* vm */

/* Shorthands.(速记(法)；(对某事)简略的表达方式) */
#define SRV_OR_USR(rp, X, Y) (rp->r_priv.s_flags & SYS_PROC ? X : Y)      /*位与 AND优先级高于条件运算符*/

/* Reply flags. */
#define RS_DONTREPLY    0    /* don't reply */
#define RS_REPLY        1   /* reply */
#define RS_CANCEL       2   /* cancel */

/* Swap flags. */
#define RS_DONTSWAP     0   /* don't swap */
#define RS_SWAP         1   /* swap */

/* Configuration constants */
#define RS_VM_DEFAULT_MAP_PREALLOC_LEN  (1024*1024*8)    /* default preallocation length for VM mappings(VM映射的默认预分配长度) */
#define RS_USE_PAGING                   0               //是否使用分页机制

/* Update macros. */    //实时更新宏
#define RUPDATE_INIT() memset(&rupdate, 0, sizeof(rupdate))
#define RUPDATE_CLEAR() RUPDATE_INIT()

/* Note that we have 'B' last in order to allow 'continue' statements */    //注意，我们将'B'放在最后，以允许'continue'语句
#define RUPDATE_ITER(HEAD, RPUPD_PREV, RPUPD, B) do { \
        for(RPUPD = HEAD, RPUPD_PREV = NULL; RPUPD != NULL; \
          RPUPD_PREV = RPUPD, RPUPD = RPUPD->next_rpupd) { \
            B \
        } \
     } while(0)
#define RUPDATE_REV_ITER(TAIL, RPUPD_PREV, RPUPD, B) do { \
        RPUPD = TAIL; \
        for(RPUPD = TAIL; RPUPD != NULL; RPUPD = RPUPD->prev_rpupd) { \
            RPUPD_PREV = RPUPD->prev_rpupd; \
            B \
        } \
     } while(0)


#define RUPDATE_IS_UPDATING() (rupdate.flags & RS_UPDATING)
#define RUPDATE_IS_VM_UPDATING() ((rupdate.flags & RS_UPDATING) && rupdate.vm_rpupd)
#define RUPDATE_IS_VM_INIT_DONE() (rproc_ptr[_ENDPOINT_P(VM_PROC_NR)]->r_flags & RS_INIT_DONE)
#define RUPDATE_IS_RS_UPDATING() ((rupdate.flags & RS_UPDATING) && rupdate.rs_rpupd)
#define RUPDATE_IS_RS_INIT_DONE() (rproc_ptr[_ENDPOINT_P(RS_PROC_NR)]->r_flags & RS_INIT_DONE)
#define RUPDATE_IS_INITIALIZING() (rupdate.flags & RS_INITIALIZING)
#define RUPDATE_IS_UPD_SCHEDULED() (rupdate.num_rpupds > 0 && !RUPDATE_IS_UPDATING())
#define RUPDATE_IS_UPD_MULTI() (rupdate.num_rpupds > 1)
#define RUPDATE_IS_UPD_VM_MULTI() (rupdate.vm_rpupd && RUPDATE_IS_UPD_MULTI())
#define SRV_IS_UPDATING(RP) ((RP)->r_flags & RS_UPDATING)
#define SRV_IS_UPDATING_AND_INITIALIZING(RP) (((RP)->r_flags & (RS_UPDATING|RS_INITIALIZING)) == (RS_UPDATING|RS_INITIALIZING))
#define UPD_INIT_MAXTIME(RPUPD) ((RPUPD)->prepare_maxtime != RS_DEFAULT_PREPARE_MAXTIME ? (RPUPD)->prepare_maxtime : RS_INIT_T)
#define UPD_IS_PREPARING_ONLY(RPUPD) ((RPUPD)->lu_flags & SEF_LU_PREPARE_ONLY)
#define SRV_IS_PREPARING_ONLY(RP) ((RP)->r_upd.rp && UPD_IS_PREPARING_ONLY(&(RP)->r_upd))
#define UPD_IS_UPD_SCHEDULED(RPUPD) (RUPDATE_IS_UPD_SCHEDULED() && (RPUPD)->rp)
#define SRV_IS_UPD_SCHEDULED(RP) UPD_IS_UPD_SCHEDULED(&(RP)->r_upd)

#endif /* RS_CONST_H */

