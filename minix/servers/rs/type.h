/* Type definitions(定义) used in RS.
 */
#ifndef RS_TYPE_H
#define RS_TYPE_H

/* Definition of an entry(条目) of the boot image priv table.(引导映像priv表条目的定义。) */    
struct boot_image_priv {
  endpoint_t endpoint;         /* process endpoint number */    //进程的端点号
  char label[RS_MAX_LABEL_LEN]; /* label to assign to(分配给) this service */ //给这个服务分配的标签

  int flags;                   /* privilege(特权) flags */  //特权标志
};

/* Definition of an entry(条目) of the boot(引导) image sys table. */
struct boot_image_sys {
  endpoint_t endpoint;         /* process endpoint number */

  int flags;                   /* system flags */
};

/* Definition of an entry(条目) of the boot image dev table. */
struct boot_image_dev {
  endpoint_t endpoint;         /* process endpoint number */

  dev_t dev_nr;                /* major device number */  //主设备号
};

/* Definition of the update descriptors(描述符). */   
struct rproc;
struct rprocupd {
  int lu_flags;		                    /* user-specified live update flags */      //用户指定的实时更新标志
  int init_flags;		                  /* user-specified init flags */             //用户指定的初始化标志
  int prepare_state;                  /* the state the process has to prepare for the update */
  endpoint_t state_endpoint;          /* the custom(用户) process to transfer the state from (if any). */ 
  clock_t prepare_tm;                 /* timestamp of when the update was scheduled */    //更新计划的时间戳
  clock_t prepare_maxtime;            /* max time to wait for the process to be ready */  //等待进程准备的时间
  struct rproc *rp;                   /* the process under update */                      //正在更新的进程
  struct rs_state_data prepare_state_data; /* state data for the update */     //更新的状态数据
  cp_grant_id_t prepare_state_data_gid; /* state data gid */
  struct rprocupd *prev_rpupd;   /* the previous process under update */      //前一个正在更新的进程
  struct rprocupd *next_rpupd;   /* the next process under update */        //下一个正在更新的进程
};
struct rupdate {
  int flags;               /* flags to keep track(跟踪) of the status of the update */  //跟踪更新状态的标志
  int num_rpupds;          /* number of descriptors scheduled for the update */   //计划更新的描述符数
  int num_init_ready_pending;   /* number of pending(等待) init ready messages */       //等待初始化就绪的消息数
  struct rprocupd *curr_rpupd;  /* the current descriptor under update */         //当前正在更新的描述符
  struct rprocupd *first_rpupd; /* first descriptor scheduled for the update */   //计划更新的第一个描述符
  struct rprocupd *last_rpupd;  /* last descriptor scheduled for the update */
  struct rprocupd *vm_rpupd;    /* VM descriptor scheduled for the update */
  struct rprocupd *rs_rpupd;    /* RS descriptor scheduled for the update */
};

/* Definition of an entry(条目) of the system process table(系统进程表). */   //系统进程表的条目定义
typedef struct priv ixfer_priv_s;     // priv是在内核中定义的，这里只是声明一下
struct rproc {
  struct rprocpub *r_pub;       /* pointer to the corresponding(相应的) public entry */     //指向相应的公共条目指针
  struct rproc *r_old_rp;       /* pointer to the slot with the old version */      //指向旧版本的槽
  struct rproc *r_new_rp;       /* pointer to the slot with the new version */      //指向新版本的槽
  struct rproc *r_prev_rp;      /* pointer to the slot with the prev replica */     //指向前一个副本的槽
  struct rproc *r_next_rp;      /* pointer to the slot with the next replica */     //指向下一个副本的槽
  struct rprocupd r_upd;        /* update descriptor */
  pid_t r_pid;			            /* process id, -1 if the process is not there */

  int r_asr_count;		          /* number of live updates with ASR */               //有ASR的实时更新数
  int r_restarts;		            /* number of restarts (initially zero) */           //重启次数(初始为零)
  long r_backoff;		            /* number of periods(周期) to wait before revive */       //等待恢复的时间
  unsigned r_flags; 		        /* status and policy flags */                       //状态和策略标志  
  int r_init_err;               /* error code at initialization time */             //初始化时错误代码

  long r_period;		    /* heartbeat period (or zero) */             //心跳周期(或零)
  clock_t r_check_tm;		/* timestamp of last check */               //最后检查的时间戳
  clock_t r_alive_tm;		/* timestamp of last heartbeat */           //最后心跳的时间戳
  clock_t r_stop_tm;		/* timestamp of SIGTERM signal */           //SIGTERM信号的时间戳
  endpoint_t r_caller;		/* RS_LATEREPLY caller */                  //RS_LATEREPLY调用者
  int r_caller_request;		/* RS_LATEREPLY caller request */          //RS_LATEREPLY调用者请求

  char r_cmd[MAX_COMMAND_LEN];	/* raw command plus arguments */    
  char r_args[MAX_COMMAND_LEN];	/* null-separated raw command plus arguments */   //空分隔的原始命令和参数
#define ARGV_ELEMENTS (MAX_NR_ARGS+2) /* path, args, null */
  char *r_argv[ARGV_ELEMENTS];
  int r_argc;  			/* number of arguments */
  char r_script[MAX_SCRIPT_LEN]; /* name of the restart script executable */      //重启脚本可执行文件名

  char *r_exec;			        /* Executable image */ 
  size_t r_exec_len;		    /* Length of image */

  ixfer_priv_s r_priv;		/* Privilege(特权) structure to be passed to the kernel.*/
  uid_t r_uid;
  endpoint_t r_scheduler;	/* scheduler */   //调度程序
  int r_priority;		      /* negative values are reserved for special meanings */  //负值是特殊含义
  int r_quantum;
  int r_cpu;
  vir_bytes r_map_prealloc_addr; /* preallocated mmap address */    //预先分配的mmap地址
  size_t r_map_prealloc_len;     /* preallocated mmap len */        //预先分配的mmap长度

  /* Backup values from the privilege structure. */       //从特权结构的备份值
  struct io_range r_io_tab[NR_IO_RANGE];
  int r_nr_io_range;
  int r_irq_tab[NR_IRQ];
  int r_nr_irq;

  char r_ipc_list[MAX_IPC_LIST];
  int r_nr_control;
  char r_control[RS_NR_CONTROL][RS_MAX_LABEL_LEN];
};

#endif /* RS_TYPE_H */


