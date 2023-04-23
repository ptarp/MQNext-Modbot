
Sub LoadAQINIVals
   /declare a int local
   /call LoadINIVar AdvCure AQCount 0 AQCount "${IniFile}" int
   /call LoadINIVar AdvCure AQNewFormat 0 AQNewFormat "${IniFile}" int
   /declare AQCheckTimer timer outer
   /if (${AQCount}) {
      /call LoadINIVar AdvCure AQCheckTime 8 AQCheckTime "${IniFile}" int
      /declare AQGem[${AQCount}] string outer
      /declare AQSpell[${AQCount}] string outer
      /declare AQSpellCureType[${AQCount}] string outer
      /declare AQSpellMinMana[${AQCount}] string outer
      /declare AQSpellFoci[${AQCount}] string outer
      /declare AQSpellRecast[${AQCount}] int outer
      /declare AQTarCnt[${AQCount}] int outer
      /declare AQTarType[${AQCount}] string outer
      /declare AQSpellAlias[${AQCount}] string outer
      /declare AQAnnounce[${AQCount}] string outer
	  /declare AQCondition[${AQCount}] string outer
      /declare AQBardSlot[${AQCount}] int outer
	  /declare AQSpellRange[${AQCount}] int outer
	  /declare AQSplashSpell[${AQCount}] bool outer
      /declare tSpellID int local
      /for a 1 to ${AQCount}
         /if (${AQNewFormat}) {
            /call LoadINIVar AQ${a} Gem " " AQGem[${a}] ${IniFile}
            /call LoadINIVar AQ${a} Spell " " AQSpell[${a}] ${IniFile}
            /call LoadINIVar AQ${a} SpellFoci " " AQSpellFoci[${a}] ${IniFile}
            /call LoadINIVar AQ${a} SpellCureType "Cursed Diseased Poisoned Corrupted EnduDrain LifeDrain ManaDrain Blind CastingLevel Charmed Feared Healing Mesmerized Resistance Rooted Silenced Slowed Snared SpellCost SpellSlowed SpellDamage Trigger All" AQSpellCureType[${a}] ${IniFile}
            /call LoadINIVar AQ${a} SpellMinMana 0 AQSpellMinMana[${a}] ${IniFile}
            /call LoadINIVar AQ${a} SpellRecast " " AQSpellRecast[${a}] ${IniFile}
            /call LoadINIVar AQ${a} TarCnt " " AQTarCnt[${a}] ${IniFile}
            /call LoadINIVar AQ${a} TarType "grp cbt idle " AQTarType[${a}] ${IniFile}
            /call LoadINIVar AQ${a} SpellAlias " " AQSpellAlias[${a}] ${IniFile}
            /call LoadINIVar AQ${a} Announce " " AQAnnounce[${a}] ${IniFile}
			/call LoadINIVar AQ${a} PreCondition "TRUE" AQCondition[${a}] ${IniFile}
         } else {
            /call LoadINIVar AdvCure AQGem${a} " " AQGem[${a}] ${IniFile}
            /call LoadINIVar AdvCure AQSpell${a} " " AQSpell[${a}] ${IniFile}
            /call LoadINIVar AdvCure AQSpellFoci${a} " " AQSpellFoci[${a}] ${IniFile}
            /call LoadINIVar AdvCure AQSpellCureType${a} "Cursed Diseased Poisoned Corrupted EnduDrain LifeDrain ManaDrain Blind CastingLevel Charmed Feared Healing Mesmerized Resistance Rooted Silenced Slowed Snared SpellCost SpellSlowed SpellDamage Trigger All" AQSpellCureType[${a}] ${IniFile}
            /call LoadINIVar AdvCure AQSpellMinMana${a} 0 AQSpellMinMana[${a}] ${IniFile}
            /call LoadINIVar AdvCure AQSpellRecast${a} " " AQSpellRecast[${a}] ${IniFile}
            /call LoadINIVar AdvCure AQTarCnt${a} " " AQTarCnt[${a}] ${IniFile}
            /call LoadINIVar AdvCure AQTarType${a} "grp cbt idle " AQTarType[${a}] ${IniFile}
            /call LoadINIVar AdvCure AQSpellAlias${a} " " AQSpellAlias[${a}] ${IniFile}
            /call LoadINIVar AdvCure AQAnnounce${a} " " AQAnnounce[${a}] ${IniFile}
			/call LoadINIVar AdvCure AQPreCondition${a} "TRUE" AQCondition[${a}] ${IniFile}
         }
		 /varset tSpellID 0
         /if (${AQGem[${a}].NotEqual[script]} && ${AQSpell[${a}].Length}) {
            /if (${Select[${AQGem[${a}]},${ValidSpellGems}]}) {
               /if (${Me.Book[${Me.Book[${AQSpell[${a}]}]}].ID}) {
                  /varset AQSpell[${a}] ${Me.Book[${Me.Book[${AQSpell[${a}]}]}].Name}
				  /varset tSpellID ${Me.Book[${Me.Book[${AQSpell[${a}]}]}].ID}
               } else /if (${Me.Book[${Me.Book[${AQSpell[${a}]} ]}].ID}) {
                  /varset AQSpell[${a}] ${Me.Book[${Me.Book[${AQSpell[${a}]} ]}].Name}
				  /varset tSpellID ${Me.Book[${Me.Book[${AQSpell[${a}]}]}].ID}
               } else /if (${Me.Book[${Spell[${AQSpell[${a}]}].RankName}]}) {
                  /varset AQSpell[${a}] ${Spell[${AQSpell[${a}]}].RankName}
				  /varset tSpellID ${Me.Book[${Me.Book[${AQSpell[${a}]}]}].ID}
               } else {
                  /bc Msg: Spell ${AQSpell[${a}]} Not found in your book.  Check your .ini settings.
                  /varset AQTarCnt[${a}] 0
               }
            } else /if (${AQGem[${a}].Find[item]}) {
               /if (!${FindItem[=${AQSpell[${a}]}].ID}) {
                  /bc Msg: Cure Item not found: ${AQSpell[${a}]}. Check your .ini settings (or get more of the item).
               } else {
				  /varset tSpellID ${FindItem[=${AQSpell[${a}]}].Spell.ID}
			   }
            } else /if (${AQGem[${a}].Equal[alt]}) {
               /if (!${Me.AltAbility[${AQSpell[${a}]}].ID} && !${Me.Shrouded}) {
                  /bc Msg: You don't have the AltAbility ${AQSpell[${a}]}.  Check your .ini settings
                  /varset AQTarCnt[${a}] 0
               } else {
                  /varset tSpellID ${Me.AltAbility[${AQSpell[${a}]}].Spell.ID} 
               }
            } 
         }
         /if (${tSpellID}) {
		    /varset AQSpellRange[${a}] ${If[${Spell[${tSpellID}].AERange},${Spell[${tSpellID}].AERange},${Spell[${tSpellID}].Range}]}
            /if (${Spell[${tSpellID}].TargetType.Equal[free target]}) {
			   /varset AQSplashSpell[${a}] TRUE
			} else {
               /varset AQSplashSpell[${a}] FALSE
            }
         } else {
            /varset AQSpellRange[${a}] 0
		    /varset AQSplashSpell[${a}] FALSE
		 }
      /next a
   }
