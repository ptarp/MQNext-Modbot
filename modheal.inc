
#Event AHDA "#*#can't cast spells while invulnerable#*#"

Sub LoadAHINIVals
   /declare a int local
   /declare b int local
   /call LoadINIVar AdvHeal AHCount 0 AHCount "${IniFile}" int
   /call LoadINIVar AdvHeal AHNewFormat 1 AHNewFormat "${IniFile}" int
   /declare AHCheckTimer timer outer
   /declare AHHealList string outer |

   /if (${AHCount}) {
      /call LoadINIVar AdvHeal AHCheckTime 2 AHCheckTime "${IniFile}" int
      /call LoadINIVar AdvHeal AHHealOOBC TRUE AHHealOOBC "${IniFile}" bool
      /call LoadINIVar AdvHeal AHHealMode "0|0|12" AHHealMode "${IniFile}" string
      /call LoadINIVar AdvHeal AHXTgtWatch " " AHXTgtWatch "${IniFile}" string
      /declare AHGem[${AHCount}] string outer
      /declare AHSpell[${AHCount}] string outer
      /declare AHSpellFoci[${AHCount}] string outer
      /declare AHSpellAlias[${AHCount}] string outer
      /declare AHDurMod[${AHCount}] string outer
      /declare AHSpellMinMana[${AHCount}] int outer 0
      /declare AHClass[${AHCount}] string outer
      /declare AHTarCnt[${AHCount}] int outer 0
      /declare AHTargetName string outer
      /declare AHAnnounce[${AHCount}] string outer
      /declare AHCondition[${AHCount}] string outer
      /declare AHThreshold[${AHCount},6] int outer 0
      /declare AHBardSlot[${AHCount}] int outer
      /declare tSpellID int local
      /declare PRSpell int outer 0
      /declare AHXTgtCount int outer 0
      /declare AHtCnt int outer 0
      /declare AHtSpellID int outer 0
      /declare AHtMembers string outer
      /declare tHurt int outer 0
      /declare AHtPreID int outer 0
      /declare tMembers string outer
      /declare HealOnlyTimer timer outer 0
      /declare AHtTargetID int outer 0
      /declare AHbTargetID int outer 0
      /declare NeedtoHeal bool outer
      /declare AHPrevTarget int outer 
      /declare tPRSpell bool outer  
      /if (${Me.XTargetSlots} && ${AHXTgtWatch.Length}) {
         /declare AHXTgtHeal[${Me.XTargetSlots}] int outer 0
         /declare tArg int local
         /for a 1 to ${Math.Calc[${AHXTgtWatch.Count[,]}+1]}
            /if (${Select[${AHXTgtWatch.Arg[${a},,]},1,2,3,4,5,6,7,8,9,10,11,12,13]}) {
               /varset tArg ${AHXTgtWatch.Arg[${a},,]}
               /if (${tArg}<=${Me.XTargetSlots} && ${AHXTgtCount}<${Me.XTargetSlots}) {
                  /varcalc AHXTgtCount ${AHXTgtCount}+1
                  /varset AHXTgtHeal[${AHXTgtCount}] ${tArg}
               }
            }
         /next a
      } else {
         /declare AHXTgtHeal[1] int outer 0
      }

      /for a 1 to ${AHCount}
         /if (${AHNewFormat}) {
            /call LoadINIVar AH${a} Gem " " AHGem[${a}] ${IniFile}
            /call LoadINIVar AH${a} Spell " " AHSpell[${a}] ${IniFile}
            /call LoadINIVar AH${a} SpellFoci " " AHSpellFoci[${a}] ${IniFile}
            /call LoadINIVar AH${a} DurMod 0 AHDurMod[${a}] ${IniFile}
            /call LoadINIVar AH${a} SpellMinMana 0 AHSpellMinMana[${a}] ${IniFile}
            /call LoadINIVar AH${a} SpellAlias " " AHSpellAlias[${a}] ${IniFile}
            /call LoadINIVar AH${a} Announce " " AHAnnounce[${a}] ${IniFile}
            /call LoadINIVar AH${a} TarCnt 0 AHTarCnt[${a}] ${IniFile}
            /call LoadINIVar AH${a} Class "pc pet group hp0 war shd pal rng mnk rog brd bst ber shm clr dru wiz mag enc nec tnt mypet self" AHClass[${a}] ${IniFile}
            /call LoadINIVar AH${a} PreCondition "TRUE" AHCondition[${a}] ${IniFile}
         } else {
            /call LoadINIVar AdvHeal AHGem${a} " " AHGem[${a}] ${IniFile}
            /call LoadINIVar AdvHeal AHSpell${a} " " AHSpell[${a}] ${IniFile}
            /call LoadINIVar AdvHeal AHSpellFoci${a} " " AHSpellFoci[${a}] ${IniFile}
            /call LoadINIVar AdvHeal AHDurMod${a} 0 AHDurMod[${a}] ${IniFile}
            /call LoadINIVar AdvHeal AHSpellMinMana${a} 0 AHSpellMinMana[${a}] ${IniFile}
            /call LoadINIVar AdvHeal AHSpellAlias${a} " " AHSpellAlias[${a}] ${IniFile}
            /call LoadINIVar AdvHeal AHAnnounce${a} " " AHAnnounce[${a}] ${IniFile}
            /call LoadINIVar AdvHeal AHTarCnt${a} 0 AHTarCnt[${a}] ${IniFile}
            /call LoadINIVar AdvHeal AHClass${a} "pc pet group hp0 war shd pal rng mnk rog brd bst ber shm clr dru wiz mag enc nec tnt mypet self" AHClass[${a}] ${IniFile}
            /call LoadINIVar AdvHeal AHPreCondition${a} "TRUE" AHCondition[${a}] ${IniFile}
         }
         /if (${AHGem[${a}].NotEqual[script]} && ${AHSpell[${a}].Length}>2) {
            /if (${Select[${AHGem[${a}]},${ValidSpellGems}]}) {
               /if (${Me.Book[${Me.Book[${AHSpell[${a}]}]}].ID}) {
                  /varset AHSpell[${a}] ${Me.Book[${Me.Book[${AHSpell[${a}]}]}].Name}
               } else /if (${Me.Book[${Me.Book[${AHSpell[${a}]} ]}].ID}) {
                  /varset AHSpell[${a}] ${Me.Book[${Me.Book[${AHSpell[${a}]} ]}].Name}
               } else /if (${Me.Book[${Spell[${AHSpell[${a}]}].RankName}]}) {
                  /varset AHSpell[${a}] ${Spell[${AHSpell[${a}]}].RankName}
               } else {
                  /bc Msg: Spell ${AHSpell[${a}]} Not found in your book.  Check your .ini settings.
                  /varset AHTarCnt[${a}] 0
               } 
               /if (${Spell[${AHSpell[${a}]}].Subcategory.Equal[Delayed]} && ${Spell[${AHSpell[${a}]}].HasSPA[44]}) {
                  /varset PRSpell ${a}
                  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}PR-type spell identified - ${AHSpell[${a}]}
               }
            } else /if (${AHGem[${a}].Equal[alt]}) {
               /if (!${Me.AltAbility[${AHSpell[${a}]}].ID} && !${Me.Shrouded}) {
                  /bc Msg: You don't have the AltAbility ${AHSpell[${a}]}.  Check your .ini settings
                  /varset AHTarCnt[${a}] 0
               }
            } 
            /if (${AHGem[${a}].Find[item]}) {
               /if (${FindItem[${AHSpell[${a}]}].Spell.Name.NotEqual[NULL]}) /varset AHHealList ${AHHealList}${FindItem[${AHSpell[${a}]}].Spell.Name}|
               /if (!${FindItem[=${AHSpell[${a}]}].ID}) {
                  /bc Msg: Heal Item not found: ${AHSpell[${a}]}. Check your .ini settings (or get more of the item).
               }
            } else /if (${AHGem[${a}].Equal[alt]}) {
               /if (${Me.AltAbility[${AHSpell[${a}]}].Spell.Name.NotEqual[NULL]}) /varset AHHealList ${AHHealList}${Me.AltAbility[${AHSpell[${a}]}].Spell.Name}|
            } else {
               /varset AHHealList ${AHHealList}${AHSpell[${a}]}|
            }
         }
