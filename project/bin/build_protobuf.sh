#!/bin/sh 

protoc --proto_path=../protocol/protobuf/proto  --cpp_out=../protocol/protobuf/message ../protocol/protobuf/proto/*
