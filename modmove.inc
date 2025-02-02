#Event GuildPortal "The Guildhall Portal has been aligned#*#"

Sub Event_GuildPortal
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}GuildPortal Event
/return


Sub LoadAMINIVals
   /call LoadINIVar Settings MBWayPntLocation "MBWayPnt.ini" MBWayPnt "${MBCommon}" string
   /if (!${MBWayPnt.Find[/]} && !${MBWayPnt.Find[\]}) /varset MBWayPnt ${IniLocation}${MBWayPnt}
   /declare PathLoc int outer
/return

| Note: Added 'nolook' to most of the /face commands.
|       A Change was made to /face to pause macro execution until the command was completed.
|       However, if AdvPath is currently in Following mode, this causes the /face command to pause the macro forever.
|       Adding 'nolook' gets aroud this.
| Additional note: /face heading xx does not have this problem but nolook doesn't hurt. 

Sub MBZone(tZoneName)
   /declare mbDoorList string local NIHALTAR801,TMTELEPORT800,CT_Crystal,VTDOOR302,NIHSTATUE800,NIHOBELISK800,POJDR501,HOHTESWTCH500,POTRANCOD500,POILAMP500,BOTPORT500,SRTGRUNE504,POTACPORTL500,HOHTESWITCH500
   /declare tZoneTimer timer local
   /declare tZoneTurn int local
   /declare tZoneTF bool local FALSE
   /declare tZoneID int local ${Zone.ID}
   /declare i int local
   /if (${Spawn[pc =${FollowName}].ID} || !${FollowName.Length}) /return
   /squelch /doortarget
   /if (${NearestSpawn[Priest_Of_Discord npc los radius 40].ID} || (${Zone.ID}==345 && ${DoorTarget.Distance3D}>90) || (${tZoneName.Length} && (${Zone.ID}==344 && ${DoorTarget.Distance3D}>40 && ${NearestSpawn[Magus npc radius 100].ID}) || (${Zone.ID}!=344 && ${NearestSpawn[Magus npc radius 100].ID}))) {
| lobby = 344, hall 345
      /if (${Zone.ID}==345) {
         /moveto loc -23 -134 mdist 10
         /delay 1s
:waitforwindow 
         /doevents GuildPortal
         /if (!${Window[LargeDialogWindow].Open}) { 
            /delay 5s ${Window[LargeDialogWindow].Open} 
            /goto :waitforwindow 
         } 
         /if (${Window[LargeDialogWindow].Open}==0) /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Specified window not detected open 
         /nomodkey /notify LargeDialogWindow LDW_YesButton leftmouseup 
      } else /if (${NearestSpawn[Priest_Of_Discord npc radius 100].ID}) {
         /squelch /tar Priest_Of_Discord npc 
         /delay 5
         /squelch /face nolook ${If[${MacroQuest.BuildName.Equal[emu]},fast ,]}
         /delay ${Math.Rand[20,50]}
         /say ${If[${Zone.ID}==302,go home,wish to go]} 
         /delay 5s
      } else /if (${NearestSpawn[Magus npc radius 100].ID}) {
         /squelch /tar magus npc
         /delay 5
         /squelch /face nolook ${If[${MacroQuest.BuildName.Equal[emu]},fast ,]}
         /delay ${Math.Rand[20,50]}
         /if (${Select[${tZoneName},butcher,butcherblock]}) {
            /say butcherblock
         } else /if (${Select[${tZoneName},nedaria]}) {
            /say nedaria
         } else /if (${Select[${tZoneName},southro,south ro,sro]}) {
            /say south ro
         } else /if (${Select[${tZoneName},northro,north ro,nro]}) {
            /say north ro
         } else /if (${Select[${tZoneName},everfrost]}) {
            /say everfrost
         } else /if (${Select[${tZoneName},ecommonlands,east commonlands,ecommons]}) {
            /say commonlands
         } else /if (${Select[${tZoneName},natambi]}) {
            /say natambi
         } else /if (${Select[${tZoneName},abysmal]}) {
            /say abysmal sea
         }
         /delay 5s
      }
   } else /if (${Zone.ID}==219 || ${Zone.ID}==202 && ${NearestSpawn[Herald_of_Druzzil npc los radius 40].ID} && ${Spawn[Herald_of_Druzzil npc].Distance}<40) {
:TargCheck
      /if (!${Me.ID} || !${ZoneName.Equal[${Zone.Name}]} || ${Spawn[pc =${FollowName}].ID}) /return
      /if (${Spawn[Herald_of_Druzzil npc].Distance}>39) /goto :NoMagusOrHoD
      /if (${Zone.ID}==219 && ${Spawn[Herald_of_Druzzil npc].Distance}<40) {
         /squelch /target id ${Spawn[Herald_of_Druzzil npc los radius 40].ID}
         /delay 1s ${Target.ID}==${Spawn[Herald_of_Druzzil npc los radius 40].ID}
         /delay ${Math.Calc[${Math.Rand[15]}+10]}
         /if (${Target.ID}!=${Spawn[Herald_of_Druzzil npc los radius 40].ID}) /goto :TargCheck
         /say send
         /delay 2s !${Me.ID}
         /return
      }
      /if (${Zone.ID}==202 && ${Spawn[Herald_of_Druzzil npc].Distance}<40) {
         /squelch /target id ${Spawn[Herald_of_Druzzil npc los radius 40].ID}
         /delay 1s ${Target.ID}==${Spawn[Herald_of_Druzzil npc los radius 40].ID}
         /delay ${Math.Calc[${Math.Rand[15]}+10]}
         /if (${Target.ID}!=${Spawn[Herald_of_Druzzil npc los radius 40].ID}) /goto :TargCheck
         /say time
         /delay 2s !${Me.ID}
         /return
      }
   } else {
:NoMagusOrHoD
      /if (!${Me.ID} || !${ZoneName.Equal[${Zone.Name}]} || ${Zone.ID} != ${tZoneID}) /return
      /delay ${Math.Rand[10]}S
      /varset tZoneTimer 15s
      /squelch /doortarget
      /delay 1s ${DoorTarget.Name.Length}
      /if (${DoorTarget.Distance}>100) {
         /nomodkey /keypress up hold
         /for i 1 to 4
            /delay 1s !${Me.ID}
            /nomodkey /keypress up
            /if (!${Me.ID} || !${ZoneName.Equal[${Zone.Name}]} || ${Zone.ID} != ${tZoneID}) /return
         /next i
      }
      /squelch /face nolook ${If[${MacroQuest.BuildName.Equal[emu]},fast ,]}door
      /if (${DoorTarget.Distance}<=70 && (${Select[${DoorTarget.Name},${mbDoorList}]} || ${DoorTarget.Name.Find[mazedoor]} || ${DoorTarget.Name.Find[cave]} || ${DoorTarget.Name.Find[_crystal]} || ${DoorTarget.Name.Find[trans_]} || ${DoorTarget.Name.Find[_book_]} || ${DoorTarget.Name.Find[crate]} || ${DoorTarget.Name.Find[pontree]} || ${DoorTarget.Name.Find[zonebox]} || ${DoorTarget.Name.Find[potran]} || ${DoorTarget.Name.Find[port]} || ${DoorTarget.Name.Find[rock]} || ${DoorTarget.Name.Find[tele]} || ${DoorTarget.Name.Find[prtl]} || ${DoorTarget.Name.Find[potrhut]} || ${DoorTarget.Name.Find[door]} || ${DoorTarget.Name.Find[grave]} || ${DoorTarget.Name.Find[switch]} || ${DoorTarget.Name.Find[EKTOWER]} || ${DoorTarget.Name.Find[FACADE]}|| ${DoorTarget.Name.Find[Guild]} || ${DoorTarget.Distance}<=20)) {
:tMoveToDoor
         /if (!${Me.ID} || !${ZoneName.Equal[${Zone.Name}]} || ${Zone.ID} != ${tZoneID}) /return
         /if (${tZoneID}==${Zone.ID}) {
            /doevents DelayOnZone
            /call MBDoor
            /if (!${Me.ID} || !${ZoneName.Equal[${Zone.Name}]} || ${Zone.ID} != ${tZoneID} || ${Spawn[pc =${FollowName}].ID} || ${Zone.ID} != ${tZoneID}) /return TRUE
            /delay 3
            /if (${Window[LargeDialogWindow].Open}) /nomodkey /notify LargeDialogWindow LDW_YesButton leftmouseup
            /delay 4s !${Window[LargeDialogWindow].Open}
            /doevents DelayOnZone
            /if (${tZoneID}!=${Zone.ID}) /return TRUE
            /if (${DoorTarget.Distance}>15) {
               /nomodkey /keypress forward hold
               /delay 3
               /nomodkey /keypress forward
            } else /if (${DoorTarget.Distance}<10) {
               /nomodkey /keypress back hold   
               /delay 1
               /nomodkey /keypress back
            } else /if (${tZoneTimer}<50) {
               /if (${tZoneTurn}) {
                  /nomodkey /keypress strafe_left hold
                  /delay 2
                  /nomodkey /keypress strafe_left
                  /varset tZoneTurn 0
               } else {
                  /nomodkey /keypress strafe_right hold
                  /delay 2
                  /nomodkey /keypress strafe_right
                  /varset tZoneTurn 1
               }
            }
            /if (${Target.ID}) /squelch /tar clear
            /squelch /doortarget
            /squelch /face nolook ${If[${MacroQuest.BuildName.Equal[emu]},fast ,]}door
|           /nomodkey /keypress home
            /delay 1
            /nomodkey /keypress u hold
            /delay 2
            /nomodkey /keypress u
            /delay 2s
            /if (${tZoneTimer}) /goto :tMoveToDoor
         }
      } else {
:tNotaDoor
         /if (!${Me.ID} || !${ZoneName.Equal[${Zone.Name}]} || ${Zone.ID} != ${tZoneID} || ${Spawn[pc =${FollowName}].ID} || ${Zone.ID} != ${tZoneID}) /return TRUE
         /if (${Select[${tZoneTurn},0,2,4]}) {
            /nomodkey /keypress forward hold
            /delay 4s !${Me.ID} || !${ZoneName.Equal[${Zone.Name}]} || ${Zone.ID} != ${tZoneID} || ${Spawn[pc =${FollowName}].ID} || ${Zone.ID} != ${tZoneID}
            /nomodkey /keypress forward
         }
         /if (${Select[${tZoneTurn},1,3]} && !${Zone.ShortName.Equal[shadowhaven]}) {
            /nomodkey /keypress back hold
            /delay 8
            /nomodkey /keypress back
            /if (${tZoneTurn}==1) {
               /nomodkey /keypress left hold
               /delay 5
               /nomodkey /keypress left
            } else /if (${tZoneTurn}==3) {
               /nomodkey /keypress right hold
               /delay 9
               /nomodkey /keypress right
            }
         }
         /varcalc tZoneTurn ${tZoneTurn}+1
         /if (${tZoneTurn}>=5) /varset tZoneTimer 0
         /if (${tZoneTimer}) /goto :tNotaDoor
      }
   }
   /delay 5s
   /doevents DelayOnZone
   /docommand ${If[${tZoneID}==${Zone.ID},/varset tZoneTF FALSE,/varset tZoneTF TRUE]}
