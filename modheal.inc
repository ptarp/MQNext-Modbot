
#Event AHDA "You can't cast spells while invulnerable#*#"

Sub LoadAHINIVals
   /declare a int local
   /declare b int local
   /call LoadINIVar AdvHeal AHCount 0 AHCount "${IniFile}" int
   /call LoadINIVar AdvHeal AHNewFormat ${DefaultNewFormat} AHNewFormat "${IniFile}" int
   /call LoadINIVar AdvHeal AHCheckTime 2 AHCheckTime "${IniFile}" int
   /call LoadINIVar AdvHeal AHHealOOBC TRUE AHHealOOBC "${IniFile}" bool
   /call LoadINIVar AdvHeal AHHealMode "0${PipeChar}0${PipeChar}12" AHHealMode "${IniFile}" string
   /call LoadINIVar AdvHeal AHXTgtWatch " " AHXTgtWatch "${IniFile}" string
   /if (${Me.Class.ShortName.Equal[clr]}) {
      /call LoadINIVar AdvHeal UseDivArb "TRUE" UseDivArb ${IniFile} bool
      /call LoadINIVar AdvHeal DivArbPct "35" DivArbPct ${IniFile} int
   }
   /if (!${Defined[UseDivArb]}) /declare UseDivArb bool outer FALSE
   /if (!${Defined[DivArbPct]}) /declare DivArbPct int outer 0
   /declare AHCheckTimer timer outer
   /declare AHHealList string outer ${PipeChar}
   /declare AHTargetName string outer
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

| If setting up new ini, create a dummy entry to show a template
   /if (${MBNewIni}) {
      /call LoadAHIniEntry 0
   }
   
   /if (${AHCount}) {
      /declare AHGem[${AHCount}] string outer
      /declare AHSpell[${AHCount}] string outer
      /declare AHSpellFoci[${AHCount}] string outer
      /declare AHSpellAlias[${AHCount}] string outer
      /declare AHDurMod[${AHCount}] string outer
      /declare AHSpellMinMana[${AHCount}] int outer 0
      /declare AHClass[${AHCount}] string outer
      /declare AHTarCnt[${AHCount}] int outer 0
      /declare AHAnnounce[${AHCount}] string outer
      /declare AHCondition[${AHCount}] string outer
      /declare AHThreshold[${AHCount},6] int outer 0
      /declare AHBardSlot[${AHCount}] int outer
      /declare AHMinLevel[${AHCount}] int outer      
      /declare tSpellID int local

      /for a 1 to ${AHCount}
         /call LoadAHIniEntry ${a}
         /if (${Select[${AHGem[${a}]},${ValidSpellGems}]}) {
            /if (${Me.Book[${Me.Book[${AHSpell[${a}]}]}].ID}) {
               /varset AHSpell[${a}] ${Me.Book[${Me.Book[${AHSpell[${a}]}]}].Name}
            } else /if (${Me.Book[${Me.Book[${AHSpell[${a}]} ]}].ID}) {
               /varset AHSpell[${a}] ${Me.Book[${Me.Book[${AHSpell[${a}]} ]}].Name}
            } else /if (${Me.Book[${Spell[${AHSpell[${a}]}].RankName}]}) {
               /varset AHSpell[${a}] ${Spell[${AHSpell[${a}]}].RankName}
            } else {
               /if (${AHTarCnt[${a}]} || ${InfoMsgLevel}) {
                  /bc Msg: Spell ${AHSpell[${a}]} Not found in your book.  Check your .ini settings.
               }
               /varset AHTarCnt[${a}] 0
            } 
            /if (${Spell[${AHSpell[${a}]}].Subcategory.Equal[Delayed]} && ${Spell[${AHSpell[${a}]}].HasSPA[44]}) {
               /varset PRSpell ${a}
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}PR-type spell identified - ${AHSpell[${a}]}
            }
            /if (!${IgnoreBuffLevelRestrictions}) {
               /call GetMinLevel ${Spell[${AHSpell[${a}]}].ID}
               /if (${Macro.Return.NotEqual[false]}) {
                  /varset AHMinLevel[${a}] ${Macro.Return}
               }
            }
            /varset AHHealList ${AHHealList}${AHSpell[${a}]}${PipeChar}
         } else /if (${AHGem[${a}].Find[item]}) {
            /if (!${FindItem[=${AHSpell[${a}]}].ID}) {
               /if (${AHTarCnt[${a}]} || ${InfoMsgLevel}) {
                  /bc Msg: Heal Item not found: ${AHSpell[${a}]}. Check your .ini settings (or get more of the item).
               }
            }
            /if (${FindItem[${AHSpell[${a}]}].Spell.Name.NotEqual[NULL]}) {
               /varset AHHealList ${AHHealList}${FindItem[${AHSpell[${a}]}].Spell.Name}${PipeChar}
            }
         } else /if (${AHGem[${a}].Equal[alt]}) {
            /if (!${Me.AltAbility[${AHSpell[${a}]}].ID} && !${Me.Shrouded}) {
               /if (${AHTarCnt[${a}]} || ${InfoMsgLevel}) {
                  /bc Msg: You don't have the AltAbility ${AHSpell[${a}]}.  Check your .ini settings
               }
               /varset AHTarCnt[${a}] 0
            }
            /if (${Me.AltAbility[${AHSpell[${a}]}].Spell.Name.NotEqual[NULL]}) {
               /varset AHHealList ${AHHealList}${Me.AltAbility[${AHSpell[${a}]}].Spell.Name}${PipeChar}
            }
         } else /if (!${Select[${AHGem[${a}]},script,ability]}) {
            /if (${AHTarCnt[${a}]} || ${InfoMsgLevel}) {
               /bc Msg: Invalid Value for AHGem #${a}. Please check your .ini settings.
            }
            /varset AHTarCnt[${a}] 0
         }