/return


Sub AQCureCheck
   /if (${MBPause} || ${AQCheckTimer}) /return
   /varset AQCheckTimer ${AQCheckTime}s

| Do a quick check to see if anyone needs a cure. To cut down on needless processing.   
   /declare x int local
   /for x 1 to ${NetBots.Counts}
      /if (${NetBots[${NetBots.Client.Arg[${x}]}].Detrimentals}>${NetBots[${NetBots.Client.Arg[${x}]}].NoCure}) /break
   /next x
   /if (${x}>${NetBots.Counts}) /return
   
   /declare a int local
   /declare c int local
   /declare i int local
   /declare tGem int local
   /declare tSpellID int local
   /declare tSpellRange int local
   /declare tCastTime timer local
   /declare tCT int local
   /declare tTargetID int local
   /declare AQtTargetID int local
   /declare tRecast int local
   /declare tPrevTarget int local
   /declare tSplashSpell bool local
   /declare tID int local
   /declare tName string local

   /for a 1 to ${AQCount}
      /if (!${DoCures} || ${MBPause} || ${NetBots[${Me.CleanName}].Silenced} || ${NetBots[${Me.CleanName}].Charmed} || ${Select[${Me.State},STUN,HOVER,DEAD]}) /return
      /if (!${AQTarCnt[${a}]}) /continue
      /if (!${AQTarType[${a}].Find[cbt]}) {
         /if (${ADMobFound}) /continue
      } else {
         /if (!${ADMobFound} && !${AQTarType[${a}].Find[idle]}) /continue
      }
      /if (${AQGem[${a}].Find[item]}) {
         /if (!${FindItem[=${AQSpell[${a}]}].ID} || ${FindItem[=${AQSpell[${a}]}].Timer}) /continue
      } else /if (${AQGem[${a}].Equal[alt]}) {
         /if (!${Me.AltAbility[${AQSpell[${a}]}].ID}) /continue
         /if (!${Me.AltAbilityReady[${AQSpell[${a}]}]} || ${Me.CurrentMana}<${Me.AltAbility[${AQSpell[${a}]}].Spell.Mana}) /continue	  
      }
	  
      /varset tTargetID 0	
	  
