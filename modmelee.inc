
#Event MobProb "Your target is too far away#*#"
#Event MobProb "You cannot see your target#*#"
#Event MobProb "You can't hit them from here#*#"
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
   /noparse /declare DefaultStick string outer hold id ${Target.ID} ${If[${Target.Height}<5,9,${Math.Calc[${Target.Height}+3].Int}]} ${If[!${Me.GroupSize} || ${Melee.AggroMode},moveback,${If[${Melee.BackStabbing},behind,!front]}]} ${If[${Me.Underwater},uw,]}    

   /call LoadINIVar Melee ACLeash 50 ACLeash "${IniFile}" int
   /call LoadINIVar Melee ACAssistPct 95 ACAssistPct "${IniFile}" int
   /call LoadINIVar Melee ACManaPct 20 ACManaPct "${IniFile}" int
   /call LoadINIVar Melee ACAnnounce " " ACAnnounce "${IniFile}" string
   /call LoadINIVar Melee ACMeleeCmd "/melee plugin=1" ACMeleeCmd "${IniFile}" string
   /call LoadINIVar Melee ACBefore " " ACBefore "${IniFile}" string
   /call LoadINIVar Melee ACAfter " " ACAfter "${IniFile}" string
   /call LoadINIVar Melee ACPetEnrage TRUE ACPetEnrage "${IniFile}" bool
   /call LoadINIVar Melee OffTank FALSE OffTank "${IniFile}" bool
   /call LoadINIVar Melee DoRanged FALSE DoRanged "${IniFile}" bool 
   /call LoadINIVar Melee TankList " " TankList "${IniFile}" string    
   
   /if (${ACMeleeCmd.Length}>10) /squelch /docommand ${ACMeleeCmd}
   /assist off
/return


