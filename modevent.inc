#Event AECEventA "#*#|${AECustomE1}|#*#"
#Event AECEventB "#*#|${AECustomE2}|#*#"
#Event AECEventC "#*#|${AECustomE3}|#*#"

Sub Event_AECEventA
   /if (${AECustomE1.Length} && ${AECustomE1.NotEqual[NULL]}) /call MBScript AECustomEvent1
/return


Sub Event_AECEventB
   /if (${AECustomE2.Length} && ${AECustomE2.NotEqual[NULL]}) /call MBScript AECustomEvent2
/return


Sub Event_AECEventC
   /if (${AECustomE2.Length} && ${AECustomE2.NotEqual[NULL]}) /call MBScript AECustomEvent3
/return


Sub LoadAEINIVals
   /declare a int local
   /declare b int local
   /declare AECustom int local 3
   /if (${AECustom}) {
      /for a 1 to ${AECustom}
          /call LoadINIVar AdvEvent AECustom${a} " " AECustomE${a} ${IniFile}
          /if (${AECustomE${a}.Length}<=2) /varset AECustomE${a} ${Math.Rand[999]}${Math.Rand[999]}${Math.Calc[${Me.Name.Length}*77]}${Math.Rand[999]}${Math.Rand[999]}
      /next a
   }
   /call LoadINIVar AdvEvent AECount 0 AECount "${IniFile}" int
   /call LoadINIVar AdvEvent AENewFormat 0 AENewFormat "${IniFile}" int
   /declare AECheckTimer timer outer
   /if (${AECount}) {
      /call LoadINIVar AdvEvent AECheckTime 8 AECheckTime "${IniFile}" int
      /declare AEGem[${AECount}] string outer
      /declare AESpell[${AECount}] string outer
      /declare AESpellFoci[${AECount}] string outer
      /declare AEDurMod[${AECount}] string outer
      /declare AEEventMinMana[${AECount}] int outer
      /declare AEEventMinHP[${AECount}] int outer
      /declare AEEventMinEndur[${AECount}] int outer
      /declare AEMinMana[${AECount}] int outer
      /declare AEMaxMana[${AECount}] int outer
      /declare AEMinHP[${AECount}] int outer
      /declare AEMaxHP[${AECount}] int outer
      /declare AEMinEndur[${AECount}] int outer
      /declare AEMaxEndur[${AECount}] int outer
      /declare AETarType[${AECount}] string outer
      /declare AESpellAlias[${AECount}] string outer
      /declare AEAnnounce[${AECount}] string outer
      /declare AEDelay[${AECount}] int outer
      /declare AETotDelay[${AECount}] int outer
      /declare AETarCnt[${AECount}] int outer
      /declare AEDiscTimer[${AECount}] int outer
      /declare AEBardSlot[${AECount}] int outer
	  /declare AEClass[${AECount}] string outer
      /declare tSpellID int local
      /for a 1 to ${AECount}
         /if (${AENewFormat}) {
            /call LoadINIVar AE${a} Gem " " AEGem[${a}] ${IniFile}
            /call LoadINIVar AE${a} Spell " " AESpell[${a}] ${IniFile}
            /call LoadINIVar AE${a} SpellFoci " " AESpellFoci[${a}] ${IniFile}
            /call LoadINIVar AE${a} DurMod 0 AEDurMod[${a}] ${IniFile}
            /call LoadINIVar AE${a} Delay 0 AEDelay[${a}] ${IniFile}
            /call LoadINIVar AE${a} EventMinMana 0 AEEventMinMana[${a}] ${IniFile}
            /call LoadINIVar AE${a} EventMinHP 0 AEEventMinHP[${a}] ${IniFile}
            /call LoadINIVar AE${a} EventMinEndur 0 AEEventMinEndur[${a}] ${IniFile}
            /call LoadINIVar AE${a} MinMana 0 AEMinMana[${a}] ${IniFile}
            /call LoadINIVar AE${a} MaxMana 101 AEMaxMana[${a}] ${IniFile}
            /call LoadINIVar AE${a} MinHP 0 AEMinHP[${a}] ${IniFile}
            /call LoadINIVar AE${a} MaxHP 101 AEMaxHP[${a}] ${IniFile}
            /call LoadINIVar AE${a} MinEndur 0 AEMinEndur[${a}] ${IniFile}
            /call LoadINIVar AE${a} MaxEndur 101 AEMaxEndur[${a}] ${IniFile}
            /call LoadINIVar AE${a} TarType "war shd pal rng mnk rog brd bst ber shm clr dru wiz mag enc nec self tnt" AETarType[${a}] ${IniFile}
            /call LoadINIVar AE${a} SpellAlias " " AESpellAlias[${a}] ${IniFile}
            /call LoadINIVar AE${a} Announce " " AEAnnounce[${a}] ${IniFile}
            /call LoadINIVar AE${a} TarCnt 1 AETarCnt[${a}] ${IniFile}
         } else {
            /call LoadINIVar AdvEvent AEGem${a} " " AEGem[${a}] ${IniFile}
            /call LoadINIVar AdvEvent AESpell${a} " " AESpell[${a}] ${IniFile}
            /call LoadINIVar AdvEvent AESpellFoci${a} " " AESpellFoci[${a}] ${IniFile}
            /call LoadINIVar AdvEvent AEDurMod${a} 0 AEDurMod[${a}] ${IniFile}
            /call LoadINIVar AdvEvent AEDelay${a} 0 AEDelay[${a}] ${IniFile}
            /call LoadINIVar AdvEvent AEEventMinMana${a} 0 AEEventMinMana[${a}] ${IniFile}
            /call LoadINIVar AdvEvent AEEventMinHP${a} 0 AEEventMinHP[${a}] ${IniFile}
            /call LoadINIVar AdvEvent AEEventMinEndur${a} 0 AEEventMinEndur[${a}] ${IniFile}
            /call LoadINIVar AdvEvent AEMinMana${a} 0 AEMinMana[${a}] ${IniFile}
            /call LoadINIVar AdvEvent AEMaxMana${a} 101 AEMaxMana[${a}] ${IniFile}
            /call LoadINIVar AdvEvent AEMinHP${a} 0 AEMinHP[${a}] ${IniFile}
            /call LoadINIVar AdvEvent AEMaxHP${a} 101 AEMaxHP[${a}] ${IniFile}
            /call LoadINIVar AdvEvent AEMinEndur${a} 0 AEMinEndur[${a}] ${IniFile}
            /call LoadINIVar AdvEvent AEMaxEndur${a} 101 AEMaxEndur[${a}] ${IniFile}
            /call LoadINIVar AdvEvent AETarType${a} "war shd pal rng mnk rog brd bst ber shm clr dru wiz mag enc nec self tnt" AETarType[${a}] ${IniFile}
            /call LoadINIVar AdvEvent AESpellAlias${a} " " AESpellAlias[${a}] ${IniFile}
            /call LoadINIVar AdvEvent AEAnnounce${a} " " AEAnnounce[${a}] ${IniFile}
            /call LoadINIVar AdvEvent AETarCnt${a} 1 AETarCnt[${a}] ${IniFile}
         }
         /if (${Select[${AEGem[${a}]},${ValidSpellGems}]} && ${AESpell[${a}].Length}) {
            /if (${Me.Book[${Me.Book[${AESpell[${a}]}]}].ID}) {
               /varset AESpell[${a}] ${Me.Book[${Me.Book[${AESpell[${a}]}]}].Name}
            } else /if (${Me.Book[${Me.Book[${AESpell[${a}]} ]}].ID}) {
               /varset AESpell[${a}] ${Me.Book[${Me.Book[${AESpell[${a}]} ]}].Name}
            } else /if (${Me.Book[${Spell[${AESpell[${a}]}].RankName}]}) {
               /varset AESpell[${a}] ${Spell[${AESpell[${a}]}].RankName}
            } else {
               /bc Msg: Spell ${AESpell[${a}]} Not found in your book. Please check your .ini settings.
               /varset AETarCnt[${a}] 0
            }
         } else /if (${AEGem[${a}].Find[item]}) {
            /if (!${FindItem[=${AESpell[${a}]}].ID}) {
               /bc Msg: Event Item not found: ${AESpell[${a}]}. Check your .ini settings (or get more of the item).
            } 
         } else /if (${AEGem[${a}].Equal[alt]}) {
            /if (!${Me.AltAbility[${AESpell[${a}]}].ID} && !${Me.Shrouded}) {
               /bc Msg: You don't have the AltAbility ${AESpell[${a}]}. Please check your .ini settings.
               /varset AETarCnt[${a}] 0
            }
         } else /if (${AEGem[${a}].Equal[disc]}) {
            /if (${Me.CombatAbility[${Spell[${AESpell[${a}]}].RankName}]}) {
               /varset AESpell[${a}] ${Spell[${AESpell[${a}]}].RankName}
            } else {
               /bc Msg: You don't have the Disc ${AESpell[${a}]}.  Please check AESpell${a} in your .ini
               /varset AETarCnt[${a}] 0
            }
         }
		 /if (${AETarType[${a}].Find[any]}) {
		    /varset AEClass[${a}] ${pcClasses}
		 } else {
		    /varset AEClass[${a}] 
		    /for b 1 to 16
               /if (${AETarType[${a}].Find[${pcClasses.Arg[${b}]}]}) {
			      /varset AEClass[${a}] ${pcClasses.Arg[${b}]} ${AEClass[${a}]}
		       }
			/next b
		 }
      /next a
   }