| Check Self only
      /if (${AQTarType[${a}].Find[self]}) {
         /if (${NetBots[${Me.CleanName}].Detrimentals}>${NetBots[${Me.CleanName}].NoCure}) {
            /if (${AQSpellCureType[${a}].Find[all]}) {
		       /varset tTargetID ${Me.ID}
			   /goto :cSpellFound
            }
            /for c 1 to ${Math.Calc[${AQSpellCureType[${a}].Count[ ]}+1]}
               /if (${NetBots[${Me.CleanName}].${AQSpellCureType[${a}].Arg[${c}]}}) {
                  /varset tTargetID ${Me.ID}
                  /break
			   }
            /next c
			/if (${tTargetID}) /goto :cSpellFound
         }
         /continue
      }
   
	  /varset tSpellRange ${AQSpellRange[${a}]}
	  /varset tSplashSpell ${AQSplashSpell[${a}]}

| Check Tank only
      /if (${AQTarType[${a}].Find[tank]}) {
         /if (${NetBots[${TankName}].Detrimentals}>${NetBots[${TankName}].NoCure}) {
            /if (!${AQTarType[${a}].Find[grp]} || ${Group.Member[${TankName}].Index}) {
               /varset tID ${Spawn[=${TankName}].ID}	  
               /if (!${tID}) /continue
			   /if (${tSpellRange}) {
			      /if (${Spawn[${tID}].Distance}>${tSpellRange}) /continue
			   }
			   /if (${tSplashSpell}) {
                  /if (!${Spawn[${tID}].CanSplashLand}) /continue
               }
               /if (${AQSpellCureType[${a}].Find[all]}) {
                  /varset tTargetID ${tID}
                  /goto :cSpellFound
		       }
               /for c 1 to ${Math.Calc[${AQSpellCureType[${a}].Count[ ]}+1]}
                  /if (${NetBots[${TankName}].${AQSpellCureType[${a}].Arg[${c}]}}) {
                     /varset tTargetID ${tID}
                     /break
                  }
               /next c
			   /if (${tTargetID}) /goto :cSpellFound
            }
         }
         /continue
      }
	  
| Check Myself
      /if (${AQTarType[${a}].Find[${Me.Class.ShortName}]} && ${NetBots[${Me.CleanName}].Detrimentals}>${NetBots[${Me.CleanName}].NoCure}) {
         /if (${AQSpellCureType[${a}].Find[all]}) {
		    /varset tTargetID ${Me.ID}
            /goto :cSpellFound
         }
         /for c 1 to ${Math.Calc[${AQSpellCureType[${a}].Count[ ]}+1]}
            /if (${NetBots[${Me.CleanName}].${AQSpellCureType[${a}].Arg[${c}]}}) {
		       /varset tTargetID ${Me.ID}
               /break
            }
         /next c
		 /if (${tTargetID}) /goto :cSpellFound
      }

| Check group members (other than myself)
      /if (${Group.Members}>0) {
         /for i 1 to ${Group.Members}
	        /if (${NetBots[${Group.Member[${i}].Name}].Detrimentals}>${NetBots[${Group.Member[${i}].Name}].NoCure}) {
               /if (${Group.Member[${i}].Present} && ${AQTarType[${a}].Find[${Group.Member[${i}].Class.ShortName}]}) {
                  /if (${tSpellRange}) {
			         /if (${Group.Member[${i}].Distance}>${tSpellRange}) /continue
			      }
			      /if (${tSplashSpell}) {
                     /if (!${Group.Member[${i}].CanSplashLand}) /continue
                  }
                  /if (${AQSpellCureType[${a}].Find[all]}) {
                     /varset tTargetID ${Group.Member[${i}].ID}
                     /break
				  }
                  /for c 1 to ${Math.Calc[${AQSpellCureType[${a}].Count[ ]}+1]}
                     /if (${NetBots[${Group.Member[${i}].Name}].${AQSpellCureType[${a}].Arg[${c}]}}) {
                        /varset tTargetID ${Group.Member[${i}].ID}
                        /break
                     }
                  /next c
		          /if (${tTargetID}) /break
               }
            }
         /next i
         /if (${tTargetID}) /goto :cSpellFound
      }  		 
		 