Sub AdvCombat(P0,P1,P2)
   /if (${MBPause}) /return
   /if (!${Melee.Enable}) {
      /if (${DoMelee} && ${MakeCampY} && ${MakeCampX} && ${Math.Distance[${Me.Y},${Me.X}:${MakeCampY},${MakeCampX}]}<40) /squelch /melee plugin=1
   }
   /if (${Zone.ShortName.Equal[tacvi]} && ${Target.CleanName.Find[Husk of]} && ${TankName.Arg[1,${PipeChar}].Equal[${Me.CleanName}]}) {
      /target clear
      /varset ACMATarget 0
   }
   /if (!${DoMelee} && !${DoRanged} && ${TankList.Find[${Me.Name}]}) {
| TODO: This may need to be expanded for taunting in raids
      /if (!${Melee.Combat} && ${TankList.Find[${Me.Name}]} && ${ACMATarget} && ${Zone.ShortName.Equal[ikkinz]} && (${Target.ID}==${ACMATarget} || ${Target.ID}==${ACTarID})) {
         /if (${Me.CombatAbilityReady[Incite]}) /disc Incite
         /if (${Me.AbilityReady[Taunt]}) /doability taunt    
      }
     /return
   }
   /if (${DoRanged} && ${Spawn[id ${ACMATarget}].ID}) {
      /if (${Spawn[id ${ACMATarget}].ID} && ${Spawn[id ${ACMATarget}].Type.NotEqual[corpse]}) {
         /if (${Target.ID}!=${ACMATarget}) /target id ${ACMATarget}
         /delay 5 ${Target.ID}==${ACMATarget}
         /squelch /face nolook ${If[${MacroQuest.BuildName.Equal[emu]},fast ,]}${Target}
         /if (!${Stick.Status.Equal[on]}) /stick hold id ${Target.ID} {Math.Calc[40+{Target.Height}]} moveback
         /if (!${Me.AutoFire}) /autofire
         /return
      }
   }
   
   /if (${TankName.Arg[1,${PipeChar}].Equal[${Me.CleanName}]}) {
      /if (${Target.Type.Equal[npc]} && ${ACMATarget} && ${Spawn[${ACMATarget}].ID} && !${Spawn[${ACMATarget}].Type.Equal[corpse]}) {
         /if (${ReTargetT} || ${Me.Combat}) /return
         /varset ReTargetT 10s
      }
   }
   /if (${Me.State.Equal[feign]} && ${PullerName.Equal[${Me.CleanName}]}) /return

   /declare a int local
   /declare x int local   
   /declare OTID int local ${Target.ID}
   /declare tTargetID int local 0
   /declare xTargetID int local 0   
   /declare tTimer timer local
   /declare tDistance float local
   /declare offtankcount int local ${Math.Calc[${TankList.Count[ ]} + 1]}
   /declare ctank int local
   /declare ntank string local
   
         

   /call ADSpawnCheck
   /if (${DoHeals} && ${AHCount}) /call AHHealCheck
   /if (${ACState}==100 || !${ACState} || (${OffTank} && !${ReTargetT} && ${ADMobCount}>0) || (${OffTank} && ${P0.Length})) {
      /if (${TankName.Arg[1,${PipeChar}].Equal[${Me.CleanName}]} && !${ACMATarget}) {
         /if (${DoPull}) {
            /if (${APTargetID}) /varset ACMATarget ${APTargetID}
         } else /if (${PullerName.Length}>2 && ${PullerName.NotEqual[${Me.CleanName}]}) {
            /call ACGetAssist "${PullerName}" "TypeNPC${PipeChar}Distance"
            /if (${Macro.Return}) /varset ACMATarget ${Macro.Return}
         }
         /if (${ACMATarget}) /goto :GotTarget
      }
   }

   /if (${TankName.Arg[1,${PipeChar}].Equal[${Me.CleanName}]} && !${Target.ID} && ${ADMobCount}>0 && ${TankList.Length} > 2) {
      /if (!${Target.ID} || ${Target.Name.Find[Husk of]}) {
      |/echo /if (!${ACMATarget} || ${Spawn[${ACMATarget}].Type.Equal[corpse]} || !${Spawn[${ACMATarget}].ID}) 
      /if (!${ACMATarget} || ${Spawn[${ACMATarget}].Type.Equal[corpse]} || !${Spawn[${ACMATarget}].ID}) {
         /for ctank 1 to ${offtankcount}
            /varset ntank ${TankList.Arg[${ctank}]}
            /if (${NetBots[${ntank}].TargetID} && ${Spawn[${ntank}].ID}) {
               /echo ${ntank} : ${NetBots[${ntank}].TargetID}
               /varset tTargetID ${NetBots[${ntank}].TargetID}
               /call ACValidTarget ${tTargetID}
               /if (${Macro.Return.Equal[TRUE]}) {
                  /echo Switching to ${ntank}'s target.
                  /varset ACMATarget ${NetBots[${ntank}].TargetID}
               }
            }
         /next ctank
         /if (${ACMATarget}) /goto :GotTarget
      }
      }
   }

      /if (${OffTank} && ${ADMobCount}>0 && ${TankName.Arg[1,${PipeChar}].NotEqual[${Me.CleanName}]}) {
         /call GetTargetID ${TankName}
         /varset tTargetID ${Macro.Return}
| Check if other toons have other mobs tanked.
| Or already have a mob as tanking target, but mob is targetting another tank.
| Check other mobs if more than 2.
         /if ((${Target.ID} && ${Target.ID}==${tTargetID}) || (${Target.ID}!=${tTargetID} && ${ACTarID} && ${TankList.Find[${Me.TargetOfTarget.CleanName}]} && ${Me.TargetOfTarget.CleanName.NotEqual[${Me.Name}]} && ${ADMobCount}>2)) {
            /varset ACTarID 0
            /for a ${ADMobMax} downto 1
               /if (${ADMobList[${a}]}) {
                  /if (${ADMobList[${a}]}!=${tTargetID}) {
                     /if (${Spawn[${ADMobList[${a}]}].ID}) {
                        /call ACValidTarget "${ADMobList[${a}]}" "Distance"
                        /if (${Macro.Return.Equal[TRUE]} && !${ExcludeList.Find[${PipeChar}${Spawn[${ADMobList[${a}]}].CleanName}${PipeChar}]}) {
                           /varset ACTarID ${ADMobList[${a}]}
| Check if other tanks are attacking this target.
                           /for x 1 to ${Math.Calc[${TankList.Count[ ]}+1]}
                              /call GetTargetID ${TankList.Arg[${x}]}
                              /varset xTargetID ${Macro.Return}
                              /if (${xTargetID}==${ACTarID}) {
                                 /varset ACTarID 0
                                 /break
                              }
                           /next x
                        }
                     }
                  }
               }
               /if (${ACTarID}) /break
            /next a
         }
         /if (${ACTarID} && ${ACTarID}!=${Target.ID}) {
| If we get here, no other tank should be attacking this mob.
            /call ACValidTarget "${ACTarID}"
            /if (${Macro.Return.Equal[TRUE]} && !${ExcludeList.Find[${PipeChar}${Spawn[${ACTarID}].CleanName}${PipeChar}]}) { 
               /if (${ACMATarget}!=${ACTarID}) {
                  /bc Msg: Switching targets to offtank << ${Spawn[${ACTarID}].Name} >>
               }
               /squelch /target clear
               /if (${AdvPath.Following} && !${AdvPath.Paused}) {
                  /varset ACState 2
                  /varset FollowState 4
                  /squelch /afollow pause
               }
               /varset DoMelee TRUE
               /varset ACMATarget ${ACTarID}
               /squelch /melee reset
               /call BackPets
               /squelch /target id ${ACMATarget}
               /delay 2s ${Target.ID}==${ACMATarget}
               /if (${ACManaPct}!=101) {
                  /killthis
                  /delay 1s ${Me.Combat}
                  /if (${Me.Combat} && !${Stick.Status.Equal[on]} && (!${Me.Casting.ID} || ${Me.Class.ShortName.Equal[brd]})) {
                     /stick ${DefaultStick}
                  }
               }
               /call SendPets
            }
         }
| If I already have a offtank target, but it's targeting a netbots member that's not a tank. Taunt it off.
         /if (${tTargetID} && ${ACMATarget} && ${ACTarID} && ${Spawn[${ACTarID}].Type.NotEqual[corpse]} && ${Target.ID}==${ACTarID} && ${tTargetID}!=${Target.ID} && ${Melee.Combat} && !${TankList.Find[${Me.TargetOfTarget.CleanName}]} && ${NetBots[${Me.TargetOfTarget.CleanName}].ID} && ${Me.AbilityReady[Taunt]}) {
            /bc Taunting a ${Spawn[${ACTarID}].Name} off of ${Me.TargetOfTarget.CleanName} 
            /if (${Me.CombatAbilityReady[Incite]}) /disc Incite
            /if (${Me.AbilityReady[Taunt]}) /doability taunt
|           /delay 1s ${Me.TargetOfTarget.CleanName.Equal[${Me.Name}]}
|           /if (${Me.TargetOfTarget.CleanName.Equal[${Me.Name}]}) /return
            /return
         }
         /if (${ACTarID} && ${Melee.Combat} && ${ACMATarget}) /return
      }
      /if (!${OffTank} || ${ADMobCount}<2) {
        /if (${Spawn[TankID].Distance}<200) {
            /call ACGetAssist "${TankName}"
            /if (${Macro.Return}) {
               /varset ACMATarget ${Macro.Return}
            }
         }
         /if (${ACMATarget}) {
            /if (!${Spawn[${ACMATarget}].Standing}) {
               /varset ACMATarget 0
            } else /if (${ACState}==100 || ${TankName.Arg[1,${PipeChar}].Equal[${Me.CleanName}]}) {
               /goto :GotTarget
            } else /if (${ACAssistPct}<${If[${NetBots[${TankName.Arg[1,${PipeChar}]}].ID},${NetBots[${TankName.Arg[1,${PipeChar}]}].TargetHP},${Spawn[${ACMATarget}].PctHPs}]}) {
               /varset ACMATarget 0
            }
         }
      } else /if (${OffTank}) {
         /if (${P0.Length}) {
            /if (${Spawn[npc radius 100 ${P0}].ID}) {
               /varset ACTarID ${Spawn[${P0}].ID}
            }
            /if (!${ACTarID}) {
               /call ACGetAssist "${P0}"
               /varset ACTarID ${Macro.Return}
            }
            /varset ReTargetT 5s
            /varset ACBeginHP ${If[${P1.Length},${P1},100]} 
            /varset ACEndHP ${If[${P2.Length},${P2},0]} 
         } else /if (!${TankName.Arg[1,${PipeChar}].Equal[${Me.CleanName}]} && ${ADMobCount}) {
            /call GetTargetID ${TankName}
            /varset tTargetID ${Macro.Return}
            /if (${PullerName.Equal[${Me.CleanName}]}) {
               /if (!${tTargetID}) {
                  /if (${Spawn[${TankID}].Distance}<${ACLeash} && ${Target.ID}) {
                     /varset tTimer 4s
                     /while (!${tTargetID} && ${tTimer}) {
                        /call GetTargetID ${TankName}
                        /varset tTargetID ${Macro.Return}
                     }
                  }
               }
            }
            /if (${tTargetID} && ${ACMATarget} && ${ACTarID} && ${Spawn[${ACTarID}].ID} && ${Spawn[${ACTarID}].Type.NotEqual[corpse]} && ${Target.ID}==${ACTarID} && ${tTargetID}!=${Target.ID} && ${Melee.Combat} && (${ReTargetT} || ${Me.TargetOfTarget.CleanName.Equal[${Me.Name}]})) /return
            /if (${Target.ID}==${tTargetID} && ${ADMobCount}>1) {
               /varset ACTarID 0
               /varset ACMATarget 0
            }
            /for a ${ADMobMax} downto 1
               /if (${ADMobList[${a}]}) {
                  /if (${ADMobList[${a}]}!=${tTargetID}) {
                     /if (${Spawn[${ADMobList[${a}]}].ID}) {
                        /call ACValidTarget "${ADMobList[${a}]}" "Distance"
                        /if (${Macro.Return.Equal[TRUE]}) {
                           /varset ACTarID ${ADMobList[${a}]}
                           /if (${ADMobCount}>2 && ${a} > 1) {
                              /squelch /target id ${ACTarID}
                              /delay 1s ${Target.ID}==${ACTarID}
                              /if (${TankList.Find[${Me.TargetOfTarget.CleanName}]} && ${Me.TargetOfTarget.CleanName.NotEqual[${Me.Name}]}) {
                                 /varset ACTarID 0
                                 /squelch /target clear
                              }
                           }
                           /if (${ACTarID}) /break
                        }
                     }
                  }
               }
            /next a
            /if (${ACTarID}) {
               /if (${ACTarID}!=${Target.ID} && ${Spawn[${ACTarID}].ID} && ${Spawn[${ACTarID}].Type.NotEqual[corpse]} && !${ExcludeList.Find[${PipeChar}${Spawn[${ACTarID}].CleanName}${PipeChar}]}) {
                  /squelch /target id ${ACTarID}
                  /delay 1s ${Target.ID}==${ACTarID}
               }
               /if (${ACTarID}!=${Me.ID} && ${Spawn[${ACTarID}].ID} && ${Spawn[${ACTarID}].Type.NotEqual[corpse]} && ${ACTarID}!=${tTargetID}) /bc Off-tanking ${Spawn[id ${ACTarID}].CleanName}
               /varset ACBeginHP ${If[${P1.Length},${P1},100]} 
               /varset ACEndHP ${If[${P2.Length},${P2},0]}
               /varset ReTargetT 5s               
            } else {
               /varset ACTarID ${tTargetID}
            }
         } else /if (${TankName.Arg[1,${PipeChar}].Equal[${Me.CleanName}]} && ${ADMobCount}) {
            /call GetTargetID ${PullerName}
            /varset xTargetID ${Macro.Return}
            /if (!${xTargetID}) {
               /for a ${ADMobMax} downto 1
                   /if (${ADMobList[${a}]}) {
                     /if (${ADMobList[${a}]}!=${tTargetID}) {
                        /if (${Spawn[${ADMobList[${a}]}].ID}) {
                           /call ACValidTarget "${ADMobList[${a}]}" "Distance"
                           /if (${Macro.Return.Equal[TRUE]}) {
                              /varset ACTarID ${ADMobList[${a}]}
                              /break
                           }
                        }
                     }
                  }
               /next a
            } 
         }
         /if (!${ACTarID} || ${ACState}==100) {
            /call ACGetAssist ${TankName}
            /if (${Macro.Return}) {
               /varset ACTarID ${Macro.Return}
               /varset ACMATarget ${Macro.Return}
            }
         }
         /if (${ACTarID} && ${ACBeginHP}) {
            /if ((${Spawn[${ACTarID}].PctHPs}<${ACBeginHP} || ${Spawn[${ACTarID}].PctHPs}>=${ACEndHP}) && ${Spawn[${ACTarID}].Standing}) {
               /varset ACMATarget ${ACTarID}
            }
         } else {
            /varset ACMATarget 0
            /if (!${TankName.Arg[1,${PipeChar}].Equal[${Me.CleanName}]} && ${ACTarID} && ${Target.ID}==${ACTarID}) {
               /varset ACTarID ${Target.ID}
               /varset ACMATarget ${Target.ID}
            }
         }
         /if (${TankName.Arg[1,${PipeChar}].Equal[${Me.CleanName}]} && ${DoPull} && !${ACMATarget} && ${APTargetID}) {
            /varset ACMATarget ${APTargetID}
         }
      }
