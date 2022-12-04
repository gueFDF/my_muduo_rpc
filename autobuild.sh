   # 重新生成proto文件
   #src文件下
   cd src
   rm -rf ./include/rpcheader.pb.h  ./rpcheader.pb.c
   protoc --cpp_out=./. rpcheader.proto 
   mv rpcheader.pb.h  ./include
   cd ..
   #example目录下``
   cd example
   protoc --cpp_out=./   user.proto
   cd ..
   cd build
   make clean
   cmake ..
   make -j16
   cd ..

   
   cd bin
