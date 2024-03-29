
#Event MobProb "#*#Your target is too far away#*#"
#Event MobProb "#*#You cannot see your target#*#"
#Event MobProb "#*#You can't hit them from here#*#"
#Event EnrageStart "#1# has become ENRAGED#*#"
#Event EnrageEnd "#1# is no longer enraged#*#"


Sub LoadACINIVals
   /declare ACStartX int outer
   /declare ACStartY int outer
   /declare ACStartHeading int outer
   /declare ACMATarget int outer
   /declare ACState int outer
   /declare ACBeginHP int outer
   /declare ACEndHP int outer
   /declare ACTarID int outer
   /declare ACMobProbT timer outer 5s
   /declare ReTargetT timer outer 5s
   /declare ACSwitchID int outer

   /call LoadINIVar Melee ACLeash 50 ACLeash "${IniFile}" int
   /call LoadINIVar Melee ACAssistPct 95 ACAssistPct "${IniFile}" int
   /call LoadINIVar Melee ACManaPct 70 ACManaPct "${IniFile}" int
   /call LoadINIVar Melee ACAnnounce " " ACAnnounce "${IniFile}" string
   /call LoadINIVar Melee ACMeleeCmd "/melee plugin=1" ACMeleeCmd "${IniFile}" string
   /call LoadINIVar Melee ACBefore " " ACBefore "${IniFile}" string
   /call LoadINIVar Melee ACAfter " " ACAfter "${IniFile}" string
   /call LoadINIVar Melee ACPetEnrage TRUE ACPetEnrage "${IniFile}" bool
   /call LoadINIVar Melee OffTank FALSE OffTank "${IniFile}" bool
   
   /if (${ACMeleeCmd.Length}>10) /squelch /docommand ${ACMeleeCmd}
   /assist off
/return


