.include <bsd.own.mk>
# 包含BSD系统的基础配置文件


# Makefile for Reincarnation Server (RS)
# 定义生成的程序名称为rs
PROG=	rs
# 定义源文件列表
SRCS=	exec.c main.c request.c manager.c table.c utility.c error.c update.c


# 如果USE_PCI变量不等于"no"，则添加-DUSE_PCI预处理宏，这个宏通常用于条件编译，控制是否包含与PCI相关的代码。
.if ${USE_PCI} != "no"
CPPFLAGS+= -DUSE_PCI
.endif


.if ${USE_PCI} != "no"
CPPFLAGS+= -DUSE_PCI
.endif

# 添加依赖库。${LIBSYS}和${LIBEXEC}是预定义的库变量。
DPADD+=	${LIBSYS} ${LIBEXEC}
# 链接时添加的库，在链接时添加-lsys和-lexec库。
LDADD+=	-lsys -lexec

# 添加头文件搜索路径。${NETBSDSRCDIR}是一个预定义的变量，指向NetBSD源代码目录。
CPPFLAGS+=	-I${NETBSDSRCDIR}/minix

# 包含Minix服务相关的makefile
.include <minix.service.mk>
