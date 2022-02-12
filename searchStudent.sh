#!/bin/bash
      studentID=$1;
      lectureID=$2;
      username=$3;
      input="AddedStudents.txt";


      while IFS= read -r line
      do
         studentIDCut="$(cut -d' ' -f1 <<<"$line")";
         lectureIDCut="$(cut -d' ' -f3 <<<"$line")";
         usernameCut="$(cut -d' ' -f4 <<<"$line")";
         absence="$(cut -d' ' -f5 <<<"$line")";
         absenceCAT="$(cut -d'.' -f1 <<<"$absence")";
         absenceCUT="$(cut -d'.' -f2 <<<"$absence")";

         if [ "$studentIDCut" == "$studentID" ] && [ "$lectureIDCut" == "$lectureID" ] && [ "$usernameCut" == "$username" ];
         then

               echo "Found" > studentSearch.txt;

               break;
         else
               echo "NotFound"> studentSearch.txt;

         fi

      done < "$input";

      studentIDCut="$(cut -d',' -f1 <<<"$line")";
      lectureIDCut="$(cut -d',' -f3 <<<"$line")";
      usernameCut="$(cut -d',' -f4 <<<"$line")";

      if [ "$studentIDCut" == "$studentID" ] && [ "$lectureIDCut" == "$lectureID" ] && [ "$usernameCut" == "$username" ];
      then
           echo "Found" > studentSearch.txt;
      fi
      
    echo "$(chmod 700 studentSearch.txt)";
      