Sub AdvCombat(P0,P1,P2)
   /if (${MBPause}) /return
   /if (!${Melee.Enable}) {
      /if (${DoMelee} && ${MakeCampY} && ${MakeCampX} && ${Math.Distance[${Me.Y},${Me.X}:${MakeCampY},${MakeCampX}]}<40) /squelch /melee plugin=1
   }
   /if (${TankName.Equal[${Me.Name}]}) {
      /if (${Target.Type.Equal[npc]} && ${ACMATarget} && ${Spawn[${ACMATarget}].ID} && !${Spawn[${ACMATarget}].Type.Equal[corpse]}) {
         /if (${ReTargetT} || ${Me.Combat}) /return
         /varset ReTargetT 10s
      }
   }
   /if (${Me.State.Equal[feign]} && ${PullerName.Equal[${Me.CleanName}]}) /return

   /declare a int local
   /declare OTID int local ${Target.ID}

   /call ADSpawnCheck
   /if (${DoHeals} && ${AHCount}) /call AHHealCheck
   /if (${ACState}==100 || !${ACState} || ${OffTank} && !${ReTargetT} && ${ADMobCount}>1 || ${OffTank} && ${P0.Length}) {
      /if (${TankName.Equal[${Me.CleanName}]} && !${ACMATarget}) {
         /if (${DoPull}) {
            /if (${APTargetID}) /varset ACMATarget ${APTargetID}
         } else /if (${PullerName.Length}>2 && ${PullerName.NotEqual[${Me.CleanName}]}) {
            /if (${Spawn[npc id ${NetBots[${PullerName}].TargetID}].Distance}<${ACLeash} && !${ExcludeList.Find[|${Spawn[${NetBots[${PullerName}].TargetID}].CleanName}|]} && !${ExcludeListID.Find[|${NetBots[${PullerName}].TargetID}|]} && ${Spawn[${NetBots[${PullerName}].TargetID}].PctHPs}<=${ACAssistPct}) {
               /varset ACMATarget ${NetBots[${PullerName}].TargetID}
               /goto :GotTarget
            }
         }
      }
      /if (!${OffTank} || ${ADMobCount}<2) {
         /if (${Spawn[pc ${TankName}].Distance}<200) {
            /if (${NetBots[${TankName}].ID}) {
               /if (!${ExcludeList.Find[|${Spawn[${NetBots[${TankName}].TargetID}].CleanName}|]} && !${ExcludeListID.Find[|${NetBots[${TankName}].TargetID}|]} && ${Spawn[${NetBots[${TankName}].TargetID}].Standing}) /varset ACMATarget ${NetBots[${TankName}].TargetID}
            } else /if (!${NetBots[${TankName}].ID}) {
               /call ACGetAssist ${TankName}
               /if (!${ExcludeList.Find[|${Spawn[${Macro.Return}].CleanName}|]} && !${ExcludeListID.Find[|${Spawn[${Macro.Return}].ID}|]}) /varset ACMATarget ${Macro.Return}
            }
         }
         /if (${ACMATarget}) {
            /if (${ACState}!=100 && ${If[${NetBots[${TankName}].ID},${NetBots[${TankName}].TargetHP},${Spawn[${ACMATarget}].PctHPs}]}>${ACAssistPct} || !${Spawn[${ACMATarget}].Standing}) /varset ACMATarget 0
         }
      } else /if (${OffTank}) {
|        /varset ACTarID 0
         /if (${P0.Length}) {
            /if (${Spawn[npc radius 100 ${P0}].ID}) /varset ACTarID ${Spawn[${P0}].ID}
            /if (!${ACTarID}) {
               /if ((${NetBots.Client.Find[${P0}]} || ${Group.Member[${P0}].Index}) && ${P0.NotEqual[${Me.CleanName}]}) {
                  /varset ACTarID ${NetBots[${P0}].TargetID}
               } else {
                  /call ACGetAssist ${P0}
                  /varset ACTarID ${Macro.Return}
               }
            }
            /varset ReTargetT 5s
            /varset ACBeginHP ${If[${P1.Length},${P1},100]} 
            /varset ACEndHP ${If[${P2.Length},${P2},0]} 
         } else {
:ChangeTarget	    	
            /if (!${TankName.Equal[${Me.CleanName}]} && ${ADMobCount}) {
               /if (${PullerName.Equal[${Me.CleanName}]}) {
                  /if (!${NetBots[${TankName}].TargetID}) {
                     /if (${Spawn[pc ${TankName}].Distance}<${ACLeash} && ${Target.ID}) /delay 4s ${NetBots[${TankName}].TargetID}
                  }
               }
               /if (${NetBots[${TankName}].TargetID} && ${ACMATarget} && ${ACTarID} && ${Spawn[${ACTarID}].ID} && ${Spawn[${ACTarID}].Type.NotEqual[corpse]} && ${Target.ID}==${ACTarID} && ${NetBots[${TankName}].TargetID}!=${Target.ID} && ${Melee.Combat}) /return
               /if (${Target.ID}==${NetBots[${TankName}].TargetID} && ${ADMobCount}>1) {
                  /varset ACTarID 0
                  /varset ACMATarget 0
               }
               /for a ${ADMobMax} downto 1
                  /if (${ADMobList[${a}]}) {
                     /if (${ADMobList[${a}]}!=${NetBots[${TankName}].TargetID} && ${Spawn[${ADMobList[${a}]}].ID} && ${Spawn[${ADMobList[${a}]}].Type.NotEqual[corpse]} && ${Spawn[${ADMobList[${a}]}].Distance}<${ACLeash} && !${ExcludeList.Find[|${Spawn[${ACTarID}].CleanName}|]} && !${ExcludeListID.Find[|${Spawn[${ACTarID}].ID}|]}) /varset ACTarID ${ADMobList[${a}]}
                  }
               /next a
               /if (${ACTarID}) {
                  /if (${ACTarID}!=${Target.ID} && ${Spawn[${ACTarID}].ID} && ${Spawn[${ACTarID}].Type.NotEqual[corpse]}) {
                     /squelch /target id ${ACTarID}
                     /delay 1s ${Target.ID}==${ACTarID}
                  }
                  /if (${ACTarID}!=${Me.ID} && ${Spawn[${ACTarID}].ID} && ${Spawn[${ACTarID}].Type.NotEqual[corpse]} && ${ACTarID}!=${NetBots[${TankName}].TargetID}) /bc Off-tanking a ${Spawn[id ${ACTarID}].CleanName}
                  /varset ACBeginHP ${If[${P1.Length},${P1},100]} 
                  /varset ACEndHP ${If[${P2.Length},${P2},0]} 
               } else {
                  /varset ACTarID ${NetBots[${TankName}].TargetID}
               }
            }
         }
         /if ((!${ACTarID} || ${ACState}==100) && !${ExcludeList.Find[|${Spawn[${NetBots[${TankName}].TargetID}].CleanName}|]} && !${ExcludeListID.Find[|${NetBots[${TankName}].TargetID}|]} && ${Spawn[${NetBots[${TankName}].TargetID}].Standing}) {
            /varset ACTarID ${NetBots[${TankName}].TargetID}
            /varset ACMATarget ${NetBots[${TankName}].TargetID}
         }
         /if (${ACTarID} && ${ACBeginHP}) {
            /if ((${Spawn[${ACTarID}].PctHPs}<${ACBeginHP} || ${Spawn[${ACTarID}].PctHPs}>=${ACEndHP}) && ${Spawn[${ACTarID}].Standing}) /varset ACMATarget ${ACTarID}
         } else {
            /varset ACMATarget 0
            /if (!${TankName.Equal[${Me.CleanName}]} && ${ACTarID} && ${Target.ID}==${ACTarID}) {
               /varset ACTarID ${Target.ID}
               /varset ACMATarget ${Target.ID}
            }
         }
         /if (${TankName.Equal[${Me.CleanName}]} && ${DoPull} && !${ACMATarget} && ${APTargetID}) /varset ACMATarget ${APTargetID}
      }
:GotTarget
      /varset ReTargetT 5s
      /if (${ACMATarget} && ${Select[${Spawn[${ACMATarget}].Type},npc,pet]} && !${Spawn[${ACMATarget}].Master.Type.Equal[pc]} && ${Spawn[${ACMATarget}].Distance3D}<=${ACLeash}) {
         /if (${ACState}==100 && ${Target.ID}!=${ACMATarget}) {
            /squelch /tar id ${ACMATarget}
            /delay 2s ${Target.ID}==${ACMATarget}
         }
         /if ((!${OTID} || !${Spawn[${OTID}].Type.Equal[npc]}) && ${ACMATarget}) /varset OTID ${ACMATarget}
         /if (${ACBefore.Length}>3 && ${OTID}==${ACMATarget}) /call IniCommand "${ACBefore}"
         /if (${ACAnnounce.Length}>5 && ${OTID}==${ACMATarget}) {
            /call ParseAnnounce "${ACAnnounce}" "${Spawn[${ACMATarget}].CleanName}" " "
         } else /if (${ACAnnounce.Length}>1 && ${ACAnnounce.Length}<6) {
            /docommand ${ACAnnounce} Meleeing: -[ ${Spawn[${ACMATarget}].CleanName} ]-
         }
         /if (${FollowFlag}) {
            /varset ACState 2
            /varset FollowFlag 0
            /if (${${FollowPlug}.Active}) {
               /call AdvPlugCall stop
            } else {
               /squelch /stick off
            }
         } else {
            /varset ACStartX ${If[${ACState}==100 && (${Math.Distance[${Me.Y},${Me.X}:${ACStartY},${ACStartX}]}>50 || !${ACStartY}),${Spawn[${TankName}].X},${Me.X}]} 
            /varset ACStartY ${If[${ACState}==100,${Spawn[${TankName}].Y},${Me.Y}]} 
            /varset ACStartHeading ${Me.Heading.DegreesCCW}
            /varset ACState 1
         }
      } else {
         /varset ACState 0
         /varset ACMATarget 0
         /varset ACTarID 0
         /varset ACBeginHP 0
      }
   }

   /if (!${ACState}) /return

   /doevents MobProb
   /if (!${ACMATarget} || ${ACState}>2 || ${Spawn[${ACMATarget}].Type.Equal[corpse]} || !${Spawn[${ACMATarget}].ID} || ${Math.Distance[${Me.Y},${Me.X}:${ACStartY},${ACStartX}]}>${ACLeash} && ${Spawn[pc ${TankName}].Distance}>30) {
      /if (${Me.Class.ShortName.Equal[BRD]} || ${Me.Casting.ID}) {
         /stopcast
|        /squelch /stoptwist
      }
      /squelch /tar clear
      /if (${Melee.Combat}) /attack off
      /if (${APTargetID}) {
         /varset APTargetID 0
         /varset APAggro 0
      }
      /delay 1s !${Melee.Combat}
      /squelch /stick off
      /nomodkey /keypress centerview
      /if (${Me.Pet.ID} && ${PetAssist}) {
         /varset PetAssist 1
         /pet back off
      }
| Force a SpawnCheck
      /varset SpawnCheckTimer 0
      /call ADSpawnCheck
      /if (${Select[${ACState},2,4]} && (!${Me.Combat} && !${Melee.Combat} || !${ACMATarget} || ${Spawn[${FollowName}].Distance}>${ACLeash} || !${CurrentMobCount})) {
         /if (${${FollowPlug}.Active}) {
            /call AdvPlugCall "${FollowName}"
         }
         /varset FollowFlag 1
      } else {
         /if (${CampStatus} && !${FollowFlag} && !${ADMobCount}) {
            /varset CampStatus 1
         } else /if (${Math.Distance[${Me.Y},${Me.X}:${ACStartY},${ACStartX}]}>10 && !${FollowFlag} && !${ADMobCount}) {
            /squelch /moveto loc ${ACStartY} ${ACStartX}
         } else {
            /squelch /face Heading ${ACStartHeading}
         }
         /if (${CampStatus} && !${FollowFlag} && !${ADMobCount}) {
           /delay 2s ${Math.Distance[${Me.Y},${Me.X}:${ACStartY},${ACStartX}]}<=10
           /squelch /face Heading ${ACStartHeading}
|          /multiline ; /face Heading ${ACStartHeading};/varset ACStartX ${Me.X};/varset ACStartY ${Me.Y}
         }
      }
      /if (${ACAfter.Length}>3) /call IniCommand "${ACAfter}"
      /if (${DoLoot}) {
         /if (${LootMobs} && (!${ADMobCount} || ${MobsTooClose}<=5)) {
            /if (${SpawnCount[corpse radius ${CorpseRadius} zradius ${ZRadius} noalert 25]}) /call LootMobs
         }
      }
      /varset ACMATarget 0
      /varset ACState 0
      /varset ACTarID 0
      /varset ACBeginHP 0
      /return
   }
   /if (!${OffTank}) {
      /if (!${ReTargetT}) {
         /varset ReTargetT 3s
		 /if (${NetBots[${TankName}].ID}) {
		    /if (${Target.ID}!=${NetBots[${TankName}].TargetID}) {
			   /if (!${NetBots.Client.Find[${Spawn[${NetBots[${TankName}].TargetID}].CleanName}]} && !${Group.Member[${Spawn[${NetBots[${TankName}].TargetID}].CleanName}].Index}) {
		          /if (!${ExcludeList.Find[|${Spawn[${NetBots[${TankName}].TargetID}].CleanName}|]} && !${ExcludeListID.Find[|${NetBots[${TankName}].TargetID}|]} && ${Spawn[${NetBots[${TankName}].TargetID}].Standing}) /varset ACMATarget ${NetBots[${TankName}].TargetID}
			   }
			}
         } else {
		    /if (${Target.ID}!=${ACMATarget}) {
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Assisting again to get target
               /call ACGetAssist ${TankName}
               /if (!${ExcludeList.Find[|${Spawn[${Macro.Return}].CleanName}|]} && !${ExcludeListID.Find[|${Spawn[${Macro.Return}].ID}|]} && ${Spawn[${Macro.Return}].Standing}) /varset ACMATarget ${Macro.Return}
			}
         }
      }
   }
   /if (${OffTank} && ${Target.ID}!=${ACMATarget} && ${ACTarID}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}188; ${Target.ID}  : ${ACTarID}
      /varset ACMATarget ${ACTarID}
      /if (${ACTarID} && ${ACBeginHP}) {
         /if  (${Spawn[${ACTarID}].PctHPs}>${ACBeginHP} || ${Spawn[${ACTarID}].PctHPs}<=${ACEndHP}) /varset ACMATarget 0
      } else {
         /varset ACMATarget 0
      }
   }
   /if (${ACMATarget}) {
      /if (${Target.ID}!=${ACMATarget}) {
         /if (${Me.PctMana}>${ACManaPct} || !${Me.PctMana} || ${ACManaPct}==101) {
            /squelch /tar id ${ACMATarget}
            /delay 2s ${Target.ID}==${ACMATarget}
         }
      }
      /if (${Target.Distance3D}<=${ACLeash}) {
         /if (${Me.Class.ShortName.Equal[BRD]}) {
            /if (${Twist.List.NotEqual[${BardCombatGems} ]} && ${BardCombatGems.Length}) {
               /if (${Me.State.Equal[sit]}) /stand
               /squelch /twist ${BardCombatGems}
               /if (${InfoMsgLevel}) {
                  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Msg: Combat gems
               }
            }
         }
         /if (${PetAssist} || ${SPetAssist}) {
            /doevents PetNoWake
            /if (${PetAssist} && ${Me.Pet.ID} && ${PetAssist}!=${ACMATarget}) {
               /pet attack ${ACMATarget}
               /varset PetAssist ${ACMATarget}
            }
            /if (${SPetAssist} && ${Target.ID}==${ACMATarget} && ${SPetAssist}!=${ACMATarget}) {
               /pet swarm
               /varset SPetAssist ${ACMATarget}
            }
         }
         /if (${Me.PctMana}>${ACManaPct} || (${Me.MaxMana}==0 && ${ACManaPct}!=101)) {
            /if (!${Me.Mount.ID} && !${Me.Standing} && (!${Melee.Combat} || !${Me.Combat})) /stand
            /if (${DoMelee} && ${Me.Standing} && ${Target.ID}==${ACMATarget} && (!${Melee.Combat} || !${Me.Combat}) && (${Me.Class.ShortName.Equal[brd]} || (!${Me.Casting.ID} && !${Cast.Status.Find[C]}))) {
               /if (${${FollowPlug}.Following}) /docommand /${FollowCommand} pause
               /if (${Stick.Status.Equal[on]}) /stick off
               /delay 1s ${Stick.Status.Equal[Off]} && (${${FollowPlug}.Paused} || !${${FollowPlug}.Following})
               /killthis
               |/delay 2 ${Stick.Status.Equal[on]}
               |/if (${Stick.Status.Equal[off]}) /stick hold ${If[${Target.Height}<5,9,${Math.Calc[${Target.Height}+3].Int}]} ${If[!${Me.GroupSize} || ${Melee.AggroMode},moveback,${If[${Melee.BackStabbing},behind,!front]}]} ${If[${Me.Underwater},uw,]} 
               /varset ACMobProbT 3s
            }
         } else /if (${Melee.Combat}) {
            /squelch /tar clear
            /if (${Melee.Combat}) /attack off
            /delay 1s !${Melee.Combat}
            /squelch /stick off
            /if (${FollowName.Length} && ${${FollowPlug}.Paused}) /docommand /${FollowCommand} unpause
         }
      }
   }
