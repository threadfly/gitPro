#!/bin/sh 
MESSAGEDIR="../protocol/protobuf/message/"
if [ ! -d $MESSAGEDIR ];then
	mkdir -p $MESSAGEDIR
fi
protoc --proto_path=../protocol/protobuf/proto  --cpp_out=../protocol/protobuf/message ../protocol/protobuf/proto/*
