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

    echo "$(mkdir backup)";
    echo "$(cp $loginFile backup)";
    echo "$(cp $lecutresFile backup)";
    echo "$(cp $studentsFile backup)";

    echo "$(cp $system backup)";
    echo "$(cp $server backup)";

    echo "$(cp $loginScript backup)";
    echo "$(cp $backupScript backup)";
    echo "$(cp $absenceScript backup)";
    echo "$(cp $lectureSearchScript backup)";
    echo "$(cp $studentSearchScript backup)";
    echo "$(cp $backupSearchScript backup)";
    echo "$(cp $numberOfLoginsScript backup)";



    FILE=$(echo "$(readlink -f "1 backup.zip")");

      if test -f "$FILE";
         then
                backupFile=$(echo "$(ls -l | grep " backup.zip" | tail -1)");
                backupFileName="$(cut -d' ' -f11 <<<"$backupFile")";
                backupFileNumber="$(cut -d' ' -f10 <<<"$backupFile")";
                addedFileNumber=$(($backupFileNumber+1));
                backupCAT=$addedFileNumber" backup.zip";
                echo "$(zip -r "$backupCAT" backup)";
                echo "$(rm -r backup)";
                echo "$(chmod 700 "$backupCAT")";

         else
                echo "$(zip -r "1 backup.zip" backup)";
                echo "$(rm -r backup)";
                echo "$(chmod 700 "1 backup.zip")";
                
         fi