/return


Sub ACGetAssist(P0)
   /declare tTargetID int local 0
   /if (!${P0.Length} || !${Spawn[pc ${P0}].ID} && !${Spawn[mercenary ${P0}].ID}) /return
   /if (${Spawn[pc ${P0}].Distance}>=200) /return
   /assist ${P0}
   /delay 1s ${Target.ID}!=${Spawn[${P0}].ID}
   /varset tTargetID ${If[${Target.ID}!=${Spawn[${P0}].ID} && ${Target.Type.NotEqual[PC]} && ${Target.Standing},${Target.ID},0]}
/return ${tTargetID}


Sub Event_EnrageStart(string Line,string MobName)
   /if (!${Me.Pet.ID}) /return
   /if (${ACPetEnrage} && ${Me.Pet.Target.CleanName.Equal[${MobName}]}) { 
      /pet back off
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Backing off Pet
   }
/return


Sub Event_EnrageEnd(string Line,string MobName)
   /if (!${Me.Pet.ID}) /return
   /if (${ACPetEnrage} && ${Target.ID}==${ACMATarget} && ${Target.CleanName.Equal[${MobName}]}) {
      /pet attack
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Enrage ended, re-sending Pet.
   }
/return


Sub Event_MobProb(string Line)
   /if (${ACMobProbT}) /return
   /if (${Melee.Combat} && ${Me.Standing} && ${Target.ID}==${ACMATarget} && ${Me.Combat} && (${Me.Class.ShortName.Equal[brd]} || !${Me.Casting.ID})) {
      /if (${Target.Distance3D}<=${ACLeash} && ${Target.Distance3D}>${Target.MaxRangeTo} && !${Stick.Status.NotEqual[on]}) /squelch /stick ${Math.Calc[${Target.MaxRangeTo}-4]}
      /if (!${Melee.Combat}) /killthis
      /if (${Melee.Combat} && ${Stick.Status.NotEqual[on]} && ${Target.Distance}<=25) {
         | /squelch /stick 10 ${Target.ID}
         /stick hold ${If[${Target.Height}<5,9,${Math.Calc[${Target.Height}+3].Int}]} ${If[!${Me.GroupSize} || ${Melee.AggroMode},moveback,${If[${Melee.BackStabbing},behind,!front]}]} ${If[${Me.Underwater},uw,]} 
         }
      /if (${Melee.Combat} && ${Stick.Status.Equal[on]} && ${Target.Distance}<=25) {
         /if (${Me.Levitating}) {
		    /squelch /face ${Target}
		 }	
         /squelch /stick -2
      }
      /varset ACMobProbT 2s
| /echo ${If[${TimeStampOnEcho},[${Time}] ,]}MP2 - ${Time} - ${MBCurrentSpell} - ${ACMATarget} - ${Spawn[${ACMATarget}].CleanName} - ${Target.Distance3D} - ${Target.MaxRangeTo} - ${Stick.Status} - ${Target.ID} - ${Target.CleanName} - ${Me.Casting.ID} - ${Cast.Status}
   }
/return