:GotTarget
      /varset ReTargetT 5s
      /if (${ACMATarget} && ${Select[${Spawn[${ACMATarget}].Type},npc,pet]} && !${Spawn[${ACMATarget}].Master.Type.Equal[pc]}) {
         /if (${Me.Pet.ID} && ${Me.Pet.ID}==${TankID}) {
            /varset tDistance ${Math.Distance[${Pet.Y},${Pet.X},${Pet.Z}:${Spawn[${ACMATarget}].Y},${Spawn[${ACMATarget}].X},${Spawn[${ACMATarget}].Z}]}
         } else {
            /varset tDistance ${Spawn[${ACMATarget}].Distance3D}
         }
      } else {
         /varcalc tDistance ${ACLeash}+1
      }
      /if (${tDistance}<=${ACLeash}) {
         /if (${ACState}==100 && ${Target.ID}!=${ACMATarget}) {
            /squelch /tar id ${ACMATarget}
            /delay 2s ${Target.ID}==${ACMATarget}
         }
         /if (!${OTID} || !${Spawn[${OTID}].Type.Equal[npc]}) {
            /varset OTID ${ACMATarget}
         }
         /if (${ACBefore.Length}>3 && ${OTID}==${ACMATarget}) {
            /call IniCommand "${ACBefore}"
         }
         /if (${ACAnnounce.Length}>5 && ${OTID}==${ACMATarget}) {
            /call ParseAnnounce "${ACAnnounce}" "${Spawn[${ACMATarget}].CleanName}" " "
         } else /if (${ACAnnounce.Length}>1 && ${ACAnnounce.Length}<6) {
            /docommand ${ACAnnounce} Meleeing: -[ ${Spawn[${ACMATarget}].CleanName} ]-
         }
         /if (${AdvPath.Following} && !${AdvPath.Paused}) {
            /varset ACState 2
            /varset FollowState 4
            /squelch /afollow pause
         } else {
            /if (${ACState}==100) {
               /if (${Math.Distance[${Me.Y},${Me.X}:${ACStartY},${ACStartX}]}>50 || !${ACStartY}) {
                  /varset ACStartX ${Spawn[${TankID}].X}
               } else {
                  /varset ACStartX ${Me.X}
               }
               /varset ACStartY ${Spawn[${TankID}].Y}
            } else {
               /varset ACStartX ${Me.X}
               /varset ACStartY ${Me.Y}
            }
            /varset ACStartHeading ${Me.Heading.DegreesCCW}
            /varset ACState 1
         }
      } else {
         /varset ACState 0
         /varset ACMATarget 0
         /varset ACTarID 0
         /varset ACBeginHP 0
         /if (${FollowState}==4) {
            /varset FollowState 1
            /squelch /afollow unpause
         }
      }
   }
   /if (${TankName.Equal[${Me.Name}]} && !${Target.ID} && ${CurrentMobCount}>0 && ${TankList.Length} > 2) {
      /if (!${ACMATarget} || ${Spawn[${ACMATarget}].Type.Equal[corpse]} || !${Spawn[${ACMATarget}].ID}) {
         /for ctank 1 to ${offtankcount}
            /varset ntank ${TankList.Arg[${ctank}]}
            /if (${NetBots[${ntank}].TargetID} && ${Spawn[${ntank}].ID}) {
               /call ACValidTarget "${NetBots[${TankList.Arg[${ctank}]}].TargetID}"
               /if (${Macro.Return.Equal[TRUE]}) {
                  /echo Switching to ${TankList.Arg[${ctank}]}'s target.
                  /varset ACMATarget ${NetBots[${TankList.Arg[${ctank}]}].TargetID}
               }
            }
         /next ctank
         /if (${ACMATarget}) /tar id ${ACMATarget}
      }
   }
   /doevents MobProb
   
   /if (!${ACState} && !${ACMATarget}) /return

   /if (!${ACMATarget} || ${ACState}>2 || ${Spawn[${ACMATarget}].Type.Equal[corpse]} || !${Spawn[${ACMATarget}].ID} || ${Math.Distance[${Me.Y},${Me.X}:${ACStartY},${ACStartX}]}>${ACLeash} && ${Spawn[${TankID}].Distance}>30) {
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
      /call BackPets
| Force a SpawnCheck
      /varset SpawnCheckTimer 0
      /call ADSpawnCheck
      /if (${Select[${ACState},2,4]} && (!${Me.Combat} && !${Melee.Combat} || !${ACMATarget} || ${Spawn[pc =${FollowName}].Distance}>${ACLeash} || !${CurrentMobCount})) {
         /if (${FollowState}==4) {
            /varset FollowState 1
            /squelch /afollow unpause
         }
      } else /if (!${FollowState} && !${ADMobCount}) {
         /if (${CampStatus}) {
            /varset CampStatus 1
            /delay 2s !${Me.Moving} || ${Math.Distance[${Me.Y},${Me.X}:${ACStartY},${ACStartX}]}<=10
         } else /if (${ReturnToLoc}) {
            /if (${Math.Distance[${Me.Y},${Me.X}:${ACStartY},${ACStartX}]}>10) {
               /squelch /moveto loc ${ACStartY} ${ACStartX} mdist 10
            }
         }
         /if (${UseHeadings}) {
            /squelch /face nolook ${If[${MacroQuest.BuildName.Equal[emu]},fast ,]}heading ${ACStartHeading}
         }
      }
      /if (${ACAfter.Length}>3) {
         /call IniCommand "${ACAfter}"
      }
      /if (${DoLoot}) {
         /if (!${ADMobCount} || ${MobsTooClose}<${Radius}) {
            /call LootMobs
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
         /call ACGetAssist "${TankName}" "ReTarget"
         /if (${Macro.Return}) {
            /varset ACMATarget ${Macro.Return}
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
|        /if (${Me.PctMana}>${ACManaPct} || !${Me.PctMana} || ${ACManaPct}==101) {
            /squelch /tar id ${ACMATarget}
            /delay 2s ${Target.ID}==${ACMATarget}
|        }
         /if (${Target.ID}!=${ACMATarget}) /return
      }
      /if (${Me.Pet.ID} && ${Me.Pet.ID}==${TankID}) {
         /varset tDistance ${Math.Distance[${Pet.Y},${Pet.X},${Pet.Z}:${Target.Y},${Target.X},${Target.Z}]}
      } else {
         /varset tDistance ${Target.Distance3D}
      }
      /if (${tDistance}<=${ACLeash}) {
         /if (${Me.Class.ShortName.Equal[BRD]}) {
            /if (${Twist.List.NotEqual[${BardCombatGems} ]} && ${BardCombatGems.Length}) {
               /if (${Me.State.Equal[sit]}) /stand
               /squelch /twist ${BardCombatGems}
               /if (${InfoMsgLevel}) {
                  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Msg: Combat gems
               }
            }
         }
         /call SendPets 
         /if (${AdvPath.Following} && !${AdvPath.Paused}) {
|           /varset ACState 2
            /varset FollowState 4
            /squelch /afollow pause
         }
         /if (${Me.PctMana}>${ACManaPct} || (${Me.MaxMana}==0 && ${ACManaPct}!=101)) {
            /if (!${Me.Mount.ID} && !${Me.Standing} && (!${Melee.Combat} || !${Me.Combat})) /stand
            /if (${DoMelee} && ${Me.Standing} && ${Target.ID}==${ACMATarget} && (!${Me.AutoFire} || !${Melee.Combat} || !${Me.Combat}) && (${Me.Class.ShortName.Equal[brd]} || (!${Me.Casting.ID} && !${Cast.Status.Find[C]}))) {
|              /if (${Spawn[${ACMATarget}].PctHPs}<=${ACAssistPct}) {
                  /killthis  
                  /varset ACMobProbT 3s
|              }
               /delay 1s ${Me.Combat}
               /if (${Me.Combat} && !${Stick.Status.Equal[on]} && (!${Me.Casting.ID} || ${Me.Class.ShortName.Equal[brd]})) {
                  /stick ${DefaultStick}
               }
            }
            /if (${DoRanged} && ${Me.Standing} && ${Target.ID}==${ACMATarget} && (!${Me.AutoFire} || !${Melee.Combat} || !${Me.Combat})) {
               /squelch /face nolook ${If[${MacroQuest.BuildName.Equal[emu]},fast ,]}${Target}
               /stick hold {Math.Calc[40+{Target.Height}]} moveback
               /autofire on
               /varset ACMobProbT 3s
            }
         } else /if (${Melee.Combat}) {
            /echo Low mana. Turning off melee.
            /squelch /tar clear
            /if (${Melee.Combat}) /attack off
            /delay 1s !${Melee.Combat}
            /squelch /stick off
         }
      }
   }
/return


Sub ACGetAssist(string tName,string tChecks)
   /if (!${tName.Length}) /return 0
   /if (${tChecks.Find[ReTarget]}) {
      /if (${NetBots[${tName.Arg[1,${PipeChar}]}].ID}) {
         /if (${NetBots[${tName.Arg[1,${PipeChar}]}].TargetID} && ${Target.ID}!=${NetBots[${tName.Arg[1,${PipeChar}]}].TargetID}) {
            /call ACValidTarget ${NetBots[${tName.Arg[1,${PipeChar}]}].TargetID}
            /if (${Macro.Return.Equal[TRUE]}) { 
               /return ${NetBots[${tName.Arg[1,${PipeChar}]}].TargetID}
            }
         }
         /return 0
      } else /if (${Target.ID}==${ACMATarget}) {
         /return 0
      }
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Assisting again to get target
   }
   /declare tTargetID int local
   /call GetTargetID ${tName}
   /if (!${Macro.Return}) /return 0   
   /varset tTargetID ${Macro.Return}
   /call ACValidTarget "${tTargetID}" "${tChecks}"
   /if (${Macro.Return.Equal[TRUE]}) /return ${tTargetID}
/return 0


Sub GetTargetID(string Name)
   /if (!${Name.Length}) /return 0
   /if (${NetBots[${Name.Arg[1,${PipeChar}]}].ID}) {
      /return ${NetBots[${Name.Arg[1,${PipeChar}]}].TargetID}
   } else /if (${Spawn[pc =${Name.Arg[1,${PipeChar}]}].ID}) {
      /if (${Spawn[pc =${Name.Arg[1,${PipeChar}]}].Distance}>=200) /return 0
   } else /if (${Spawn[mercenary =${Name}].ID}) {
      /if (${Spawn[mercenary =${Name}].Distance}>=200) /return 0
   } else {
      /return 0
   }
   /declare tTargetID int local 0
   /if (${Target.ID}) /squelch /target clear
   /assist ${Name}
   /delay 1s ${Target.ID}
   /varset tTargetID ${Target.ID}
/return ${tTargetID}


Sub ACValidTarget(int tTargetID,string tChecks)
   |/echo ${tTargetID} : ${Spawn[${tTargetID}].ID}
   /if (!${tTargetID}) /return FALSE
   /if (!${Spawn[${tTargetID}].ID}) /return FALSE
   /if (${Spawn[${tTargetID}].Type.NotEqual[npc]}) {
      /if (${Spawn[${tTargetID}].Type.Equal[pet]}) {
         /if (${IgnorePets}) /return FALSE
| Use Not=NPC here instead of =PC to handle new player zoning in and pet not showing master yet (NULL).
	     /if (${Spawn[${tTargetID}].Master.Type.NotEqual[npc]}) /return FALSE
      } else {
         /return FALSE
      }
   }
   |/if (${Spawn[${tTargetID}].Body.Name.Equal[object]} || ${Spawn[${tTargetID}].Race.Name.Equal[campfire]}) /return FALSE
   /if (${Spawn[${tTargetID}].Name.Left[1].Equal[_]} && ${Spawn[${tTargetID}].CleanName.Length}<2) /return FALSE
   /if (${Spawn[${tTargetID}].CleanName.Find[`s pet]} || ${Spawn[${tTargetID}].CleanName.Find[`s Animated Corpse]}) {
      /if (${Spawn[${Spawn[${tTargetID}].CleanName.Left[${Math.Calc[${Spawn[${tTargetID}].CleanName.Find[`s]}-1]}]} pc].ID}) /return FALSE
   }
   /if (${ExcludeList.Find[${PipeChar}${Spawn[${tTargetID}].CleanName}${PipeChar}]}) /return FALSE
   /if (${tChecks.Equal[ADSpawnCheck]}) /return TRUE
   /if (!${Spawn[${tTargetID}].Standing}) /return FALSE
   /if (${ExcludeList.Find[${PipeChar}${Spawn[${tTargetID}].CleanName}${PipeChar}]} || ${ExcludeListID.Find[${PipeChar}${tTargetID}${PipeChar}]}) /return FALSE
   /declare tDistance float local
   /if (${tChecks.Length}) {
      /if (${tChecks.Find[TypeNPC]}) {
         /if (!${Spawn[${tTargetID}].Type.Equal[npc]}) /return FALSE
      }
      /if (${tChecks.Find[PctHPS]}) {
         /if (${Spawn[${tTargetID}].PctHPs}>${ACAssistPct}) /return FALSE
      }
      /if (${tChecks.Find[Distance]}) {
         /if (${Me.Pet.ID} && ${Me.Pet.ID}==${TankID}) {
            /varset tDistance ${Math.Distance[${Pet.Y},${Pet.X},${Pet.Z}:${Spawn[${tTargetID}].Y},${Spawn[${tTargetID}].X},${Spawn[${tTargetID}].Z}]}
         } else {
            /varset tDistance ${Spawn[${tTargetID}].Distance3D}
         }
         /if (${tDistance}>${ACLeash}) /return FALSE
      }
   }
/return TRUE


Sub Event_EnrageStart(string Line,string MobName)
   /if (!${Me.Pet.ID}) /return
   /if (${ACPetEnrage} && ${Me.Pet.Target.CleanName.Equal[${MobName}]}) { 
      /call BackPets
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Backing off Pet
   }
/return


Sub Event_EnrageEnd(string Line,string MobName)
   /if (!${Me.Pet.ID}) /return
   /if (${ACPetEnrage} && ${Spawn[${ACMATarget}].CleanName.Equal[${MobName}]}) {
      /call SendPets
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Enrage ended, re-sending Pet.
   }
/return


Sub Event_MobProb(string Line)
   /doevents flush MobProb
   /if (${ACMobProbT}) /return
   /varset ACMobProbT 2s
   /if (!${Melee.Combat} && !${Me.Combat}) /return
   /if (${Me.Standing} && ${Target.ID}==${ACMATarget} && (${Me.Class.ShortName.Equal[brd]} || !${Me.Casting.ID})) {
      /if (${DoRanged}) {
         /squelch /face nolook ${If[${MacroQuest.BuildName.Equal[emu]},fast ,]}${Target}
      } else /if (${Stick.Status.Equal[off]}) {
         /stick ${DefaultStick}
      }
|      /if (${Target.Distance3D}<=${ACLeash} && ${Target.Distance3D}>${Target.MaxRangeTo} && !${Stick.Status.NotEqual[on]}) /squelch /stick ${Math.Calc[${Target.MaxRangeTo}-4]}
      /if (!${Melee.Combat}) /killthis
      /if (${Melee.Combat} && ${Stick.Status.NotEqual[on]} && ${Target.Distance}<=80) /squelch /stick ${DefaultStick}
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


Sub SendPets
   /if (!${ACMATarget}) /return
   /doevents flush PetNoWake
   /if (${Me.Pet.ID}) {
      /if (${TankName.Arg[1,${PipeChar}].Equal[${Me.CleanName}]} || ${PetAssist}) {
         /if (!${Pet.Combat} || ${Pet.Target.ID}!=${ACMATarget}) {  
            /pet attack ${ACMATarget}
         }
         /varset PetAssist ${ACMATarget}
      }
      /if (${Me.Pet.ID}==${TankID}) {
         /if (!${Pet.Taunt}) /pet taunt on
      }
   }
   /if (${SPetAssist} && ${Target.ID}==${ACMATarget} && ${SPetAssist}!=${ACMATarget}) {
      /pet swarm
      /varset SPetAssist ${ACMATarget}
   }
   /doevents PetNoWake
/return


Sub BackPets
   /if (!${Me.Pet.ID}) /return
   /pet back off
   /if (${PetAssist}) /varset PetAssist 1
   /if (!${Pet.Hold}) {
      /delay 2
      /pet hold on
   }
/return