| Build the Threshold Heal amounts
| 1 - PCs
         /if (${AHClass[${a}].Find[self]}) {
            /call CheckAHArgs "${AHClass[${a}]}"|self
            /if (${Macro.Return}>0) /varset AHThreshold[${a},1] ${Macro.Return}
         } else /if (${AHClass[${a}].Find[pc]}) {
            /for b 1 to 16
               /if (${AHClass[${a}].Find[${pcClasses.Arg[${b}]}]}) {
                  /call CheckAHArgs "${AHClass[${a}]}"|${pcClasses.Arg[${b}]} 
                  /if (${Macro.Return} && ${Macro.Return}!=${AHThreshold[${a},1]}) {
| 5- Set to "1" if more than one heal threshold for the Class string
                     /if (${AHThreshold[${a},1]}) /varset AHThreshold[${a},5] 1 
                     /if (${Macro.Return}>${AHThreshold[${a},1]}) /varset AHThreshold[${a},1] ${Macro.Return}
                  }
               }
            /next b
         }
| 2 - Pets
         /if (${AHClass[${a}].Find[mypet]}) {
            /call CheckAHArgs "${AHClass[${a}]}"|mypet
            /if (${Macro.Return}>0) /varset AHThreshold[${a},2] ${Macro.Return}
         } else /if (${AHClass[${a}].Find[pet]}) {
            /call CheckAHArgs "${AHClass[${a}]}"|pet
            /if (${Macro.Return}>0) /varset AHThreshold[${a},2] ${Macro.Return}
         }
| 3 - Tank
         /if (${AHClass[${a}].Find[tank]} && ${AHClass[${a}].Find[pc]}) {
            /call CheckAHArgs "${AHClass[${a}]}"|tank
            /if (${Macro.Return}>0) /varset AHThreshold[${a},3] ${Macro.Return}
         }
| 4 - Grp
         /if (${AHClass[${a}].Find[grp]}) {
            /call CheckAHArgs "${AHClass[${a}]}"|grp
            /if (${Macro.Return}>0) /varset AHThreshold[${a},4] ${Macro.Return}
         }
| 6 - XTgt
         /if (${AHXTgtCount} && ${AHClass[${a}].Find[xtgt]}) {
            /call CheckAHArgs "${AHClass[${a}]}"|xtgt
            /if (${Macro.Return}>0) /varset AHThreshold[${a},6] ${Macro.Return}
         }
