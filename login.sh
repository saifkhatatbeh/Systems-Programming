#!/bin/bash
      username=$1;
      password=$2;



      input="LoginData.txt";
      while IFS= read -r line
      do
         email="$(cut -d',' -f1 <<<"$line")";
         pass="$(cut -d',' -f2 <<<"$line")";



         if [ "$email" == "$username" ] && [ "$pass" == "$password" ];
         then
               echo "LoggedIn $username"> check.txt;

               break;
         else
               echo "InvalidLogin $username"> check.txt;
         fi

      done < "$input";

      email="$(cut -d',' -f1 <<<"$line")";
      pass="$(cut -d',' -f2 <<<"$line")";

      if [ "$email" == "$username" ] && [ "$pass" == "$password" ];
      then
            echo "LoggedIn $username"> check.txt;
      fi
      
          echo "$(chmod 700 check.txt)";

