int sendTCP(int sock, char *msg, int sizemsg){
 int restoctets = sizemsg;
 while(restoctets != 0){
  int res = send(sock, msg+(sizemsg - restoctets), restoctets, 0);
  if (res<=0) {
      return res;
      restoctets = restoctets - res;
 }
return 1;