| /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${a} - ${AHThreshold[${a},1]} - ${AHThreshold[${a},2]} - ${AHThreshold[${a},3]} - ${AHThreshold[${a},4]} - ${AHThreshold[${a},5]} - ${AHThreshold[${a},6]}
      /next a
   }
/return


Sub AHHealCheck
   /if (${MBPause} || ${AHCheckTimer}) /return
   /varset AHCheckTimer ${AHCheckTime}s
   /declare a int local
   /declare b int local
   /declare x int local
   /declare tGem int local
   /declare tSpellID int local
   /declare tSpellRange int local
   /declare AHtDurMod int local
   /declare mString string local
   /declare tTarCnt int local
   /declare tHealClass string local
   /declare tgtLevel int local
   /declare tgtName string local
   /declare tTargetID int local
   /declare tgotoTop bool local
   /declare giveUpTimer timer 0
   /if (!${AHHealList.Find[${Me.Casting.Name}]} || !${Me.Casting.ID}) {
      /varset AHtCnt 0
      /varset AHtSpellID 0
      /varset AHtMembers 
      /varset tHurt 0
      /varset AHtPreID 0
      /varset tMembers 
      /varset AHbTargetID 0
      /varset NeedtoHeal FALSE
      /varset AHPrevTarget 0
      /varset tPRSpell FALSE
      /varset AHtTargetID 0
   }
| Note: If you are a Bard, AHtTargetID will never be non-zero. Just to keep that in mind as to why we don't make sure it's not a Bard on some of the spell casting stuff.
   /while (1) {
      /doevents AHDA
      /for a 1 to ${AHCount}
         /if (!${DoHeals} || ${MBPause} || ${NetBots[${Me.CleanName}].Silenced} || ${NetBots[${Me.CleanName}].Charmed} || ${Select[${Me.State},STUN,HOVER,DEAD]}) /return
         /if (${Me.Ducking}) {
		    /nomodkey /keypress duck
		 }
         /if (!${AHTarCnt[${a}]}) /continue
         /if (!${AHtTargetID}) /doevents
         /varset tGem 0
         /varset tSpellID 0
         /varset tgotoTop FALSE
         /if (${AHtTargetID}) {
            /if (!${Me.Casting.ID}) /break
            /if (!${giveUpTimer}) {
               /if (${MBDebug}) {
                  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}no timera - ${Time} - ${Macro.RunTime} - ${MBCurrentSpell} - ${Cast.Status} - ${Cast.Result} - ${Cast.Stored} - ${AHtTargetID} - ${Me.Casting.ID} 
               }
               /call StuckGems
            }
         }
         /if (${Select[${AHGem[${a}]},${ValidSpellGems}]}) {
|Spell
            /if (${Me.CurrentMana}<${Me.Book[${Me.Book[${AHSpell[${a}]}]}].Mana} || ${Me.PctMana}<=${AHSpellMinMana[${a}]}) /continue
            /varset tSpellID ${Me.Book[${Me.Book[${AHSpell[${a}]}]}].ID} 
            /varset tGem ${AHGem[${a}]}
            /if (${Me.Gem[${tGem}].ID}!=${tSpellID}) {
			   /call MemGem ${tGem} "${Spell[${tSpellID}]}"
               /if (${Macro.Return.NotEqual[TRUE]}) {
                  /continue
               }
            }
            /if (${Me.Class.ShortName.NotEqual[BRD]} && ${Me.GemTimer[${tGem}]}) /continue
         } else /if (${AHGem[${a}].Find[item]}) {
|Item
            /if (!${FindItem[=${AHSpell[${a}]}].ID} || ${FindItem[=${AHSpell[${a}]}].Timer}) /continue
            /varset tSpellID ${FindItem[=${AHSpell[${a}]}].Spell.ID} 
            /varset tGem 20
            /if (${Me.Class.ShortName.Equal[BRD]}) {
               /if (!${AHBardSlot[${a}]}) {
                  /call GetBardSlot "${AHSpell[${a}]}" AH|${a}
                  /if (!${AHBardSlot[${a}]}) {
                     /bc Msg: No BardSlot assigned. Turning off Item casting for: ${AHSpell[${a}]}
                     /varset AHTarCnt[${a}] 0
                     /continue
                  }
               }
            }
         } else /if (${AHGem[${a}].Equal[alt]}) {
|Alt
            /if (!${Me.AltAbilityReady[${AHSpell[${a}]}]}) /continue
            /if (${Me.CurrentMana}<${Me.AltAbility[${AHSpell[${a}]}].Spell.Mana} || ${Me.PctMana}<${AHSpellMinMana[${a}]} || !${Me.AltAbility[${AHSpell[${a}]}].ID}) /continue
            /varset tSpellID ${Me.AltAbility[${AHSpell[${a}]}].Spell.ID} 
            /varset tGem 30
         } else /if (${AHGem[${a}].Equal[script]}) {
|Script
            /varset tGem 40
            /varset tSpellID 0
         } else /if (${AHGem[${a}].Equal[ability]}) {
|Ability
            /if (!${Me.AbilityReady[${AHSpell[${a}]}]}) /continue
            /varset tSpellID 0
            /varset tSpellRange 50
            /varset tGem 50
         }
         /if (!${tGem}) /continue
         /varset tSpellRange ${If[${Spell[${tSpellID}].AERange} && ${tGem}<39,${Spell[${tSpellID}].AERange},${Spell[${tSpellID}].Range}]}
         /varset tHurt 0
         /varset tMembers
         /call GrpEval "${AHClass[${a}]}" ${tSpellRange} ${AHTarCnt[${a}]} ${a}
         /if (${Macro.Return.Equal[FALSE]}) /continue