| Replace All/Any with Class list (strip percentage, if present)
         /if (${AHClass[${a}].Find[all]}) {
            /varset AHClass[${a}] ${AHClass[${a}].Replace[${AHClass[${a}]${If[!${AHClass[${a}].Arg[1].Find[all]},.Right[-${Math.Calc[${AHClass[${a}].Find[all]}-1]}],]}.Arg[1]},${pcClasses}]}
         } else /if (${AHClass[${a}].Find[any]}) {
            /varset AHClass[${a}] ${AHClass[${a}].Replace[${AHClass[${a}]${If[!${AHClass[${a}].Arg[1].Find[any]},.Right[-${Math.Calc[${AHClass[${a}].Find[any]}-1]}],]}.Arg[1]},${pcClasses}]}
         }
| Build the Threshold Heal amounts
| 1 - PCs
         /if (${AHClass[${a}].Find[self]}) {
            /call CheckAHArgs "${AHClass[${a}]}"${PipeChar}self
            /if (${Macro.Return}>0) /varset AHThreshold[${a},1] ${Macro.Return}
         } else /if (${AHClass[${a}].Find[pc]}) {
            /for b 1 to ${pcClassCount}
               /if (${AHClass[${a}].Find[${pcClasses.Arg[${b}]}]}) {
                  /call CheckAHArgs "${AHClass[${a}]}"${PipeChar}${pcClasses.Arg[${b}]} 
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
            /call CheckAHArgs "${AHClass[${a}]}"${PipeChar}mypet
            /if (${Macro.Return}>0) /varset AHThreshold[${a},2] ${Macro.Return}
         } else /if (${AHClass[${a}].Find[pet]}) {
            /call CheckAHArgs "${AHClass[${a}]}"${PipeChar}pet
            /if (${Macro.Return}>0) /varset AHThreshold[${a},2] ${Macro.Return}
         }
| 3 - Tank
         /if (${AHClass[${a}].Find[tank]} && ${AHClass[${a}].Find[pc]}) {
            /call CheckAHArgs "${AHClass[${a}]}"${PipeChar}tank
            /if (${Macro.Return}>0) /varset AHThreshold[${a},3] ${Macro.Return}
         }
| 4 - Grp
         /if (${AHClass[${a}].Find[grp]}) {
            /call CheckAHArgs "${AHClass[${a}]}"${PipeChar}grp
            /if (${Macro.Return}>0) /varset AHThreshold[${a},4] ${Macro.Return}
         }
| 6 - XTgt
         /if (${AHXTgtCount} && ${AHClass[${a}].Find[xtgt]}) {
            /call CheckAHArgs "${AHClass[${a}]}"${PipeChar}xtgt
            /if (${Macro.Return}>0) /varset AHThreshold[${a},6] ${Macro.Return}
         }
| /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${a} - ${AHThreshold[${a},1]} - ${AHThreshold[${a},2]} - ${AHThreshold[${a},3]} - ${AHThreshold[${a},4]} - ${AHThreshold[${a},5]} - ${AHThreshold[${a},6]}
      /next a
   }
