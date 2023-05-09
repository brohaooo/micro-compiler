FROM ubuntu:18.04

RUN apt-get clean && apt-get update && apt-get install g++ -y && apt-get install flex bison make -y