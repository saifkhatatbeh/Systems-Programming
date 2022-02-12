#!/bin/bash

    loginFile=$(echo "$(readlink -f "LoginData.txt")");
    lecutresFile=$(echo "$(readlink -f "lectures.txt")");
    studentsFile=$(echo "$(readlink -f "AddedStudents.txt")");

    system=$(echo "$(readlink -f "insturctorPortal.c")");
    server=$(echo "$(readlink -f "serverC.c")");

    loginScript=$(echo "$(readlink -f "login.sh")");
    backupScript=$(echo "$(readlink -f "backup.sh")");
    absenceScript=$(echo "$(readlink -f "absenceManager.sh")");
    lectureSearchScript=$(echo "$(readlink -f "searchLecture.sh")");
    studentSearchScript=$(echo "$(readlink -f "searchStudent.sh")");
    backupSearchScript=$(echo "$(readlink -f "searchBackupFiles.sh")");
    numberOfLoginsScript=$(echo "$(readlink -f "numberOfLogins.sh")");

      if test -f "$loginFile";
         then
               echo "Found" > backupCheck.txt;
         else
               echo "NotFound" > backupCheck.txt;
               exit;
         fi


      if test -f "$lecutresFile";
         then
               echo "Found" > backupCheck.txt;
         else
               echo "NotFound" > backupCheck.txt;
               exit;

         fi


      if test -f "$studentsFile";
         then
               echo "Found" > backupCheck.txt;
         else
               echo "NotFound" > backupCheck.txt;
               exit;

         fi
         
         
      if test -f "$system";
         then
               echo "Found" > backupCheck.txt;
         else
               echo "NotFound" > backupCheck.txt;
               exit;

         fi
         
         
      if test -f "$server";
         then
               echo "Found" > backupCheck.txt;
         else
               echo "NotFound" > backupCheck.txt;
               exit;

         fi
         
         
      if test -f "$loginScript";
         then
               echo "Found" > backupCheck.txt;
         else
               echo "NotFound" > backupCheck.txt;
               exit;

         fi
         
         
      if test -f "$backupScript";
         then
               echo "Found" > backupCheck.txt;
         else
               echo "NotFound" > backupCheck.txt;
               exit;

         fi
         
         
      if test -f "$absenceScript";
         then
               echo "Found" > backupCheck.txt;
         else
               echo "NotFound" > backupCheck.txt;
               exit;

         fi

 if test -f "$studentSearchScript";
         then
               echo "Found" > backupCheck.txt;
         else
               echo "NotFound" > backupCheck.txt;
               exit;

         fi
         
          if test -f "$lectureSearchScript";
         then
               echo "Found" > backupCheck.txt;
         else
               echo "NotFound" > backupCheck.txt;
               exit;

         fi
         
        if test -f "$backupSearchScript";
         then
               echo "Found" > backupCheck.txt;
         else
               echo "NotFound" > backupCheck.txt;
               exit;

         fi
         
            if test -f "$numberOfLoginsScript";
         then
               echo "Found" > backupCheck.txt;
         else
               echo "NotFound" > backupCheck.txt;
               exit;

         fi
         
    echo "$(chmod 700 backupCheck.txt)";


