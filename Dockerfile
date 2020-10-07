FROM ubuntu:18.04
MAINTAINER Mahmoud Khaled eng.mk@msn.com

# set time/language
ENV TZ=Europe/Kiev
ENV LANG C.UTF-8
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# update image
RUN apt-get -qq update \
	&& apt-get -qq install -y --no-install-recommends apt-utils \
	&& apt-get -qq -y upgrade

# install required libs/tools
RUN apt-get -qq install -y wget git unzip build-essential gcc g++ clang

# install scots-ready (latest)
RUN git clone https://github.com/mkhaled87/scots-ready.git ./scots/ \
	&& cd scots \
	&& make \
	&& mkdir -p /opt/local/include \
	&& mkdir -p /opt/local/lib \
	&& cp /scots/cudd-3.0.0/*.h* /opt/local/include/ \
	&& cp /scots/cudd-3.0.0/libcudd.a /opt/local/lib/
	

# install BDD2Implement (latest)
RUN git clone https://github.com/mkhaled87/BDD2Implement
