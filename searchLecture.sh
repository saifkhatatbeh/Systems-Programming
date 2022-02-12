#!/bin/bash
      idTaken=$1;
      usernameTaken=$2;

      input="lectures.txt";
      while IFS= read -r line
      do
         id="$(cut -d' ' -f1 <<<"$line")";
         username="$(cut -d' ' -f4 <<<"$line")";


         if [ "$idTaken" == "$id" ] && [ "$usernameTaken" == "$username" ];
         then
               echo "Found $id" > lectureSearch.txt;

               break;
         else
               echo "NotFound $id" > lectureSearch.txt;
         fi

      done < "$input";

      id="$(cut -d' ' -f1 <<<"$line")";
      username="$(cut -d' ' -f4 <<<"$line")";

      if [ "$idTaken" == "$id" ] && [ "$usernameTaken" == "$username" ];
         then
               echo "Found" > lectureSearch.txt;

         fi
    echo "$(chmod 700 lectureSearch.txt)";
      