| This is not worth it, IMHO. When tested, many times I would see 100% HPs, but the heal would land for a large amount. So, use the above.
|        /if (${Macro.Return.Equal[FALSE]}) {
|           /if (${AHtTargetID} && ${AHtTargetID}==${AHbTargetID}) {
|              /if (${AHtSpellID} && ${AHtSpellID}==${Me.Casting.ID}) {
|                 /if (${Spawn[${AHtTargetID}].PctHPs}>99 && !${Spell[${Me.Casting.ID}].Duration}) {
|                    /echo ${If[${TimeStampOnEcho},[${Time}] ,]}interrupt no heal needed
|                    /stopcast
|                 }
|              }
|           }
|           /continue
|        }
         /if (${Macro.Return.Length}) {
            /varset mString ${Macro.Return}
            /varset mString ${mString.Replace[""",]}
            /varset tMembers ${mString.Arg[1,|]}
            /varset tHurt ${mString.Arg[2,|]}
         }
         /if (${AHTarCnt[${a}]}>${tHurt} && !${AHClass[${a}].Find[grp]}) /continue
         /varset tTarCnt ${AHTarCnt[${a}]}
         /varset tHealClass ${AHClass[${a}]}
         /varset tPRSpell FALSE
         /for b 1 to ${tHurt}
            /varset AHbTargetID ${tMembers.Arg[${b}]}
            /if (!${Spawn[${AHbTargetID}].ID} || ${Spawn[${AHbTargetID}].Type.Equal[corpse]}) /continue
            /if (${AHbTargetID}==${Me.ID}) {
               /if (${tTarCnt}==1 && ${Me.Song[${AHSpell[${a}]}].ID}) /continue
            }
            /if (${AHCondition[${a}].NotEqual[TRUE]}) {
               /if (${AHCondition[${a}].Left[4].Find[/if ]}) {
			      /varset PSTargetID ${AHbTargetID}
                  /call IniCommand "${AHCondition[${a}]}"
                  /if (${Macro.Return.NotEqual[TRUE]}) /break
               } else /if (${AHCondition[${a}].NotEqual[pr]}) {
			      /varset PSTargetID ${AHbTargetID}
                  /call MBScript ${AHCondition[${a}]} ${AHbTargetID} ${tSpellID}    	
                  /if (!${Macro.Return}) /break
| Check the lowest PR Spell stacking to check for PR Buff
               } else /if (${Spawn[${AHbTargetID}].Type.Equal[pc]}) {
                  /if (!${NetBots[${tgtName}].Stacks[9755]}) /break
               } else {
                  /if (!${NetBots[${Spawn[${AHbTargetID}].Master.CleanName}].StacksPet[10342]}) /break
               }
            }
            /if (${Corpse.Open}) {
			   /nomodkey /keypress esc
			}
            /varset tgtName ${Spawn[${AHbTargetID}].Name}
            /if (${Select[${Spawn[${AHbTargetID}].Class.ShortName},shd,nec,mnk,brd]}) {
               /if (!${NetBots[${tgtName}].Feigning} || ${NetBots[${tgtName}].CombatState}==0) {
                  /if (${Spawn[${AHbTargetID}].Distance}>${ACLeash}) /continue
               }
            }
            /if (${tHealClass.Find[tnt]}) {
               /if (!${ACMATarget} || !${ACState} || !${Spawn[${ACMATarget}].ID} || ${Spawn[${ACMATarget}].Type.Equal[corpse]}) /break
               /varset AHbTargetID ${ACMATarget}
            } else /if (${tHealClass.Find[self]}) {
               /if (!${tMembers.Find[${Me.ID}]}) /break
               /varset AHbTargetID ${Me.ID}
            } else {
               /if (${a}==${PRSpell}) {
| Check the lowest PR Spell stacking to check for PR Buff
                  /if (${Spawn[${AHbTargetID}].Type.Equal[pc]}) {
                     /if (!${NetBots[${tgtName}].Stacks[9755]} || !${NetBots[${tgtName}].FreeBuffSlots}) /break
                  } else {
                     /if (!${NetBots[${Spawn[${AHbTargetID}].Master.CleanName}].StacksPet[10342]}) /break
                  }
               }
               /if (${NetBots[${tgtName}].ShortBuff.Find[${tSpellID}]}) /break
               /if (${Spell[${tSpellID}].Duration}) {
                  /if (${tTarCnt}==1 && ${tGem}<20) {
                     /if (${Defined[${tgtName}${a}]}) {
                        /if (${${tgtName}${a}}>0) /break
                     }
                     /varset tgtLevel ${If[${Spawn[${AHbTargetID}].Type.Equal[pc]},${Spawn[${AHbTargetID}].Level},${Spawn[${AHbTargetID}].Master.Level}]}
                     /if (${Spell[${tSpellID}].Level}<255 && ${tgtLevel}<61) {
                        /if (${Spell[${tSpellID}].Level}>65) /break
                        /if (${tgtLevel}<46 && ${Spell[${tSpellID}].Level}>60) /break
                     }
                  }
               }
               /if (${tHealClass.Find[mypet]} && ${AHbTargetID}!=${Me.Pet.ID}) /continue
               /if (${Spawn[${AHbTargetID}].Master.Type.Equal[pc]} && ${Select[${Spawn[${AHbTargetID}].Master.Class.ShortName},clr,wiz]}) /continue
               /if (${Spell[${tSpellID}].TargetType.Equal[free target]}) {
                  /if (!${Spawn[${AHbTargetID}].CanSplashLand}) /continue
               }
               /if (!${AHHealOOBC}) {
                  /if (${Spawn[${AHbTargetID}].Type.Equal[pc]}) {
                     /if (!${NetBots.Client.Find[${tgtName}]}) /continue
                  } else /if (${Spawn[${AHbTargetID}].Master.Type.Equal[pc]}) {
                     /if (!${NetBots.Client.Find[${Spawn[${AHbTargetID}].Master.CleanName}]}) /continue
                  }
               }
               /if (!${Cast.Status.Equal[i]} && ${Me.Class.ShortName.NotEqual[BRD]}) {
                  /if (${Me.Casting.ID} && !${Spell[${Me.Casting.ID}].IsSkill} && (!${AHHealList.Find[${Me.Casting.Name}]} || (${Me.Casting.ID}==13 && ${tHurt}>1 && ${AHTarCnt[${a}]}>1))) {
                     /if (${InfoMsgLevel}) {
                        /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Interrupting <${Me.Casting.Name}> to cast <${AHSpell[${a}]}> on <${tgtName}>
                     }
                     /call interrupt
                     /varset AHtTargetID 0
                     /delay 1s ${Cast.Status.Equal[i]}
                     /if (${Me.Casting.ID}) {
                        /call StuckGems
                     }
                  }
               }
               /if (${Spell[${tSpellID}].TargetType.Find[Group]}) {
                  /varset AHbTargetID 99999
               } else /if (${Spell[${tSpellID}].TargetType.Equal[pet]} && ${tMembers.Find[${AHbTargetID}]} && ${Me.Pet.ID}) {
                  /varset AHbTargetID ${Me.Pet.ID}
               }
            }
            /if (${AHtTargetID}) {
               /if (${Me.Casting.ID}) /break
               /if (${Cast.Status.Equal[i]} && ${Cast.Result.Equal[CAST_INTERRUPTED]}) { 
                  /varset AHtTargetID 0
                  /varset tgotoTop TRUE
                  /break
               }
            }
            /if (${tGem}>=40) {
               /if (${Target.ID}!=${AHbTargetID} && ${AHbTargetID}) /squelch /tar id ${If[${AHbTargetID}==99999,${Me.ID},${AHbTargetID}]}
               /if (${tGem}==40) {
                  /varset PSTargetID ${Target.ID}
				  /call MBScript ${AHSpell[${a}]}
               } else {
                  /doability ${AHSpell[${a}]}
               }
               /return
            }
            /varset NeedtoHeal TRUE
            /if (${TankName.Equal[${Me.CleanName}]}) {
			   /if (${Target.ID}==${ACMATarget}) /varset AHPrevTarget ${Target.ID}
			}
            /if (${Me.Class.ShortName.Equal[BRD]}) {
               /if (${Target.ID}!=${AHbTargetID} && ${Spell[${tSpellID}].TargetType.Find[Single]}) {
                  /varset AHPrevTarget ${Target.ID}
                  /if (!${Spell[${tSpellID}].SpellType.Find[Beneficial]}) {
                     /squelch /stoptwist
                  }
                  /if (${Melee.Combat}) /attack off
                  /squelch /tar id ${AHbTargetID}
                  /delay 1s ${Target.ID}==${AHbTargetID}
               } else {
                  /varset AHPrevTarget 0
               }
               /if (${tGem}==30) {
                  /alt act ${Me.AltAbility[${AHSpell[${a}]}].ID}
                  /delay 2
               } else /if (${tGem}==20) {
                  /stopsong
                  /squelch /twist once ${AHBardSlot[${a}]}
                  /delay 2
                  /delay ${Math.Calc[${FindItem[=${AHSpell[${a}]}].CastTime}/100+3]}
               } else {
                  /stopsong
                  /squelch /twist once ${tGem}
                  /delay 5
                  /delay ${Math.Calc[${Spell[${AHSpell[${a}]}].MyCastTime}/100]}
               }
			   /if (${AHPrevTarget}) /target id ${AHPrevTarget}
               /if (${AHAnnounce[${a}].Length}) {
                  /call ParseAnnounce Healing: "${AHAnnounce[${a}]}" "${Spawn[${AHbTargetID}].CleanName}" "${AHSpell[${a}]}"
               }
               /return
            }
            /if (!${AHtTargetID}) {
               /if (${tGem}<20) {
                  /if (!${Cast.Status.Equal[i]} || ${Me.GemTimer[${tGem}]}) /break
               } else {
                  /if (!${Cast.Ready[${AHSpell[${a}]}|${AHGem[${a}]}]}) /break
               }
               /if (${Me.State.Equal[sit]}) /stand
               /if (${Stick.Status.Equal[on]} || ${MakeCamp.Status.Equal[on]}) {
                  /squelch /stick pause
                  /delay 1s !${Me.Moving}
               }
               /if (${AdvPath.Following}) {
                  /if (!${AdvPath.Paused} && ${FollowFlag}<2) {
                     /varset FollowFlag 3
                     /squelch /afollow pause
                  }
               }
               /varset tTargetID ${AHbTargetID}
               /varset MBCurrentSpell AH|${a}|${If[${tTargetID}==99999,0,${tTargetID}]}
               /if (${tTargetID}==99999) /varset tTargetID ${Me.ID}

               /casting "${AHSpell[${a}]}|${AHGem[${a}]}" ${If[!${tTargetID}, ,-targetID|${tTargetID}]} ${If[${AHSpellFoci[${a}].Length},-bandolier|${AHSpellFoci[${a}]},]} -maxtries|2
               /varset SpellWasCast TRUE

               /if (${AHHealMode.Arg[1,|]}) {
                  /if (${a}<=${AHHealMode.Arg[2,|]}) /varset HealOnlyTimer ${AHHealMode.Arg[3,|]}s
               }
               /varset AHtTargetID ${tTargetID}
               /varset AHtSpellID ${tSpellID}
               /varset AHtCnt ${a}
               /varset AHtMembers ${tMembers}
               /if (!${AHtTargetID} || ${AHtTargetID}==99999) /varset AHtTargetID ${Me.ID}
               /if (${AHAnnounce[${a}].Length}) {
                  /call ParseAnnounce Healing: "${AHAnnounce[${a}]}" "${Spawn[${AHtTargetID}].CleanName}" "${AHSpell[${a}]}"
               }
               /if (${tTargetID} && ${tTargetID}!=99999) /varset AHtPreID ${tTargetID}
               /if (${AHtCnt}==${PRSpell}) /varset tPRSpell TRUE
| If we are casting 'early' with MQ2Cast plugin, we need to wait for the actual cast to start.
               /while (${Cast.Status.Find[C]}) {
                  /if (${Me.Casting.ID}) /break
               }
               /varcalc giveUpTimer ${Spell[${AHSpell[${a}]}].MyCastTime}/100+50
            }
            /break
         /next b
         /if (${tgotoTop} || ${tPRSpell}) /break
      /next a
      /if (${tgotoTop}) /continue
      /if (${AHtTargetID}) {
         /if (${Me.Casting.ID}) {
            /if (!${giveUpTimer}) {
               /delay 5
               /bc ${If[${TimeStampOnEcho},[${Time}] ,]}no timerb - ${Time} - ${Macro.RunTime} - ${MBCurrentSpell} - ${Cast.Status} - ${Cast.Result} - ${Cast.Stored} - ${AHtTargetID} - ${Me.Casting.ID}
               /call StuckGems
            }
            /if (!${tPRSpell}) /continue
         }
         /if (${Spell[${AHtSpellID}].Duration}) {
            /if (${AHDurMod[${AHtCnt}]}) {
               /varcalc AHtDurMod ((${Spell[${AHtSpellID}].Duration.TotalSeconds}*(${AHDurMod[${a}]}/100))+${Spell[${AHtSpellID}].Duration.TotalSeconds}*10)
            } else /if (${Spell[${AHtSpellID}].MyDuration}) {
               /varcalc AHtDurMod ${Spell[${AHtSpellID}].MyDuration.TotalSeconds}*10
            } else {
               /varcalc AHtDurMod ${Spell[${AHtSpellID}].Duration.TotalSeconds}*10
            }
            /if (${AHtCnt}==${PRSpell}) {
               /varcalc AHtDurMod ${AHtDurMod}+60
            }
            /if (${Spell[${AHtSpellID}].AERange} && ${AHtMembers.Count[ ]}) {
               /for x 1 to ${Math.Calc[${AHtMembers.Count[ ]}]}
                  /varset mString ${Spawn[${AHtMembers.Arg[${x}]}].Name}${AHtCnt}
                  /if (!${Defined[${mString}]}) {
                     /declare ${mString} timer outer
                  }
                  /varset ${mString} ${AHtDurMod}
               /next x
            } else {
               /varset mString ${Spawn[${AHtTargetID}].Name}${AHtCnt}
               /if (!${Defined[${mString}]}) {
                  /declare ${mString} timer outer
               }
               /varset ${mString} ${AHtDurMod}
            }
         }
         /varset AHtTargetID 0
         /varset MBCurrentSpell
         /if (${NeedtoHeal} || ${HealOnlyTimer} || ${AHHealList.Find[${Me.Casting.Name}]}) /continue
      }
      /if (${HealOnlyTimer}) {
         /varset AHtTargetID 0
         /varset MBCurrentSpell
         /continue
      }
      /break
   }
|  /varset MBCurrentSpell
   /if (!${AHHealList.Find[${Me.Casting.Name}]}) /varset AHtTargetID 0
   /if (${AdvPath.Paused} && ${FollowFlag}==3) /squelch /afollow unpause
   /if (${AHPrevTarget}) /target id ${AHPrevTarget}
/return


Sub GrpEval(string AHLine, int radius, int tarcnt, int a)
| Check self/mypet, grp, and tank HP. Then group members and xtargets. Then look for netbots clients out of group and pets (use Netbots only if out of group). Return string of ID's with count as last arg.
   /if (${AHLine.Find[self]}) {
      /if (${Me.PctHPs}<${AHThreshold[${a},1]}) /return "${Me.ID}|1"
      /return FALSE
   }
   /if (${AHLine.Find[mypet]}) {
      /if (${Me.Pet.ID} && ${Me.Pet.PctHPs}<${AHThreshold[${a},2]}) {
         /if (!${radius} || ${Me.Pet.Distance}<=${radius}) /return "${Me.Pet.ID}|1"
      }
      /return FALSE
   }
| Check Grp (Group.Injured)
   /if (${AHLine.Find[grp]}) {
      /if (${Group.Injured[${AHThreshold[${a},4]}]}>=${tarcnt}) /return "${Me.ID}|1"
      /return FALSE
   }
   /declare TankID int local
   /declare tPctHPs int local
| Check Tank
   /if (${AHLine.Find[tank]} && ${AHLine.Find[pc]} && ${TankName.Length}) {
      /if (!${AHLine.Find[group]} || ${Group.Member[${TankName}].Index}) {
         /varset TankID ${Spawn[=${TankName}].ID}
         /if (${TankID}) {
            /varset tPctHPs ${If[${Group.Member[${TankName}].Index} || !${NetBots[${TankName}].ID},${Spawn[${TankID}].PctHPs},${NetBots[${TankName}].PctHPs}]}
            /if (${tPctHPs}<${AHThreshold[${a},3]}) {
               /if (!${radius} || ${Spawn[${TankID}].Distance}<=${radius}) /return "${TankID}|1"
            }
         }
      }
   }
   /if (!${AHThreshold[${a},1]} && !${AHThreshold[${a},2]} && !${AHThreshold[${a},6]}) /return FALSE
   /declare i int local
   /declare matches string local
   /declare totalcount int local 0
   /declare Lowest int local 0
| Check myself	
   /if (${AHLine.Find[${Me.Class.ShortName}]} && ${AHLine.Find[pc]} && ${Me.PctHPs}<${AHThreshold[${a},1]}) {
      /if (${AHThreshold[${a},5]}) {
         /call CheckAHArgs "${AHLine}"|${Me.Class.ShortName}
         /if (!${Macro.Return} || ${Macro.Return}<${Me.PctHPs}) /goto :tCheckmypet
      }
      /varset Lowest ${Me.PctHPs}
      /varset matches ${Me.ID} ${matches}
      /varcalc totalcount ${totalcount}+1
   }
| Check my pet
:tCheckmypet
   /if (${Me.Pet.ID} && ${AHLine.Find[pet]} && ${Me.Pet.PctHPs}<${AHThreshold[${a},2]}) {
      /if (${Select[${Me.Class.ShortName},shm,nec,mag,bst,dru,enc,shd]} && !${Me.Pet.Name.Find[familiar]}) {
         /varset matches ${matches} ${Me.Pet.ID}
         /varcalc totalcount ${totalcount}+1
      }
   }
| Check group members/pets (other than myself/my pet)
   /if (${Group.Members}>0) {
      /if (${Group.Injured[${AHThreshold[${a},1]}]}>=1) {
         /for i 1 to ${Group.Members}
            /if (${Group.Member[${i}].Present} && ${Group.Member[${i}].PctHPs}<${AHThreshold[${a},1]}) {
               /if (!${Group.Member[${i}].Type.Equal[corpse]} && ${AHLine.Find[${Group.Member[${i}].Class.ShortName}]}) {
                  /if (${AHThreshold[${a},5]}) {
                     /call CheckAHArgs "${AHLine}"|${Group.Member[${i}].Class.ShortName}
                     /if (!${Macro.Return} || ${Macro.Return}<${Group.Member[${i}].PctHPs}) /continue
                  }
                  /if (!${radius} || ${Group.Member[${i}].Distance}<=${radius}) {
                     /if (!${Lowest} || ${Group.Member[${i}].PctHPs}<${Lowest}) {
                        /varset Lowest ${Group.Member[${i}].PctHPs}
                        /varset matches ${Group.Member[${i}].ID} ${matches}
                     } else {
                        /varset matches ${matches} ${Group.Member[${i}].ID}
                     }
                     /varcalc totalcount ${totalcount}+1
                  }
               }
            }
         /next i
      }
      /if (${AHThreshold[${a},2]}) { 
         /for i 1 to ${Group.Members}
            /if (${Group.Member[${i}].Pet.ID} && ${Group.Member[${i}].Pet.PctHPs}<${AHThreshold[${a},2]}) {
               /if (${Select[${Group.Member[${i}].Class.ShortName},shm,nec,mag,bst,dru,enc,shd]} && !${Group.Member[${i}].Pet.Name.Find[familiar]}) {
                  /if (!${radius} || ${Group.Member[${i}].Pet.Distance}<=${radius}) { 
                     /varset matches ${matches} ${Group.Member[${i}].Pet.ID}
                     /varcalc totalcount ${totalcount}+1
                  }
               }
            }
         /next i
      }
   }
| Check XTargets
   /if (${AHThreshold[${a},6]}) { 
      /for i 1 to ${AHXTgtCount}
         /if (${Me.XTarget[${AHXTgtHeal[${i}]}].ID} && !${Me.XTarget[${AHXTgtHeal[${i}]}].Type.Equal[corpse]}) {
            /if (${Me.XTarget[${AHXTgtHeal[${i}]}].Type.Equal[pc]} || ${Me.XTarget[${AHXTgtHeal[${i}]}].Master.Type.Equal[pc]}) {
               /if (${Me.XTarget[${AHXTgtHeal[${i}]}].PctHPs}<${AHThreshold[${a},6]}) {
                  /if (!${radius} || ${Me.XTarget[${AHXTgtHeal[${i}]}].Distance}<=${radius}) {
                     /if (!${Lowest} || ${Me.XTarget[${AHXTgtHeal[${i}]}].PctHPs}<${Lowest}) {
                        /varset Lowest ${Me.XTarget[${AHXTgtHeal[${i}]}].PctHPs}
                        /varset matches ${Me.XTarget[${AHXTgtHeal[${i}]}].ID} ${matches}
                     } else {
                        /varset matches ${matches} ${Me.XTarget[${AHXTgtHeal[${i}]}].ID}
                     }
                     /varcalc totalcount ${totalcount}+1
                  }
               }
            }
         }
      /next i
   }
| Check NetBots members/pets only out of group since group check already done.
   /if (!${AHLine.Find[group]} && (${AHThreshold[${a},1]} || ${AHThreshold[${a},2]})) { 
      /for i 1 to ${NetBots.Counts}
         /if (${Group.Member[${NetBots.Client.Arg[${i}]}].Index} || ${NetBots.Client.Arg[${i}].Equal[${Me.CleanName}]}) /continue
         /if (${AHThreshold[${a},1]}) { 
            /if (!${Spawn[pc ${NetBots.Client.Arg[${i}]}].PctHPs} || ${Spawn[${NetBots.Client.Arg[${i}]}].Type.Equal[corpse]}) /continue
            /if (${NetBots[${NetBots.Client.Arg[${i}]}].PctHPs}<${AHThreshold[${a},1]} && ${AHLine.Find[${NetBots[${NetBots.Client.Arg[${i}]}].Class.ShortName}]}) {
               /if (${AHThreshold[${a},5]}) {
                  /call CheckAHArgs "${AHLine}"|${NetBots[${NetBots.Client.Arg[${i}]}].Class.ShortName}
                  /if (!${Macro.Return} || ${Macro.Return}<${NetBots[${NetBots.Client.Arg[${i}]}].PctHPs}) /goto :tCheckNBpet
               }
               /if (!${radius} || ${Spawn[pc ${NetBots.Client.Arg[${i}]}].Distance}<=${radius}) { 
                  /if (!${Lowest} || ${NetBots[${NetBots.Client.Arg[${i}]}].PctHPs}<${Lowest}) { 
                     /varset Lowest ${NetBots[${NetBots.Client.Arg[${i}]}].PctHPs}
                     /varset matches ${NetBots[${NetBots.Client.Arg[${i}]}].ID} ${matches}
                  } else {
                     /varset matches ${matches} ${NetBots[${NetBots.Client.Arg[${i}]}].ID}
                  }
                  /varcalc totalcount ${totalcount}+1
               }
            }
         }
:tCheckNBpet
         /if (${AHThreshold[${a},2]}) { 
            /if (${NetBots[${NetBots.Client.Arg[${i}]}].PetID} && ${NetBots[${NetBots.Client.Arg[${i}]}].PetHP}<${AHThreshold[${a},2]}) {
               /if (${Select[${NetBots[${NetBots.Client.Arg[${i}]}].Class.ShortName},shm,nec,mag,bst,dru,enc,shd]} && !${Spawn[${NetBots[${NetBots.Client.Arg[${i}]}].PetID}].Name.Find[familiar]}) {
                  /if (!${radius} || ${Spawn[${NetBots[${NetBots.Client.Arg[${i}]}].PetID}].Distance}<=${radius}) {
                     /varset matches ${matches} ${NetBots[${NetBots.Client.Arg[${i}]}].PetID}
                     /varcalc totalcount ${totalcount}+1
                  }
               }
            }
         } 
      /next i
   }
   /if (${matches.Length} && ${totalcount}) /return "${matches}|${totalcount}"
/return FALSE


Sub CheckAHArgs(string CheckString)
   /declare AHLine string local ${CheckString.Arg[1,|]}
   /declare ClassChk string local ${CheckString.Arg[2,|]}
   /declare ParseLine string local
   /if (${AHLine.Find[${ClassChk}]}) {
| Find if class shortname has a value, if not - use hp setting
:tRecheck
      /if (${AHLine.Arg[1].Find[${ClassChk}]}) {
         /varset ParseLine ${AHLine.Arg[1]}
      } else {		
         /varset ParseLine ${AHLine.Right[-${Math.Calc[${AHLine.Find[${ClassChk}]}-1]}]}
      }	
      /if (${AHLine.Find[hp]} && ${ClassChk.NotEqual[hp]} && (!${ParseLine.Arg[1].Find[${ClassChk}]} || ${ParseLine.Arg[1].Length}==${ClassChk.Length} || ${ParseLine.Arg[1].Equal[NULL]})) {
         /varset ClassChk hp
         /goto :tRecheck
      }
      /if (${ParseLine.Arg[1].Length}!=${ClassChk.Length}) /call IsNumber ${ParseLine.Arg[1].Right[-${ClassChk.Length}]}
      /if (${Macro.Return.Length} && ${Macro.Return.NotEqual[FALSE]}) /return ${ParseLine.Arg[1].Right[-${ClassChk.Length}]}
   }
|  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem determining heal percentage for Class ${Class} in AHClass line of ${AHLine}
/return FALSE


Sub event_AHDA
   /docommand /${BeepCommand}
   /bc Msg: DA on ME
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}DA on ME
   /doevents flush AHDA
/return