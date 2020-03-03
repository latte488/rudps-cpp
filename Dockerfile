FROM ubuntu:18.04

RUN apt-get update && apt-get install -y \
    vim \
    git \
    curl \
    build-essential \
    libssl-dev \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/* \
 && useradd -m -p docker docker

USER docker

WORKDIR /home/docker

RUN mkdir -p ~/.vim/pack/git-plugins/start \
 && git clone --depth 1 \
        https://github.com/dense-analysis/ale.git ~/.vim/pack/git-plugins/start/ale \
 && mkdir -p ~/.vim/autoload ~/.vim/bundle \
 && curl -LSso ~/.vim/autoload/pathogen.vim https://tpo.pe/pathogen.vim \
 && cd ~/.vim/bundle \
 && git clone https://github.com/dense-analysis/ale.git \
 && cd ~ \
 && git clone https://github.com/latte488/rudps-cpp.git