| Check NetBots members only out of group since group check already done.
      /if (!${AQTarType[${a}].Find[grp]}) {
         /for i 1 to ${NetBots.Counts}
		 	/varset tName ${NetBots.Client.Arg[${i}]}
	        /if (${NetBots[${tName}].Detrimentals} > ${NetBots[${tName}].NoCure}) {
			   /if (${NetBots[${tName}].InZone} && !${NetBots[${tName}].InGroup} && ${AQTarType[${a}].Find[${NetBots[${tName}].Class.ShortName}]}) {
                  /if (${tSpellRange}) {
			         /if (${Spawn[${NetBots[${tName}].ID}].Distance}>${tSpellRange}) /continue
			      }
                  /if (${tSplashSpell}) {
                     /if (!${Spawn[${NetBots[${tName}].ID}].CanSplashLand}) /continue
                  }
                  /if (${AQSpellCureType[${a}].Find[all]}) {
                     /varset tTargetID ${NetBots[${tName}].ID}
                     /break
                  }
                  /for c 1 to ${Math.Calc[${AQSpellCureType[${a}].Count[ ]}+1]}
                     /if (${NetBots[${tName}].${AQSpellCureType[${a}].Arg[${c}]}}) {
                        /varset tTargetID ${NetBots[${tName}].ID}
                        /break				  
                     }
                  /next c
		          /if (${tTargetID}) /break
               }
            }
         /next i
         /if (${tTargetID}) /goto :cSpellFound
      }	  		 
		 
      /continue
	  
:cSpellFound
      /if (${AQCondition[${a}].Length}>4) {
	     /varset PSTargetID ${tTargetID}
         /call IniCommand "${AQCondition[${a}]}"
         /if (${Macro.Return.NotEqual[TRUE]}) /continue
      }
      /if (${Corpse.Open}) {
	     /nomodkey /keypress esc
	  }
      /if (${Me.Ducking}) {
	     /nomodkey /keypress duck
	  }
|/echo ${If[${TimeStampOnEcho},[${Time}] ,]}Cure Needed 2 - ${Spawn[${tTargetID}].Name} - ${AQSpell[${a}]} 
      /if (${Select[${AQGem[${a}]},${ValidSpellGems}]}) {
|Spell
         /if (${Me.CurrentMana}<${Me.Book[${Me.Book[${AQSpell[${a}]}]}].Mana} || ${Me.PctMana}<${AQSpellMinMana[${a}]}) /break
         /varset tSpellID ${Me.Book[${Me.Book[${AQSpell[${a}]}]}].ID} 
         /varset tGem ${AQGem[${a}]} 
         /varcalc tCT ${Spell[${tSpellID}].MyCastTime}/100+40
         /if (${Me.Gem[${tGem}].ID}!=${tSpellID}) {
            /call MemGem ${tGem} "${AQSpell[${a}]}"
			/if (${Macro.Return.NotEqual[TRUE]}) {
               /continue
            }
         }
		 /if (${Me.Class.ShortName.NotEqual[BRD]} && ${Me.GemTimer[${tGem}]}) /continue
      } else /if (${AQGem[${a}].Find[item]}) {
|Item
         /varset tSpellID ${FindItem[=${AQSpell[${a}]}].Spell.ID}
         /varset tGem 20 
         /varcalc tCT ${FindItem[=${AQSpell[${a}]}].CastTime}/100+40
         /if (${Me.Class.ShortName.Equal[BRD]}) {
            /if (!${AQBardSlot[${a}]}) {
               /call GetBardSlot "${AQSpell[${a}]}" AQ|${a}
               /if (!${AQBardSlot[${a}]}) {
                  /bc Msg: No BardSlot assigned. Turning off Item casting for: ${AQSpell[${a}]}
                  /varset AQTarCnt[${a}] 0
                  /continue
               }
            }
         }
      } else /if (${AQGem[${a}].Equal[alt]}) {
|Alt
         /varset tSpellID ${Me.AltAbility[${AQSpell[${a}]}].Spell.ID} 
         /varset tGem 30 
         /varcalc tCT ${Me.AltAbility[${AQSpell[${a}]}].Spell.CastTime}/100+40
      } else /if (${AQGem[${a}].Equal[script]}) {
|Script
         /varset PSTargetID ${tTargetID}
         /call MBScript ${AQSpell[${a}]}
         /goto :tContCure
      }
      /if (!${tGem}) /continue

      /varset AQtTargetID 0
      /varset tRecast 0
      /varset tCastTime ${tCT}