/return ${tZoneTF}


Sub MBMoveTo(MBy,MBx,int MBp,int MBt)
   /declare tDistThen float local
   /declare tDistNow float local
   /declare tMoveCheck timer local 22
   /declare tMoveTimer timer local 2m
   /declare tMB string local FALSE
   /declare tZoneID int local ${Zone.ID}
   /declare tDoor int local 0
   
   /varset tDistThen ${Math.Distance[${Me.Y},${Me.X}:${MBy},${MBx}]}
   /if (${MBt}) {
      /varset tMoveTimer ${MBt}
   }
   /squelch /stick off
   /squelch /moveto unpause
   /squelch /moveto loc ${MBy} ${MBx} mdist 10
   
   /while (${tMoveTimer}) {
      /doevents ImHit
      /if (!${tMoveCheck}) {
         /if (${APAggro} && !${MBp}) {
            /varset tMB TRUE
            /break
         }
         /varset tDistNow ${Math.Distance[${Me.Y},${Me.X}:${MBy},${MBx}]}
         /doevents
         /if (${tZoneID}!=${Zone.ID}) { 
            /varset tMB ZONED
            /break
         }
         /if (${tDistThen}-${tDistNow}<2 && ${Me.Moving} && !${Me.Stunned}) {
            /nomodkey /keypress forward
            /nomodkey /keypress back hold
            /delay 5
            /nomodkey /keypress back
            /if (${DoorTarget.Distance} && ${DoorTarget.Distance}<15 && !${tDoor}) {
               /call MBDoor
               /if (!${Me.ID} || ${ZoneName.NotEqual[${Zone.Name}]}) /break
               /if (!${Macro.Return}) {
                  /squelch /tar clear
                  /nomodkey /keypress forward hold
                  /delay 2
                  /nomodkey /keypress forward
                  /call MBDoor
               }
               /varset tDoor 1
            } else {
               /nomodkey /keypress ${If[${Math.Rand[99]}>50,strafe_right,strafe_left]} hold
               /delay 1s
               /nomodkey /keypress strafe_right
               /nomodkey /keypress strafe_left
            }
            /delay 1s !${Me.Moving}
         }
         /if (!${Me.Moving}) {
            /squelch /moveto loc ${MBy} ${MBx} mdist 10
            /nomodkey /keypress jump
         }
         /varset tDistThen ${Math.Distance[${Me.Y},${Me.X}:${MBy},${MBx}]}
         /varset tDoor 0
         /varset tMoveCheck 15
      }
      /delay 2
      /if (${Select[${MoveTo},off]}) /break
   }
   /if (!${Select[${MoveTo},off]}) {
      /squelch /moveto off
   }
   /if (${tMB.Equal[FALSE]}) {
      /if (${Math.Distance[${Me.Y},${Me.X}:${MBy},${MBx}]}<10) {
         /varset tMB TRUE
      }
   }
