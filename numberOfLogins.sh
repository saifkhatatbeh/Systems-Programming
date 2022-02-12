#!/bin/bash
      username=$1;
      input="loggedLogins.txt";
      FILE=$(echo "$(readlink -f "loggedLogins.txt")");
      check="no";
      
       if test -f "$FILE";
         then
              while IFS= read -r line
              do
                 usernameTaken="$(cut -d',' -f1 <<<"$line")";
                 numberoFLogins="$(cut -d',' -f2 <<<"$line")";
                 numOfLoginsCAT="$(cut -d'.' -f1 <<<"$numberoFLogins")";
                 numOfLoginsCUT="$(cut -d'.' -f2 <<<"$numberoFLogins")";

                 if [ "$usernameTaken" == "$username" ];
                 then
                       addedLogin=$(($numOfLoginsCUT+1));
                       stringCAT=$numOfLoginsCAT"."$addedLogin;
                       echo "$(sed -i "s/$numberoFLogins/$stringCAT/" loggedLogins.txt)";
                       check="no";
                       break;
                 else
                      check="yes";
                 fi

         done < "$input";

         else
                stringCat=$username",NUMLOG"$username".0";
                echo "$stringCat">> loggedLogins.txt;
                echo "$(chmod 700 "loggedLogins.txt")";
                echo "hi";
         fi
 

   if [ "$check" == "yes" ];
    then
        stringCat=$username",NUMLOG"$username".0";
        echo "$stringCat">> loggedLogins.txt;
    fi