/return


Sub LoadAHIniEntry(int a)
   /if (${AHNewFormat}) {
      /call LoadINIVar AH${a} Gem " " ${If[${a},AHGem[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AH${a} Spell " " ${If[${a},AHSpell[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AH${a} SpellFoci " " ${If[${a},AHSpellFoci[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AH${a} DurMod 0 ${If[${a},AHDurMod[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AH${a} SpellMinMana 0 ${If[${a},AHSpellMinMana[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AH${a} SpellAlias " " ${If[${a},AHSpellAlias[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AH${a} Announce " " ${If[${a},AHAnnounce[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AH${a} TarCnt 0 ${If[${a},AHTarCnt[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AH${a} Class " " ${If[${a},AHClass[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AH${a} PreCondition "TRUE" ${If[${a},AHCondition[${a}],NOVAR]} ${IniFile}
   } else {
      /call LoadINIVar AdvHeal AHGem${a} " " ${If[${a},AHGem[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AdvHeal AHSpell${a} " " ${If[${a},AHSpell[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AdvHeal AHSpellFoci${a} " " ${If[${a},AHSpellFoci[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AdvHeal AHDurMod${a} 0 ${If[${a},AHDurMod[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AdvHeal AHSpellMinMana${a} 0 ${If[${a},AHSpellMinMana[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AdvHeal AHSpellAlias${a} " " ${If[${a},AHSpellAlias[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AdvHeal AHAnnounce${a} " " ${If[${a},AHAnnounce[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AdvHeal AHTarCnt${a} 0 ${If[${a},AHTarCnt[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AdvHeal AHClass${a} " " ${If[${a},AHClass[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AdvHeal AHPreCondition${a} "TRUE" ${If[${a},AHCondition[${a}],NOVAR]} ${IniFile}
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
   /declare tBuffs string local
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
      | KISS for cleric divarb to hopefully just work without needing setup as a seperate heal.
      |  Group.Injured should be enough but we also need to check range.
      |  Interrupt any other spell because sometimes this is a cleric's most important ability for emergency.
      /if (${Me.Class.ShortName.Equal[clr]} && ${UseDivArb} && ${Me.AltAbilityReady[Divine Arbitration]} && ${Group.Injured[${DivArbPct}]}) {
         /for x 0 to ${Math.Calc[${Group.Members} + 1]}
            /if (${Group.Member[${x}].PctHPs}<${DivArbPct} && ${Group.Member[${x}].Distance}<${Spell[Divine Arbitration].Location} && ${Me.AltAbilityReady[Divine Arbitration]}) {
               /if (${Me.Casting.ID} && ${Me.Casting.ID}!=${Spell[Divine Arbitration].ID}) {
                  /interrupt
                  /delay 1s !${Me.Casting.ID}
               }
               /casting "Divine Arbitration|alt"
               /delay 1s !${Me.AltAbilityReady[Divine Arbitration]} && !${Me.Casting.ID}
               /if (${Me.AltAbilityReady[Celestial Regeneration]}) /casting "Celestial Regeneration|alt"
               /break
            }
         /next x
      }
      /if (!${DoHeals}) /return
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
                  /call GetBardSlot "${AHSpell[${a}]}" AH${PipeChar}${a}
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
            /if (${Me.Class.ShortName.Equal[BRD]}) {
               /if (!${AHBardSlot[${a}]} && ${Spell[${tSpellID}].MyCastTime}) {
                  /call GetBardSlot "${AHSpell[${a}]}" AH${PipeChar}${a}
                  /if (!${AHBardSlot[${a}]}) {
                     /bc Msg: No BardSlot assigned. Turning off AA casting for: ${AHSpell[${a}]}
                     /varset AHTarCnt[${a}] 0
                     /continue
                  }
               }
            }
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
            /varset tMembers ${mString.Arg[1,${PipeChar}]}
            /varset tHurt ${mString.Arg[2,${PipeChar}]}
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
                  /varset tBuffs ${NetBots[${tgtName}].Buff}${NetBots[${tgtName}].ShortBuff}
                  /call BuffWillStack 9755 "${tBuffs}"
                  /if (${Macro.Return.Equal[false]}) {
|                    /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${AHSpell[${a}]} - Won't stack on ${tgtName}
                     /break
                  }
               } else {
                  /varset tBuffs ${NetBots[${Spawn[${AHbTargetID}].Master.CleanName}].PetBuff}
                  /call BuffWillStack 10342 "${tBuffs}"
                  /if (${Macro.Return.Equal[false]}) {
|                    /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${AHSpell[${a}]} - Won't stack on ${tgtName}
                     /break
                  }
               }
            }
            /if (${Corpse.Open}) {
               /nomodkey /keypress esc
            }
            /varset tgtName ${Spawn[${AHbTargetID}].Name}
| Don't heal the puller if out of camp and in Combat. If he's not on NetBots, the best we can do is to check FD.
            /if (${PullerName.Equal[${tgtName}]}) {
               /if ((${NetBots[${PullerName}].ID} && ${NetBots[${PullerName}].CombatState}==0) || (!${NetBots[${PullerName}].ID} && !${Spawn[${AHbTargetID}].Feigning})) {
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
                     /varset tBuffs ${NetBots[${tgtName}].Buff}${NetBots[${tgtName}].ShortBuff}
                     /call BuffWillStack 9755 "${tBuffs}"
                     /if (${Macro.Return.Equal[false]} || !${NetBots[${tgtName}].FreeBuffSlots}) /break
                  } else {
                     /varset tBuffs ${NetBots[${Spawn[${AHbTargetID}].Master.CleanName}].PetBuff}
                     /call BuffWillStack 10342 "${tBuffs}"
                     /if (${Macro.Return.Equal[false]}) /break
                  }
               }
               /if (${NetBots[${tgtName}].ShortBuff.Find[${tSpellID}]}) /break
               /if (${Spell[${tSpellID}].Duration}) {
                  /if (${tTarCnt}==1 && ${tGem}<20) {
                     /if (${Defined[${tgtName}${a}]}) {
                        /if (${${tgtName}${a}}>0) /break
                     }
                     /if (${AHMinLevel[${a}]}) {
                        /varset tgtLevel ${If[${Spawn[${AHbTargetID}].Type.Equal[pc]},${Spawn[${AHbTargetID}].Level},${Spawn[${AHbTargetID}].Master.Level}]}
                        /if (${tgtLevel}<${AHMinLevel[${a}]}) /return
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
               /if (${Target.ID}!=${AHbTargetID} && ${AHbTargetID}) {
                  /squelch /tar id ${If[${AHbTargetID}==99999,${Me.ID},${AHbTargetID}]}
               }
               /if (${tGem}==40) {
                  /varset PSTargetID ${Target.ID}
                  /call MBScript ${AHSpell[${a}]}
               } else {
                  /doability ${AHSpell[${a}]}
               }
               /return
            }
            /varset NeedtoHeal TRUE
            /if (${TankName.Arg[1,${PipeChar}].Equal[${Me.CleanName}]}) {
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
                  /if (${AHBardSlot[${a}]}) {
                     /stopsong
                     /squelch /twist once ${AHBardSlot[${a}]}
                  } else {
                     /alt act ${Me.AltAbility[${AHSpell[${a}]}].ID}
                  }
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
                  /if (!${Cast.Ready[${AHSpell[${a}]}${PipeChar}${AHGem[${a}]}]}) /break
               }
               /if (${Me.State.Equal[sit]}) /stand
               /if (${Stick.Status.Equal[on]} || ${MakeCamp.Status.Equal[on]}) {
                  /squelch /stick pause
                  /delay 1s !${Me.Moving}
               }
               /if (${AdvPath.Following} && !${AdvPath.Paused}) {
|/echo pausing in heal
                  /varset FollowState 5
                  /squelch /afollow pause
               }
               /varset tTargetID ${AHbTargetID}
               /varset MBCurrentSpell AH${PipeChar}${a}${PipeChar}${If[${tTargetID}==99999,0,${tTargetID}]}
               /if (${tTargetID}==99999) /varset tTargetID ${Me.ID}

               /casting "${AHSpell[${a}]}${PipeChar}${AHGem[${a}]}" ${If[!${tTargetID}, ,-targetID${PipeChar}${tTargetID}]} ${If[${AHSpellFoci[${a}].Length},-bandolier${PipeChar}${AHSpellFoci[${a}]},]} -maxtries${PipeChar}2
               /varset SpellWasCast TRUE

               /if (${AHHealMode.Arg[1,${PipeChar}]}) {
                  /if (${a}<=${AHHealMode.Arg[2,${PipeChar}]}) /varset HealOnlyTimer ${AHHealMode.Arg[3,${PipeChar}]}s
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
   /if (${Stick.Status.Equal[paused]}) /squelch /stick unpause
   /if (${FollowState}==5) {
|/echo unpause after heal
      /squelch /afollow unpause
      /varset FollowState 1
   }
   /if (${AHPrevTarget}) /target id ${AHPrevTarget}
/return


Sub GrpEval(string AHLine, int radius, int tarcnt, int a)
| Check self/mypet, grp, and tank HP. Then group members and xtargets. Then look for netbots clients out of group and pets (use Netbots only if out of group). Return string of ID's with count as last arg.
   /if (${AHLine.Find[self]}) {
      /if (${Me.PctHPs}<${AHThreshold[${a},1]}) /return "${Me.ID}${PipeChar}1"
      /return FALSE
   }
   /if (${AHLine.Find[mypet]}) {
      /if (${Me.Pet.ID} && ${Me.Pet.PctHPs}<${AHThreshold[${a},2]}) {
         /if (!${radius} || ${Me.Pet.Distance}<=${radius}) /return "${Me.Pet.ID}${PipeChar}1"
      }
      /return FALSE
   }
| Check Grp (Group.Injured)
   /if (${AHLine.Find[grp]}) {
      /if (${Group.Injured[${AHThreshold[${a},4]}]}>=${tarcnt}) /return "${Me.ID}${PipeChar}1"
      /return FALSE
   }
| Check Tank
   /if (${AHLine.Find[tank]} && ${TankName.Length}) {
      /if (!${AHLine.Find[group]} || ${Group.Member[${TankName.Arg[1,${PipeChar}]}].Index}) {
         /if ((${AHLine.Find[pc]} && ${Select[${Spawn[${TankID}].Type},pc,mercenary]}) || (${AHLine.Find[pet]} && ${Spawn[${TankID}].Type.Equal[pet]})) {
            /if (${Spawn[${TankID}].PctHPs}<${AHThreshold[${a},3]}) {
               /if (!${radius} || ${Spawn[${TankID}].Distance}<=${radius}) /return "${TankID}${PipeChar}1"
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
         /call CheckAHArgs "${AHLine}"${PipeChar}${Me.Class.ShortName}
         /if (!${Macro.Return} || ${Macro.Return}<${Me.PctHPs}) /goto :tCheckmypet
      }
      /varset Lowest ${Me.PctHPs}
      /varset matches ${Me.ID} ${matches}
      /varcalc totalcount ${totalcount}+1
   }
| Check my pet
:tCheckmypet
   /if (${Me.Pet.ID} && ${AHLine.Find[pet]} && ${Me.Pet.PctHPs}<${AHThreshold[${a},2]}) {
      /if (${Select[${Me.Class.ShortName},${petClasses},dru]} && !${Me.Pet.Name.Find[familiar]}) {
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
                     /call CheckAHArgs "${AHLine}"${PipeChar}${Group.Member[${i}].Class.ShortName}
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
               /if (${Select[${Group.Member[${i}].Class.ShortName},${petClasses},dru]} && !${Group.Member[${i}].Pet.Name.Find[familiar]}) {
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
                  /call CheckAHArgs "${AHLine}"${PipeChar}${NetBots[${NetBots.Client.Arg[${i}]}].Class.ShortName}
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
               /if (${Select[${NetBots[${NetBots.Client.Arg[${i}]}].Class.ShortName},${petClasses},dru]} && !${Spawn[${NetBots[${NetBots.Client.Arg[${i}]}].PetID}].Name.Find[familiar]}) {
                  /if (!${radius} || ${Spawn[${NetBots[${NetBots.Client.Arg[${i}]}].PetID}].Distance}<=${radius}) {
                     /varset matches ${matches} ${NetBots[${NetBots.Client.Arg[${i}]}].PetID}
                     /varcalc totalcount ${totalcount}+1
                  }
               }
            }
         } 
      /next i
   }
   /if (${matches.Length} && ${totalcount}) /return "${matches}${PipeChar}${totalcount}"
/return FALSE


Sub CheckAHArgs(string CheckString)
   /declare AHLine string local ${CheckString.Arg[1,${PipeChar}]}
   /declare ClassChk string local ${CheckString.Arg[2,${PipeChar}]}
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


Sub Event_AHDA
   /docommand /${BeepCommand}
   /bc Msg: DA on ME
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}DA on ME
   /doevents flush AHDA
/return
