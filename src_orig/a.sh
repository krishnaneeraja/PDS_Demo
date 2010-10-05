  925  curl -F curl -F password=@/etc/passwd http://192.168.1.25/AMS/uploadfile.aspx?userid=3&password=padma&date=
  926  curl -F password=@/etc/passwd http://192.168.1.25/AMS/uploadfile.aspx?userid=3&password=padma&date=
  927  curl -F FileUpload1=@/etc/passwd http://192.168.1.25/AMS/uploadfile.aspx?userid=3&password=padma&date=
  928  curl -F FileUpload1=@/etc/passwd "http://192.168.1.25/AMS/uploadfile.aspx?userid=3&password=padma&date="
  929  curl -F FileUpload1=@/etc/passwd http://192.168.1.25/AMS/uploadfile.aspx?userid=3\&password=padma\&date=
  931  curl -F FileUpload1=@/etc/passwd press=BtnUpload http://192.168.1.25/AMS/uploadfile.aspx?userid=3\&password=padma\&date=
  932  curl -F FileUpload1=@/etc/passwd http://192.168.1.25/AMS/uploadfile.aspx?userid=3\&password=padma\&date=
  933  curl -F FileUpload1=@/etc/passwd http://192.168.1.25/AMS/uploadfile.aspx?userid=3&password=padma&date=
  934  curl -F FileUpload1=@/etc/passwd http://192.168.1.25/AMS/uploadfile.aspx?userid=3\&password=padma\&date=
  935  curl -F FileUpload1=@/etc/passwd http://192.168.1.25/AMS/uploadfile.aspx?userid=3\&password=padma\&date=
  936  curl -F FileUpload1=@/etc/passwd press=Upload http://192.168.1.25/AMS/uploadfile.aspx?userid=3\&password=padma\&date=
  937  curl -F FileUpload1=@/etc/passwd http://192.168.1.25/AMS/uploadfile.aspx?userid=3\&password=padma\&date=
  939  curl -F "file=@/etc/passwd;filename=FileUpload1;userid=3;password=padma;date=" http://192.168.1.25/AMS/uploadfile.aspx
  940  curl -F "file=@/etc/passwd;filename=FileUpload1;userid=3;password=padma;date=" http://192.168.1.25/AMS/uploadfile.aspx
  941  curl -F "file=@/etc/passwd;filename=FileUpload1;userid=3;password=padma;date=" http://192.168.1.25/AMS/uploadfile.aspx
  942  curl -F "file=@/etc/passwd;filename=FileUpload1" -d "userid=3&password=padma&date=" http://192.168.1.25/AMS/uploadfile.aspx
  943  curl -F "file=@/etc/passwd;filename=FileUpload1" -F "userid=3" -F "password=padma" -F "date=" http://192.168.1.25/AMS/uploadfile.aspx
 1009   curl -F "file=@/etc/passwd;filename=FileUpload1" -F "userid=3" -F "password=padma" -F "date=" http://192.168.1.25/AMS/uploadfile.aspx
 1010   curl -F FileUpload1=@/etc/passwd -F "userid=3" -F "password=padma" -F "date=" http://192.168.1.25/AMS/uploadfile.aspx
 1011   curl -F "FileUpload1=@/etc/passwd" -F "userid=3" -F "password=padma" -F "date=" http://192.168.1.25/AMS/uploadfile.aspx
 1012  curl -F FileUpload1=@/etc/passwd http://192.168.1.25/AMS/uploadfile.aspx?userid=3\&password=padma\&date=
 1013   curl -F "FileUpload1=@/etc/passwd" -F "userid=3" -F "password=padma" -F "date=" http://192.168.1.25/AMS/uploadfile.aspx
 1014   curl -F "FileUpload1=@/etc/passwd" -F "userid=3" -F "password=padma" -F "date=20100117" http://192.168.1.25/AMS/uploadfile.aspx
 1015   curl -v -F "FileUpload1=@/etc/passwd" -F "userid=3" -F "password=padma" -F "date=20100117" http://192.168.1.25/AMS/uploadfile.aspx
 1017   curl -v -F "FileUpload1=@/etc/passwd;filename=passwd" -F "userid=3" -F "password=padma" -F "date=20100117" http://192.168.1.25/AMS/uploadfile.aspx
 1018   curl -v -F "FileUpload1=@/etc/passwd;filename=passwd" http://192.168.1.25/AMS/uploadfile.aspx
 1019   curl -v -F "FileUpload1=@/etc/passwd;filename=passwd" 192.168.1.25/AMS/uploadfile.aspx
 1021   curl -v -F "FileUpload1=@/etc/passwd;filename=passwd" -F "BtnUpload=Upload" -F "userid=3" -F "password=padma" -F "date=20100117" http://192.168.1.25/AMS/uploadfile.aspx
 1022   curl -v -F "FileUpload1=@/etc/passwd" -F "BtnUpload=Upload" -F "userid=3" -F "password=padma" -F "date=20100117" http://192.168.1.25/AMS/uploadfile.aspx
 1023   curl -v -F "FileUpload1=@/etc/passwd&BtnUpload=Upload&userid=3&password=padma&date=20100117" http://192.168.1.25/AMS/uploadfile.aspx
 1024   curl -v -F "FileUpload1=@/etc/passwd" -F "BtnUpload=Upload&userid=3&password=padma&date=20100117" http://192.168.1.25/AMS/uploadfile.aspx
 1026  ./upload /etc/passwd
 1030  ./upload /etc/passwd
 1033  ./upload /etc/passwd
 1035  ./upload /etc/passwd
 1038   curl -v -F "FileUpload=@/etc/passwd" -F "BtnUpload=Upload&userid=3&password=padma&date=20100117" http://192.168.1.25/AMS/uploadfile.aspx
 1047  history|grep passwd
 1048   curl -F "file=@/etc/passwd;filename=FileUpload1" -F "userid=3" -F "password=padma" -F "date=" http://192.168.1.25/AMS/uploadfile.aspx
 1049  curl -F "file=@/etc/passwd;filename=FileUpload1" -F "userid=3" -F "password=padma" -F "date=" http://192.168.1.25/AMS/uploadfile.aspx
 1050  curl -F "file=@/etc/passwd;filename=FileUpload1;userid=3;password=padma;date=" http://192.168.1.25/AMS/uploadfile.aspx
 1051   curl -v -F "FileUpload1=@/etc/passwd;filename=passwd" -F "BtnUpload=Upload" -F "userid=3" -F "password=padma" -F "date=20100117" http://192.168.1.25/AMS/uploadfile.aspx
 1052  ./upload /etc/passwd
 1053   curl -v -F "FileUpload1=@/etc/passwd" -F "BtnUpload=Upload&userid=3&password=padma&date=20100117" http://192.168.1.25/AMS/uploadfile.aspx
 1054   curl -v -F "FileUpload=@/etc/passwd" -F "BtnUpload=Upload&userid=3&password=padma&date=20100117" http://192.168.1.25/AMS/uploadfile.aspx
 1056  curl -F FileUpload1=@/etc/passwd http://192.168.1.25/AMS/uploadfile.aspx?userid=3\&password=padma\&date=
 1059  history|grep passwd > a.sh
