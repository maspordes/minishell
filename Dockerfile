# 使用Ubuntu作为基础镜像
FROM ubuntu:22.04

# 设置环境变量避免交互式安装
ENV DEBIAN_FRONTEND=noninteractive

# 安装必要的工具和库
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    libreadline-dev \
    libc6-dev \
    valgrind \
    gdb \
    strace \
    vim \
    git \
    && rm -rf /var/lib/apt/lists/*

# 设置工作目录
WORKDIR /minishell

# 设置用户和权限
RUN useradd -m -s /bin/bash testuser
RUN chown -R testuser:testuser /minishell

# 切换到普通用户
USER testuser

# 设置shell环境
ENV SHELL=/bin/bash
ENV TERM=xterm-256color

# 默认命令
CMD ["/bin/bash"] 