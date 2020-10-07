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

# install scots-ready
RUN git clone https://github.com/mkhaled87/scots-ready.git ./scots/ \
	&& cd scots-ready \
	&& make