:tCastCure
      /if (${NetBots[${Me.CleanName}].Silenced} || ${NetBots[${Me.CleanName}].Charmed} || ${Select[${Me.State},STUN,HOVER,DEAD]}) /return
      /doevents eqbc
      /if (${AQtTargetID}) {
         /if (${Spawn[${AQtTargetID}].Type.Equal[corpse]} || !${Spawn[${AQtTargetID}].ID}) { 
            /stopcast
            /continue
         }
         /if (${Cast.Status.Equal[i]}) {
            /if (${Cast.Result.Equal[cast_recover]}) { 
               /varset tCastTime ${tCT}+30
               /varset AQtTargetID 0
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}recovery time issue
               /goto :tCastCure
            }
            /if (${Cast.Result.Equal[cast_success]}) /goto :tContCure
            /if (${Select[${Cast.Result},cast_immune,cast_takehold]}) { 
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}immune or did not take hold
               /goto :tContCure
            }
            /if (${Select[${Cast.Result},cast_resist,cast_fizzle,cast_interrupted]}) {
               /if (!${AQSpellRecast[${a}]}) /continue
               /if (${AQSpellRecast[${a}]}>${tRecast}) {
                  /delay 2s ${Cast.Status.Equal[i]}
                  /varcalc tRecast ${tRecast}+1
                  /varset tCastTime ${tCT}+30
                  /varset AQtTargetID 0
                  /goto :tCastCure
               }
            }
| Everything else - cast_outofrange,cast_cannotsee,cast_distracted,cast_notarget,CAST_CANCELLED,CAST_NOTREADY - and whatever we missed
            /continue
         }
      } else /if (${Me.Class.ShortName.Equal[BRD]}) {
         /if (${Target.ID}!=${tTargetID}) {
            /varset tPrevTarget ${Target.ID}
            /if (${Melee.Combat}) /attack off
            /squelch /tar id ${tTargetID}
            /delay 1s ${Target.ID}==${tTargetID}
         } else {
            /varset tPrevTarget 0
         }
         /if (${Me.State.Equal[sit]}) /stand
         /varset AQtTargetID ${tTargetID}
         /if (${tGem}==30) {
            /alt act ${tSpellID}
            /delay 2
            /if (${tPrevTarget}) /target id ${tPrevTarget}
         } else /if (${tGem}==20) {
            /stopsong
            /squelch /twist once ${AQBardSlot[${a}]}
            /delay 3
            /if (${tPrevTarget}) /target id ${tPrevTarget}
            /delay ${Math.Calc[${tCT}-40]}
         } else {
            /stopsong
            /squelch /twist once ${AQGem[${a}]}
            /delay 5
            /if (${tPrevTarget}) /target id ${tPrevTarget}
            /delay ${Math.Calc[${tCT}-40]}
         }
         /goto :tContCure
      } else /if (${Cast.Ready[${AQSpell[${a}]}|${AQGem[${a}]}]}) {
         /if (${Target.ID}!=${tTargetID}) {
            /if (${Melee.Combat}) /attack off
            /squelch /tar id ${tTargetID}
            /delay 1s ${Target.ID}==${tTargetID}
         }
         /varset AQtTargetID ${tTargetID}
         /varset tCastTime ${tCT}
         /varset MBCurrentSpell AQ|${a}|${AQtTargetID}
         /call MQ2Cast "${AQSpell[${a}]}" ${If[${Select[${AQGem[${a}]},${ValidSpellGems}]},gem,]}${AQGem[${a}]} ${tCastTime} CastCheck -targetid|${AQtTargetID} ${If[${AQSpellFoci[${a}].Length},-bandolier|${AQSpellFoci[${a}]},]}
         /delay 5
      }
      /if (${tCastTime} || ${Cast.Timing} || ${Cast.Status.Equal[i]}) /goto :tCastCure
      /if (${MBDebug}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Cast took too long, aborting this cast ${Spell[${tSpellID}].Name}
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${Cast.Status} - ${Cast.Result} - ${AQtTargetID} ${Spawn[${AQtTargetID}].Name}
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}a=${a} tTargetID=${tTargetID} c=${c} 
      }
      /continue
:tContCure
      /varset MBCurrentSpell  
      /if (${AQAnnounce[${a}].Length}) {
         /if (!${Select[${Cast.Result},cast_immune,cast_takehold]} && !${CastResult.Equal[immune]}) {
            /call ParseAnnounce Curing: "${AQAnnounce[${a}]}" "${Spawn[${AQtTargetID}].CleanName}" "${AQSpell[${a}]}" 
         }
      }
      /return
   /next a
/return