/return ${tMB}


Sub MBWayRecord(Section)
   /if (!${Section.Length}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Syntax Error - mbwayplay <path name>
      /return
   }
   /declare WPCount int local 0
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Entering MBWayRecord - Must endmacro or sit to eject from loop
:tMBRTop
   /if (${Me.State.Equal[duck]}) {
      /varcalc WPCount ${WPCount}+1
      /ini "${MBWayPnt}" "${Section}" "WPCount" "${WPCount}"
      /ini "${MBWayPnt}" "${Section}" "WP${WPCount}" "${Me.Y} ${Me.X}"
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}WayPoint[${WPCount}] ${Me.Y} ${Me.X}
      /nomodkey /keypress duck
   }
   /if (${Me.State.Equal[sit]}) /return
   /goto :tMBRTop
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Exiting MBWAyRecord
/return


Sub MBDoor
   /declare tDoorTimer timer local 8s
   /declare tMB bool local FALSE
   /declare tZoneID int local ${Zone.ID}
:tTop
   /if (!${FollowName.Length} || ${Spawn[=${FollowName}].Distance} < 70 && ${Me.Moving}) /return
   /if (!${Me.ID} || !${ZoneName.Equal[${Zone.Name}]} || ${Zone.ID} != ${tZoneID}) /return
   /nomodkey /keypress home
   /nomodkey /keypress u
   /if (${Target.ID}) /squelch /tar clear
   /squelch /doortarget
   /squelch /face nolook ${If[${MacroQuest.BuildName.Equal[emu]},fast ,]}door
   /look -15
   /nomodkey /keypress u
   /if (${Target.ID}) /squelch /tar clear
   /if (!${Me.ID} || !${ZoneName.Equal[${Zone.Name}]} || ${Zone.ID} != ${tZoneID}) /return
   /squelch /doortarget
   /squelch /face nolook ${If[${MacroQuest.BuildName.Equal[emu]},fast ,]}door
   /look -25
   /nomodkey /keypress u
   /if (${Target.ID}) /squelch /tar clear
   /if (!${Me.ID} || !${ZoneName.Equal[${Zone.Name}]} || ${Zone.ID} != ${tZoneID}) /return
   /squelch /doortarget
   /squelch /face nolook ${If[${MacroQuest.BuildName.Equal[emu]},fast ,]}door
   /nomodkey /keypress u
   /if (!${Me.ID} || !${ZoneName.Equal[${Zone.Name}]} || ${Zone.ID} != ${tZoneID}) /return
   /if (${Target.ID}) /squelch /tar clear
   /if (!${Switch.Open}) {
      /nomodkey /keypress use hold
      /delay 2
      /nomodkey /keypress use
   }
   /delay 2
   /nomodkey /keypress u
   /if (${Window[LargeDialogWindow].Open}) {
      /nomodkey /notify LargeDialogWindow LDW_YesButton leftmouseup
      /varset tMB TRUE
      /delay 5
      /goto :tEnd
   }
   /if (${Target.ID} || !${Switch.Open}) {
      /delay 7
      /nomodkey /keypress forward hold
      /delay 2
      /nomodkey /keypress forward
      /goto :tTop
   }
   /if (!${Zone.Name.Equal[${ZoneName}]} || !${Me.ID}) {
      /varset tMB TRUE
      /delay 5
      /goto :tEnd
   }
   /delay 2
   /if (${Switch.Open}) {
      /varset tMB TRUE
      /delay 5
      /goto :tEnd
   }
   /if (${tDoorTimer}) /goto :tTop