/return


Sub AEEventCheck
   /if (${MBPause} || ${AECheckTimer} || ${${FollowPlug}.Following} && !${${FollowPlug}.Idle}) /return
   /varset AECheckTimer ${AECheckTime}s
   /declare a int local
   /declare b int local
   /declare tCT int local
   /declare tSpellID int local
   /declare tGem int local
   /declare tSpellDuration int local
   /declare tSpellDelay int local
   /declare tCastTime timer local
   /declare AEtTargetID int local
   /declare tTargetID int local
   /declare tAEMob int local
   /declare tPrevTarget int local

   /call ADSpawnCheck Find
   /for a 1 to ${AECount}
      /if (!${DoEvents} || ${MBPause} || ${Select[${Me.State},HOVER,DEAD]}) /break
      /if (!${AETarCnt[${a}]}) /continue
|     /if (${AETotDelay[${a}]}>${Macro.RunTime}) /continue
      /if (${Me.PctHPs}<${AEEventMinHP[${a}]} || ${Me.PctMana}<${AEEventMinMana[${a}]} || ${Me.PctEndurance}<${AEEventMinEndur[${a}]}) /continue
      /if (!${AETarType[${a}].Find[cbt]}) {
         /if (${ADMobFound}) /continue
      } else {
         /if (!${AETarType[${a}].Find[idle]} && !${ADMobFound}) /continue
      }
	  /varset tTargetID 0
      /if (${AETarType[${a}].Find[self]}) {
         /if (${Me.PctMana}>${AEMinMana[${a}]} && ${Me.PctMana}<=${AEMaxMana[${a}]}) {
            /if (${Me.PctEndurance}>${AEMinEndur[${a}]} && ${Me.PctEndurance}<=${AEMaxEndur[${a}]}) {
               /if (${Me.PctHPs}>${AEMinHP[${a}]} && ${Me.PctHPs}<=${AEMaxHP[${a}]}) {			
                  /varset tTargetID ${Me.ID}
               }
			}
         }
      } else /if (${AETarType[${a}].Find[tnt]}) {
         /if (!${ACMATarget} || !${ACState} || !${Spawn[${ACMATarget}].ID} || ${Spawn[${ACMATarget}].Type.Equal[corpse]}) /continue
         /varset tTargetID ${ACMATarget}
      } else /if (${AEClass[${a}].Length}>2) {
         /for b 1 to ${NetBots.Counts}
            /if (!${Spawn[pc ${NetBots.Client.Arg[${b}]}].PctHPs} || ${Spawn[${NetBots.Client.Arg[${b}]}].Type.Equal[corpse]}) /continue
			/if (${AEClass.Find[${NetBots[${NetBots.Client.Arg[${b}]}].Class.ShortName}]}) {
			   /if (${NetBots[${NetBots.Client.Arg[${b}]}].PctMana}>${AEMinMana[${a}]} && ${NetBots[${NetBots.Client.Arg[${b}]}].PctMana}<=${AEMaxMana[${a}]} {
			      /if (${NetBots[${NetBots.Client.Arg[${b}]}].PctEndurance}>${AEMinEndur[${a}]} && ${NetBots[${NetBots.Client.Arg[${b}]}].PctEndurance}<=${AEMaxEndur[${a}]} {
			         /if (${NetBots[${NetBots.Client.Arg[${b}]}].PctHPs}>${AEMinHP[${a}]} && ${NetBots[${NetBots.Client.Arg[${b}]}].PctHPs}<=${AEMaxHP[${a}]} {
					    /varset tTargetID ${NetBots[${NetBots.Client.Arg[${b}]}].ID}
					 }
				  }
			   }
			}
			/if (${tTargetID}) /break
         /next b 
      }
	  /if (!${tTargetID}) /continue

      /varset tGem 0
      /varset tSpellID 0
      /varset tSpellDuration 0
      /if (${Select[${AEGem[${a}]},${ValidSpellGems}]}) {
|Spell
         /if (${Me.CurrentMana}<${Me.Book[${Me.Book[${AESpell[${a}]}]}].Mana}) /continue
         /varset tSpellID ${Me.Book[${Me.Book[${AESpell[${a}]}]}].ID}
         /varset tGem ${AEGem[${a}]}
         /varcalc tCT ${Spell[${tSpellID}].MyCastTime}/100+40
         /if (${Spell[${tSpellID}].Trigger.ID}) {
            /if (${Spell[${tSpellID}].Trigger.Duration}) {
               /varset tSpellDuration ${Spell[${tSpellID}].Trigger.Duration.TotalSeconds}
            }
		 } else /if (${Spell[${tSpellID}].Trigger[1].ID}) {
            /if (${Spell[${tSpellID}].Trigger[1].Duration}) {
               /varset tSpellDuration ${Spell[${tSpellID}].Trigger[1].Duration.TotalSeconds}
            }
         } else /if (${Spell[${tSpellID}].Duration}) {
            /varset tSpellDuration ${Spell[${tSpellID}].Duration.TotalSeconds}
         }
      } else /if (${AEGem[${a}].Find[item]}) {
|Item
         /if (!${FindItem[=${AESpell[${a}]}].ID} || ${FindItem[=${AESpell[${a}]}].Timer}) /continue
         /varset tSpellID ${FindItem[=${AESpell[${a}]}].Spell.ID}
         /varset tGem 20
         /varcalc tCT ${FindItem[=${AESpell[${a}]}].CastTime}/100+40
         /if (${Spell[${tSpellID}].Duration}) /varset tSpellDuration ${Spell[${tSpellID}].Duration.TotalSeconds}
         /if (${Me.Class.ShortName.Equal[BRD]}) {
            /if (!${AEBardSlot[${a}]}) {
               /call GetBardSlot "${AESpell[${a}]}" AE|${a}
               /if (!${AEBardSlot[${a}]}) {
                  /bc Msg: No BardSlot assigned. Turning off Item casting for: ${AESpell[${a}]}
                  /varset AETarCnt[${a}] 0
                  /continue
               }
            }
         }
      } else /if (${AEGem[${a}].Equal[alt]}) {
|Alt
         /if (!${Me.AltAbility[${AESpell[${a}]}].ID}) /continue
         /if (!${Me.AltAbilityReady[${AESpell[${a}]}]}) /continue
         /if (${Me.AltAbility[${AESpell[${a}]}].Spell.Trigger.ID}) {
            /if (${Me.CurrentMana}<${Me.AltAbility[${AESpell[${a}]}].Spell.Trigger.Mana}) /continue
            /if (${Me.AltAbility[${AESpell[${a}]}].Spell.Trigger.Duration}) {
               /varset tSpellDuration ${Me.AltAbility[${AESpell[${a}]}].Spell.Trigger.Duration.TotalSeconds}
            }
		 } else /if (${Me.AltAbility[${AESpell[${a}]}].Spell.Trigger[1].ID}) {
            /if (${Me.CurrentMana}<${Me.AltAbility[${AESpell[${a}]}].Spell.Trigger[1].Mana}) /continue
            /if (${Me.AltAbility[${AESpell[${a}]}].Spell.Trigger[1].Duration}) {
               /varset tSpellDuration ${Me.AltAbility[${AESpell[${a}]}].Spell.Trigger[1].Duration.TotalSeconds}
            }
         } else {
            /if (${Me.CurrentMana}<${Me.AltAbility[${AESpell[${a}]}].Spell.Mana}) /continue
            /if (${Me.AltAbility[${AESpell[${a}]}].Spell.Duration}) {
               /varset tSpellDuration ${Me.AltAbility[${AESpell[${a}]}].Spell.Duration.TotalSeconds}
            }
         }
         /varset tSpellID ${Me.AltAbility[${AESpell[${a}]}].Spell.ID}
         /varset tGem 30
         /varcalc tCT ${Me.AltAbility[${AESpell[${a}]}].Spell.MyCastTime}/100+40
      } else /if (${AEGem[${a}].Equal[script]}) {
|Script
         /varset PSTargetID ${tTargetID}
         /call MBScript ${AESpell[${a}]} 
         /varset tSpellDelay 0
         /varset tGem 40
         /varset tSpellID 0
         /goto :tContEvent
      } else /if (${AEGem[${a}].Equal[disc]}) {
|Disc
         /if (${AEDiscTimer[${a}]}>${Macro.RunTime} || !${Me.CombatAbilityReady[${AESpell[${a}]}]}) /continue
         /varset AEDiscTimer[${a}] ${Macro.RunTime}+${AEDelay[${a}]}
         /if (${tTargetID}) {
            /varset AEtTargetID ${tTargetID}
         } else {
            /varset AEtTargetID ${Me.ID}
         }
         /disc ${Spell[${AESpell[${a}]}].ID}
         /varset tSpellDelay 0
         /varset tGem 50
         /goto :tContEvent
      }

      /if (!${tGem} || !${tSpellID} || !${tTargetID}) /continue
	  /if (${tTargetID}!=${Me.ID}) {
         /if (${Spell[${tSpellID}].Range}) {
            /if (${Spawn[${tTargetID}].Distance}>${Spell[${tSpellID}].Range}) /continue
         } else /if (${Spell[${tSpellID}].AERange}) {
            /if (${Spawn[${tTargetID}].Distance}>${Spell[${tSpellID}].AERange}) /continue
	     }
      }
      /if (${tGem}<19) {
         /if (${Me.Gem[${tGem}].ID}!=${tSpellID}) {
            /call MemGem ${tGem} "${Spell[${tSpellID}]}"
            /if (${Macro.Return.NotEqual[TRUE]}) {
               /continue
            }
            /if (!${Cast.Ready[${tGem}]}) /varcalc tCT ${Spell[${tSpellID}].RecastTime}/100+${tCT}
         }
      }
      /if (${Spell[${tSpellID}].TargetType.Equal[self]}) /varset tTargetID 0
      /if (${Spell[${tSpellID}].TargetType.Find[group]}) {
	     /varset tTargetID ${If[${Target.Type.Equal[pc]},${Me.ID},0]}
	  }	 
      /varset AEtTargetID 0
      /varset tSpellDelay ${AEDelay[${a}]}
      /varset CastResult UNKNOWN
:tCastEvent
      /if (${NetBots[${Me.CleanName}].Silenced} || ${NetBots[${Me.CleanName}].Charmed} || ${Select[${Me.State},STUN,HOVER,DEAD]}) /break
      /if (${Spell[${tSpellID}].RecastTime.Float}>20 && ${Cast.Ready} && !${Cast.Ready[${tSpellID}|${AEGem[${a}]}]} && ${Select[${AEGem[${a}]},${ValidSpellGems}]}) /continue
      /if (${DoHeals} && ${AHCount}) /call AHHealCheck
      /if (${tGem}==50) {
         /varset AEDiscTimer[${a}] ${Macro.RunTime}+${AESpellRecast[${a}]}
         /disc ${tSpellID}
         /varset tSpellDelay 0
		 /varset AEtTargetID ${If[${tTargetID},${tTargetID},${Me.ID}]}
         /varset tSpellID 0
         /goto :tContEvent
      }
      /if (${AEtTargetID}) {
         /if (${Cast.Status.Equal[i]}) {
            /if (${Cast.Result.Equal[cast_success]}) /goto :tContEvent
            /if (${Select[${Cast.Result},cast_immune,cast_takehold]}) { 
               /varset tSpellDelay 99999
               /goto :tContEvent
            }
|           /if (${Select[${Cast.Result},cast_outofrange,cast_cannotsee,cast_distracted,cast_notarget,CAST_CANCELLED,CAST_NOTREADY]}) /continue
            /continue
         }
      } else /if (${Me.Class.ShortName.Equal[BRD]}) {
         /if (${Target.ID}!=${AEtTargetID}) {
            /varset tPrevTarget ${AEtTarget.ID}
            /if (${Melee.Combat}) /attack off
            /squelch /tar id ${AEtTargetID}
            /delay 1s ${Target.ID}==${AEtTargetID}
         } else {
            /varset tPrevTarget 0
         }
         /if (${Me.State.Equal[sit]}) /stand
         /if (${tGem}==30) {
            /alt act ${Me.AltAbility[${AESpell[${a}]}].ID}
            /delay 2
            /if (${tPrevTarget}) /target id ${tPrevTarget}
         } else /if (${tGem}==20) {
            /stopsong
            /squelch /twist once ${AEBardSlot[${a}]}
            /delay 3
            /if (${tPrevTarget}) /target id ${tPrevTarget}
            /delay ${Math.Calc[${tCT}-40]}
         } else {
            /stopsong
            /squelch /twist once ${AEGem[${a}]}
            /delay 5
            /if (${tPrevTarget}) /target id ${tPrevTarget}
            /delay ${Math.Calc[${tCT}-40]}
         }
         /goto :tContEvent
      } else /if (${Cast.Ready[${AESpell[${a}]}|${AEGem[${a}]}]}) {
         /varset AEtTargetID ${tTargetID}
         /varset tCastTime ${tCT}
         /varset MBCurrentSpell AE|${a}|${AEtTargetID}
         /call MQ2Cast "${AESpell[${a}]}" ${If[${Select[${AEGem[${a}]},${ValidSpellGems}]},gem,]}${AEGem[${a}]} ${tCastTime} CastCheck -targetid|${AEtTargetID} ${If[${AESpellFoci[${a}].Length},-bandolier|${AESpellFoci[${a}]},]}
		 /if (${interruptFlag}) /break
         /if (!${AEtTargetID}) /varset AEtTargetID ${Me.ID}
         /if (!${Select[${Cast.Result},cast_immune,cast_takehold,cast_outofrange,cast_cannotsee,cast_distracted,cast_notarget]}) /goto :tContEvent
      }
|     /if (${interruptFlag}) /break
      /if (${tCastTime} || ${Cast.Timing}) /goto :tCastEvent
      /if (${MBDebug}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}WTF - Event : ${AESpell[${a}]}
      }
      /continue
:tContEvent
      /varset MiscCheckTimer 5s
      /if (${AEAnnounce[${a}].Length}) {
         /call ParseAnnounce Evented: "${AEAnnounce[${a}]}" "${Spawn[${AEtTargetID}].CleanName}" "${AESpell[${a}]}"
      }
      /if (${tSpellDuration} && ${tGem}<40) {
         /if (${AEDurMod[${a}]}) {
            /varcalc tSpellDuration ((${AEDurMod[${a}]}/100)*${tSpellDuration})+${tSpellDuration}-${Spell[${tSpellID}].MyCastTime.TotalSeconds}-5
         } else {
            /varcalc tSpellDuration ${tSpellDuration}-${Spell[${tSpellID}].MyCastTime.TotalSeconds}-5
         }
         /varcalc tSpellDelay ${tSpellDelay}+${AEDelay[${a}]}+${tSpellDuration}+${Macro.RunTime}
      } else {
         /varcalc tSpellDelay ${tSpellDelay}+${AEDelay[${a}]}+${Macro.RunTime}-${Spell[${tSpellID}].MyCastTime.TotalSeconds}-5
      }
      /if (${AEDelay[${a}]}) {
         /if (!${tSpellDelay} || ${tSpellDelay}<${AEDelay[${a}]}) /varcalc tSpellDelay ${AEDelay[${a}]}+${Macro.RunTime}
      }
      /if (${Spell[${tSpellID}].RecastTime.TotalSeconds} && (${tSpellDelay}<=${Macro.RunTime} || ${Math.Calc[${tSpellDelay}-${Macro.RunTime}]}<${Spell[${tSpellID}].RecastTime.TotalSeconds})) /varset tSpellDelay ${Spell[${tSpellID}].RecastTime.TotalSeconds}
      /varcalc AETotDelay[${a}] ${tSpellDelay}
      /varset MBCurrentSpell
	  /doevents
      /break
   /next a
/return