:tEnd
/return ${tMB}


Sub MBWayPlay(Section,DirectionUD,ZoneTF,DataA,DataB)
| Syntax MBWayPlay([name of INI section],[direction <u>p,<d>own,<b>egining,<e>nd],[try to zone <t>rue,<f>alse],[Options,<c>hain,<pull>,<sell>,<buy>],[Options,<[merchant name]>,<[pull script]>)
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Entering MBWayPlay
   /if (${MakeCamp.Status.Equal[on]}) /squelch /makecamp off
   /if (!${Section.Length} || !${DirectionUD.Length}) { 
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Syntax Error - mbwayplay <path name> <(up down begin end)u,d,b,e> <(zone)t,f> <(chain paths)c>
      /return
   }
   /if (!${ZoneTF.Length}) /varset ZoneTF f
   /if (!${DataA.Length}) /varset DataA
   /if (!${DataB.Length}) /varset DataB
   /declare a int local
   /declare b int local
   /declare x int local
   /declare y int local
   /declare Sx int local
   /declare Sy int local
   /declare WPStart int local 1
   /declare tMB bool local FALSE
   /declare tZoneName string local
   /declare tZoneID int local ${Zone.ID}
   /declare tZoneTF string local
   /declare tPull int local 0
   /declare tMBWayINI string local ${MBWayPnt}
:tTop  
   /if (${Select[${Section.Arg[1,-]},net,network]}) { 
      /varset tMBWayINI ${NetworkINI}
      /varset Section ${Section.Right[${Math.Calc[${Section.Length}-${Section.Find[-]}]}]}
   } 
   /docommand ${If[!${Defined[WPLoop]},/declare WPLoop bool local ${Ini["${tMBWayINI}",${Section},WPLoop]},/varset WPLoop ${Ini["${tMBWayINI}",${Section},WPLoop]}]}
   /declare WPCount int local ${Ini["${tMBWayINI}",${Section},WPCount]}
   /declare WP[${WPCount}] string Local
   /varset b 0 
   /for a ${If[${Select[${DirectionUD},u,e]},${WPCount},${If[${DirectionUD.Find[AP]},${DirectionUD.Right[-2]},1]}]} ${If[${Select[${DirectionUD},u,e]},downto,to]} ${If[${Select[${DirectionUD},u,e]},1,${WPCount}]} 
      /varcalc b ${b}+1
      /varset WP[${b}] ${Ini["${tMBWayINI}",${Section},WP${a}]}
      /varset y ${WP[${b}].Arg[1]}
      /varset x ${WP[${b}].Arg[2]}
      /if (${b}==1) { 
         /varset Sy ${y}
         /varset Sx ${x}
      }
      /if (${Select[${DirectionUD},u,d]} && ${Math.Distance[${Me.Y},${Me.X}:${y},${x}]}<${Math.Distance[${Me.Y},${Me.X}:${Sy},${Sx}]}) { 
         /varset Sy ${y}
         /varset Sx ${x}
         /varset WPStart ${b}
      }
      /if (!${tPull} && ${WP[${b}].Find[ pull]}) /varset tPull 1
   /next a
   /for a ${If[${WPStart}>1,${WPStart},1]} to ${WPCount}
      /varset PathLoc ${a}
      /if (${Me.Casting.ID} && ${Me.Class.Name.NotEqual[Bard]}) /delay 3s !${Me.Casting.ID}
      /if (!${DoPull} || ${a}!=1 || (${a}==1 && ${Math.Distance[${Me.Y},${Me.X}:${WP[${a}].Arg[1]},${WP[${a}].Arg[2]}]}>30 && ${SpawnCount[npc loc ${WP[${a}].Arg[1]} ${WP[${a}].Arg[2]} radius ${Radius}]}) || ${FailCheck}) /call MBMoveTo ${WP[${a}].Arg[1]} ${WP[${a}].Arg[2]}
      /if (${DoPull} && (${tZoneID}!=${Zone.ID} || ${Select[${Macro.Return},zoned]})) /goto :tMWAbort
      /if (${WP[${a}].Arg[3].Equal[pull]} || ${tPull} && ${APAggro} || ${a}==${WPCount} || ${a}==1 && ${CurrentMobCount} && !${APTargetID}) {
         /if (${WP[${a}].Arg[3].Equal[pull]} || ${tPull} && ${APAggro} || ${a}==1 && ${CurrentMobCount} && !${APTargetID}) /call AdvPull ${WP[${a}].Arg[4]}
         /if (${DoPull} && !${Macro.Return} && !${APAggro} && ${a}!=${WPCount} || ${DoPull} && ${Macro.Return.Equal[NULL]} && !${APAggro} && ${a}!=${WPCount} || ${FailCheck}) /next a
         /if (${DoPull}) {
            /if (${Macro.Return} || ${a}==${WPCount} || && (${APTargetID} || ${APAggro})) {
               /if (${WPLoop}) {
                  /if (${a}<${WPCount}-1) {
                     /for b ${a} downto 1
                         /call MBMoveTo ${WP[${b}].Arg[1]} ${WP[${b}].Arg[2]} ${tPull}
                         /if (${DoPull} && ${tZoneID}!=${Zone.ID}) /goto :tMWAbort
                         /varset PathLoc ${b}
                     /next b
                  }
               } else /if (!${APRetPath.Length} && !${WPLoop}) {
                  /call MBMoveTo ${WP[1].Arg[1]} ${WP[1].Arg[2]} ${APAggro}
                  /if (${DoPull} && ${tZoneID}!=${Zone.ID}) /goto :tMWAbort
                  /varset PathLoc 1
               }
               /if (${APTargetID}) {
                  /squelch /face nolook ${If[${MacroQuest.BuildName.Equal[emu]},fast ,]}id ${APTargetID}
                  /varset tMB TRUE
               } else {
                  /if (${MakeCampD} && ${UseHeadings}) {
                     /squelch /face nolook ${If[${MacroQuest.BuildName.Equal[emu]},fast ,]}heading ${MakeCampD}
                  }     
                  /varset tMB FALSE
                  /if (${WP[${WPCount}].Arg[4].Length}) {
                     /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Different Pull path detected
                     /varset a ${WPCount}
                     /varset WP[${a}] empty empty ${WP[${a}].Arg[4]} b
                     /goto :tChainPull
                  }
               }
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Was pull good - ${tMB}
               /goto :tMWAbort
            }
         }
      } 
      /if (${WP[${a}].Arg[3].Equal[door]}) /call MBDoor
      /if (${WP[${a}].Arg[3].Equal[sell]}) {
         /call SellCheck ${WP[${a}].Arg[4]}
         /if (!${Macro.Return}) /goto :tMWAbort
      }
   /next a
   /if (${Math.Distance[${Me.Y},${Me.X}:${WP[${b}].Arg[1]},${WP[${b}].Arg[2]}]}<15) /varset tMB TRUE  
   /if (${ZoneTF.Equal[t]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Trying to Zone ${ZoneTF}
      /varset a 0
      /if (${DataA.Length}>2) /varset a 1
      /if (${DataB.Length}) {
         /varset DataA ${DataA} ${DataB}
         /varset a 2
      }
      /if (${a}) {
         /call MBZone ${DataA.Arg[${a}]}
      } else {
         /call MBZone
      }
      /if (!${Macro.Return}) /goto :tMWAbort
      /delay 600s ${Me.ID}
   }
   /if (${DataA.Arg[1].Equal[c]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Chaining Waypoints
      /varset a 0
      /if ((${DirectionUD.Find[u]} || ${DirectionUD.Find[e]}) && ${WP[1].Arg[3].Length}) /varset a 1
      /if ((${DirectionUD.Find[d]} || ${DirectionUD.Find[b]}) && ${WP[${WPCount}].Arg[3].Length}) /varset a ${WPCount}
      /if (${a}>=1) {
:tChainPull
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Chain Found
         /varset Section ${WP[${a}].Arg[3]}
         /varset DirectionUD ${WP[${a}].Arg[4]}
         /varset ZoneTF 
         /varset DataA 
         /varset DataB
         /if (${WP[${a}].Arg[5].Length}) /varset ZoneTF ${WP[${a}].Arg[5]}
         /if (${WP[${a}].Arg[6].Length}) /varset DataA ${WP[${a}].Arg[6]}
         /if (${WP[${a}].Arg[7].Length}) /varset DataB ${WP[${a}].Arg[7]}
         /if (!${Section.Length} || !${DirectionUD.Length}) /goto :tMWAbort
         /deletevar WP
         /deletevar WPCount
         /delay 3s
         /goto :tTop
      } else {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Chain Not Found
      }
      /varset tMB TRUE
:tMWAbort
   }
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Exiting MBWayPlay ${tMB}
   /nomodkey /keypress back
   /if (${tPull}) {
      /if (${APLoopCntr}>3 && ${ACAssistPct}!=100) {
         /varset APLoopCntr 0
         /varset APCheckTimer 6m
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Looped through a few times, No mobs, waiting 6 mins, time now ${Time}
      } else {
         /varcalc APLoopCntr ${APLoopCntr}+1
      }
   }
/return ${tMB}


Sub SitCheck
   /if (${SitCheckTimer}) /return
   /varset SitCheckTimer 3s
   /if ((${Me.MaxMana}==0 || ${Me.PctMana}>=${SitManaPct}) && ${Me.PctEndurance}>=${SitEndurPct}) /return
   /if (${Me.Mount.ID} || ${Me.Moving} || ${Me.State.Equal[feign]} || ${Me.Sitting}) /return
   /if ((${Me.Casting.ID} && !${Me.Class.ShortName.Equal[BRD]}) || ${ACState} || ${Melee.Combat} || (${TankName.Equal[${Me.Name}]} && ${ADMobFound})) /return
   /if (${FollowState} && ${Spawn[${FollowID}].Distance}>=${FollowDistance} && !${AdvPath.Idle}) /return
   /if (${Me.Class.ShortName.Equal[BRD]}) {
      /if (${Twist} && !${Me.State.Equal[sit]} && ${Twist.List.NotEqual[${BardRestGems} ]}) /return
   }
   /if (${AdvPath.Following} && ${Spawn[${FollowID}].Distance}>10 && ${Target.ID}==${FollowID} && !${Me.State.Equal[sit]}) {
	  /squelch /moveto id ${FollowID} mdist 10
   }
   /if (${Select[${Zone.ShortName},${AllNonCombatZones}]}) /goto :SitOK
   /if (${ZoneName.NotEqual[${Zone.Name}]}) /call ProcessExcludeList
   /declare tSpawnParms string local npc radius ${SitAggroRadiusCheck} zradius ${ZRadius} noalert ${AlertList}
   /declare TotalNPCs int local ${SpawnCount[${tSpawnParms}]}
   /declare tNPCCount int local 0
   /declare npcid int local 0
:GetSpawn
   /varcalc tNPCCount ${tNPCCount}+1
   /if (${tNPCCount}>${TotalNPCs}) /goto :SitOK
   /varset npcid ${NearestSpawn[${tNPCCount},${tSpawnParms}].ID}
   /if (!${npcid}) /goto :SitOK
   /if (!${Spawn[${npcid}].LineOfSight}) {
      /if (${ACMATarget}!=${npcid}) /goto :GetSpawn
      /if (${Spawn[${npcid}].Distance3D}>25) /goto :GetSpawn
   }
   /if (${Spawn[${npcid}].Aggressive}) {
      /if (!${Me.Mount.ID} && !${Me.Standing}) /stand
      /return
   }
   /goto :GetSpawn
:SitOK
   /if (${ADCharmAD}) {
      /if (${ADCharmID} && ${ADCharmID}==${Me.Pet.ID} && !${Me.Sitting}) {
         /squelch /face nolook ${If[${MacroQuest.BuildName.Equal[emu]},fast ,]}id ${ADCharmID}
      }
   }
   /if (${Me.Class.ShortName.Equal[BRD]}) /squelch /stoptwist
   /if (!${Me.Sitting}) /sit on
/return


Sub AdvPathCall
   /declare tZoneName string local
   /declare tZoneTimer timer local
   /if ((${Me.State.Equal[feign]} || ${Spawn[${FollowID}].Type.Equal[corpse]}) && ${AdvPath.Following} && !${AdvPath.Paused}) {
      /call AdvPlugCall off
      /return
   }
   /if (!${FollowState} || ${Me.State.Equal[feign]}) /return
   /if (${Me.Level}<51 && ${Me.Sneaking} && ${Group.Members}) { 
      /doability sneak
      /delay 5
   }
   /if (!${Me.Mount.ID} && !${Me.Standing}) {
      /if ((${Spawn[${FollowID}].Distance3D}>${FollowDistance} && ${Spawn[${FollowID}].Name.Find[${FollowName}]}) || !${Spawn[pc =${FollowName}].ID}) {
         /sit off
      }
   }
   /if (${Spawn[pc =${FollowName}].ID} && ${FollowID}!=${Spawn[pc =${FollowName}].ID}) {
      /squelch /stick off
      /varset FollowID ${Spawn[pc =${FollowName}].ID}
      /varset FollowState 1
   }
   /call AdvPlugCall
   /if (!${Spawn[pc =${FollowName}].ID} && ${FollowState}<10) {
      /if (${NetBots[${FollowName}].State.Equal[dead]} || ${Spawn[${FollowName}'s corpse].ID}) {
         /squelch /stick off
         /call AdvPlugCall off
         /return
      }
      /if (${Zone.Name.Equal[${ZoneName}]}) {
         /if (!${Me.Mount.ID} && !${Me.Standing}) /stand
         /bc Msg: Trying to Zone
         /varset FollowState 10
         /if (${Spawn[Magus radius 100].ID}) {
            /delay 2s
            /varset tZoneTimer 350s
			/while (${tZoneTimer}) {
			   /if (${NetBots[${FollowName}].ID}) /break
			   /delay 1
            }
            /if (!${Zone[${NetBots[${FollowName}].Zone}].ShortName.Length} || ${Me.Invis}) { 
               /bc Msg: Can't zone through
               /varset FollowState 0
               /return
            }
            /varset tZoneName ${Zone[${NetBots[${FollowName}].Zone}].ShortName}
         }
         /call MBZone ${tZoneName}
         /if (!${Macro.Return} && !${Spawn[pc =${FollowName}].ID}) { 
            /bc Msg: Can't zone through
            /varset FollowState 0
         }
      }
   }
/return


Sub AdvPlugCall(string Toggle,string Close)
| Toggle : Can be [off/stop/pause/ToonName, or no entry when called to 'true-up']
| Close  : Can be [close] for following closely (5)
   /declare tID int local ${FollowID}
   /declare tName string local ${FollowName}
   /if (!${Toggle.Length}) {
      /if (${AdvPath.Following}) {  
         /if (!${AdvPath.Paused}) /return
         /if (!${ACMATarget} && ${FollowName.Length} && ${Spawn[pc =${FollowName}].ID} && !${CampStatus}  && !${Spawn[${Me}'s corpse'].ID} && !${Melee.Combat} && !${Me.Combat}) {
            /squelch /afollow unpause
            /varset FollowState 1
            /return
         }
      }
| AdvPath sometimes stops itself (zoning, getting summoned, etc.), so we may need to start it again down below
      /if (!${FollowName.Length} || !${FollowID}) /return
   } else /if (${Select[${Toggle},off,stop]}) { 
      /squelch /afollow off
      /varset FollowState 0
      /varset FollowID 0
      /varset FollowName
      /return
   } else /if (${Toggle.Equal[pause]}) {
      /if (${AdvPath.Following} && !${AdvPath.Paused}) { 
         /squelch /afollow pause
         /varset FollowState 4
      }
      /return
   } else /if (${Spawn[pc =${Toggle}].ID}) {
      /if (${AdvPath.Following} && ${Spawn[pc =${Toggle}].ID}!=${Spawn[pc =${AdvPath.Monitor}].ID} || ${AdvPath.Paused}) {
         /squelch /afollow off
         /varset FollowState 0
         /varset FollowID 0
         /varset FollowName
      }
      /if (!${FollowID} || !${FollowName.Length} || ${FollowID}!=${Spawn[pc =${Toggle}].ID}) {
         /varset tID ${Spawn[pc =${Toggle}].ID}
         /varset tName ${Spawn[pc =${Toggle}].CleanName}
      }
   } else {
|/bc what am i doing here? Toggle = ${Toggle}
      /return
   }
   
| Check/set follow
   /if (!${Spawn[pc =${tName}].ID} || !${tID} && !${tName.Length} || ${tName.Equal[${Me.CleanName}]} && (!${tID} || ${tID}==${Me.ID})) /return
   /if (${Spawn[pc =${tName}].ID}!=${tID} && ${tName.NotEqual[${Me.CleanName}]}) {
      /varset tID ${Spawn[pc =${tName}].ID}
   }
   /if (!${tID}) /return  
   /if (${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[brd]}) {
      /delay 10s !${Me.Casting.ID}
   }
   /if (${Stick.Status.Equal[on]}) /squelch /stick off
   /squelch /afollow on spawn ${tID} ${If[${Close.Equal[close]},5,${FollowDistance}]}
   /varset FollowState 1
   /varset FollowID ${tID}
   /varset FollowName ${Spawn[${tID}].CleanName}
   /if (${Me.Pet.ID}) {
      /if (${Pet.Stance.NotEqual[follow]}) /pet follow
   }
/return
