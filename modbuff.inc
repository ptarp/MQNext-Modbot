
#Event BuffBlocked "#*#Your #1# did not take hold on #2#. (Blocked by #3#.)#*#"
#Event BuffBlocked "#*#Your #1# did not take hold. (Blocked by #3#.)#*#"

Sub LoadABINIVals
   /declare a int local
   /declare b int local
   /call LoadINIVar AdvBuff ABCount 0 ABCount "${IniFile}" int
   /call LoadINIVar AdvBuff ABNewFormat ${DefaultNewFormat} ABNewFormat "${IniFile}" int
   /call LoadINIVar AdvBuff ABMobMax 18 ABMobMax "${IniFile}" int
   /call LoadINIVar AdvBuff ABCheckTime 8 ABCheckTime "${IniFile}" int
   /declare ABCheckTimer timer outer
   /declare ABAliasList string outer
   /declare ABHasRecast bool outer FALSE
   /declare ABPetAB int outer 0
   /declare ABMountAB int outer 0
   /declare ABSelfShrinkAB int outer 0
   /declare ABSelfShrinkSize string outer
   /declare ABSelfShrinkTries int outer 0
   /declare ABPetShrinkAB int outer 0
   /declare ABPetShrinkSize string outer
   /declare ABPetShrinkTries int outer 0
   /declare PetShrinkTries int outer
   /declare SelfShrinkTries int outer
   /declare tABinterruptFlag bool outer
   /declare ABGemInUse string outer
   /declare SMPetCast bool outer FALSE
   /declare DoBuffsCBT bool outer FALSE
   /declare ABBuffBlocked bool outer
   /declare ABBlockMatch string outer
   /declare ABBlockCount int outer
   /declare ABBlockSpell[10] string outer
| If setting up new ini, create a dummy entry to show a template
   /if (${MBNewIni}) {
      /call LoadABIniEntry 0
   }
   /if (${ABCount}) {
      /declare ABMBL[${ABMobMax}] string outer
      /declare ABGem[${ABCount}] string outer
      /declare ABSpell[${ABCount}] string outer
      /declare ABSpellFoci[${ABCount}] string outer
      /declare ABDurMod[${ABCount}] string outer
      /declare ABSpellMinMana[${ABCount}] int outer
      /declare ABTarCnt[${ABCount}] int outer
      /declare ABTarType[${ABCount}] string outer
      /declare ABSpellAlias[${ABCount}] string outer
      /declare ABAnnounce[${ABCount}] string outer
      /declare ABScriptTime[${ABCount}] int outer
      /declare ABRecast[${ABCount}] bool outer
      /declare ABSpellIcon[${ABCount}] string outer
      /declare ABCondition[${ABCount}] string outer
      /declare ABSpellID[${ABCount}] int outer
      /declare ABBardSlot[${ABCount}] int outer
      /declare ABSpellIconX[${ABCount}] string outer
      /declare ABWontStack[${ABCount}] string outer
      /declare ABMinLevel[${ABCount}] int outer
      /declare tString1 string local
      /declare tString2 string local
      
      /for a 1 to ${ABCount}
         /call LoadABIniEntry ${a}
         /if (${ABSpellIcon[${a}].Length}>1) {
            /varset tString1 ${ABSpellIcon[${a}].Token[1,|]}
            /if (${tString1.Length}>1) {
               /varset tString2 ${ABSpellIcon[${a}].Token[2,|]}
            } else { 
               /varset tString2 ${ABSpellIcon[${a}].Arg[1,|]}
            }
            /if (${tString2.NotEqual[null]} && ${tString2.Length}>1) {
               /for b 1 to ${Math.Calc[${tString2.Count[,]}+1]}
                  /if (${Spell[${tString2.Arg[${b},,]}].ID}) {
                     /varset ABSpellIconX[${a}] ${Spell[${tString2.Arg[${b},,]}].ID} ${ABSpellIconX[${a}]}
                  } else {
                     /if (${ABTarCnt[${a}]} || ${InfoMsgLevel}) {
                        /echo ${If[${TimeStampOnEcho},[${Time}] ,]}ABSpellIcon${a} Additional (${tString2.Arg[${b},,]}) does not evaluate to a spell name.  Please check your spelling.
                     }
                  }
               /next b 
            }
            /if (${tString1.NotEqual[null]} && ${tString1.Length}>1) {
               /if (!${Spell[${tString1}].ID}) {
                  /if (${ABTarCnt[${a}]} || ${InfoMsgLevel}) {
                     /echo ${If[${TimeStampOnEcho},[${Time}] ,]}ABSpellIcon${a} (${tString1}) does not evaluate to a spell name.  Please check your spelling.
                  }
                  /varset ABSpellIcon[${a}]
               } else {
                  /varset ABSpellIcon[${a}] ${tString1}
               }
            } else {
               /varset ABSpellIcon[${a}]
            }
         } else {
            /varset ABSpellIcon[${a}]
         }
         /if (${ABTarType[${a}].Find[cbt]}) /varset DoBuffsCBT TRUE
         /varset ABAliasList ${ABAliasList} ${ABSpellAlias[${a}]}|
         /if (${Select[${ABGem[${a}]},${ValidSpellGems}]}) {
|Spell
            /if (${Me.Book[${Me.Book[${ABSpell[${a}]}]}].ID}) {
               /varset ABSpell[${a}] ${Me.Book[${Me.Book[${ABSpell[${a}]}]}].Name}
               /varset ABSpellID[${a}] ${Me.Book[${Me.Book[${ABSpell[${a}]}]}].ID}
            } else /if (${Me.Book[${Me.Book[${ABSpell[${a}]} ]}].ID}) {
               /varset ABSpell[${a}] ${Me.Book[${Me.Book[${ABSpell[${a}]} ]}].Name}
               /varset ABSpellID[${a}] ${Me.Book[${Me.Book[${ABSpell[${a}]}]}].ID}
            } else /if (${Me.Book[${Spell[${ABSpell[${a}]}].RankName}]}) {
               /varset ABSpell[${a}] ${Spell[${ABSpell[${a}]}].RankName}
               /varset ABSpellID[${a}] ${Me.Book[${Me.Book[${ABSpell[${a}]}]}].ID}
            } else {
               /if (${ABTarCnt[${a}]} || ${InfoMsgLevel}) {
                  /bc Msg: Spell ${ABSpell[${a}]} Not found in your book. Please check your .ini settings.
               }
               /varset ABTarCnt[${a}] 0
            }
            /if (!${IgnoreBuffLevelRestrictions}) {
               /call GetMinLevel ${ABSpellID[${a}]}
               /if (${Macro.Return.NotEqual[false]}) {
                  /varset ABMinLevel[${a}] ${Macro.Return}
               }
            }
            /call BuildNoStack ${a}
            /if (${Macro.Return.NotEqual[false]}) {
               /varset ABWontStack[${a}] ${Macro.Return}
            }
          } else /if (${ABGem[${a}].Equal[item]}) {
|Item
            /if (${FindItem[=${ABSpell[${a}]}].ID}) {
               /varset ABSpellID[${a}] ${FindItem[=${ABSpell[${a}]}].Spell.ID}
            } else {
               /if (${ABTarCnt[${a}]} || ${InfoMsgLevel}) {
                  /bc Msg: Buff Item not found: ${ABSpell[${a}]}. Check your .ini settings (or get more of the item).
               }
            }
         } else /if (${ABGem[${a}].Equal[alt]}) {
|Alt
            /if (!${Me.AltAbility[${ABSpell[${a}]}].ID} && !${Me.Shrouded}) {
               /if (${ABTarCnt[${a}]} || ${InfoMsgLevel}) {
                  /bc Msg: You don't have the AltAbility ${ABSpell[${a}]}. Please check your .ini settings.
               }
               /varset ABTarCnt[${a}] 0
            } else {
               /varset ABSpellID[${a}] ${Me.AltAbility[${ABSpell[${a}]}].Spell.ID}
            }
         } else /if (${ABGem[${a}].Equal[disc]}) {
|Disc
            /if (${Me.CombatAbility[${Spell[${ABSpell[${a}]}].RankName}]}) {
               /varset ABSpell[${a}] ${Spell[${ABSpell[${a}]}].RankName}
               /varset ABSpellID[${a}] ${Me.CombatAbility[${Me.CombatAbility[${ABSpell[${a}]}]}].ID}
            } else {
               /if (${ABTarCnt[${a}]} || ${InfoMsgLevel}) {
                  /bc Msg: You don't have the ${ABSpell[${a}]} Disc. Please check your .ini settings.
               }
               /varset ABTarCnt[${a}] 0
            }
         } else /if (${ABGem[${a}].NotEqual[script]}) {
|Script
            /if (${ABTarCnt[${a}]} || ${InfoMsgLevel}) {
               /bc Msg: Invalid Value for ABGem #${a}. Please check your .ini settings.
            }
            /varset ABTarCnt[${a}] 0
         }
         /if (${ABRecast[${a}]}) {
            /if (${Spell[${ABSpellID[${a}]}].TargetType.Find[Group]} || ${ABTarType[${a}].Find[self]}) {
               /varset ABRecast[${a}] FALSE
            }
         }
         /if (${ABRecast[${a}]}) /varset ABHasRecast TRUE
         /if (${ABTarType[${a}].Find[petspell]} || ${ABTarType[${a}].Find[petcast]}) {
            /varset ABPetAB ${a}
         } else /if (${ABTarType[${a}].Find[mountspell]} || ${ABTarType[${a}].Find[mountcast]}) {
            /varset ABMountAB ${a}
         } else /if (${ABTarType[${a}].Find[selfshrink]}) {
            /varset ABSelfShrinkAB ${a}
            /varset ABSelfShrinkSize ${If[${ABTarType[${a}].Arg[2,|].Length},${ABTarType[${a}].Arg[2,|]},1.5]}
            /varset ABSelfShrinkTries ${If[${ABTarType[${a}].Arg[3,|].Length},${ABTarType[${a}].Arg[3,|]},1]}
            /if (${ABSelfShrinkTries}<=0 || ${ABSelfShrinkTries}>3) /varset ABSelfShrinkTries 1
         } else /if (${ABTarType[${a}].Find[petshrink]}) {
            /varset ABPetShrinkAB ${a}
            /varset ABPetShrinkSize ${If[${ABTarType[${a}].Arg[2,|].Length},${ABTarType[${a}].Arg[2,|]},1.4]}
            /varset ABPetShrinkTries ${If[${ABTarType[${a}].Arg[3,|].Length},${ABTarType[${a}].Arg[3,|]},1]}
            /if (${ABPetShrinkTries}<=0 || ${ABPetShrinkTries}>3) /varset ABPetShrinkTries 1
         }
         /if (${ABTarType[${a}].Find[all]}) {
            /varset ABTarType[${a}] ${ABTarType[${a}].Replace[all,${pcClasses}]}
         } else /if (${ABTarType[${a}].Find[any]}) {
            /varset ABTarType[${a}] ${ABTarType[${a}].Replace[any,${pcClasses}]}
         }
      /next a
   }
/return


Sub LoadABIniEntry(int a)
   /if (${ABNewFormat}) {
      /call LoadINIVar AB${a} Gem " " ${If[${a},ABGem[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AB${a} Spell " " ${If[${a},ABSpell[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AB${a} SpellFoci " " ${If[${a},ABSpellFoci[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AB${a} DurMod 0 ${If[${a},ABDurMod[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AB${a} SpellAlias " " ${If[${a},ABSpellAlias[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AB${a} Announce " " ${If[${a},ABAnnounce[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AB${a} SpellMinMana 0 ${If[${a},ABSpellMinMana[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AB${a} TarCnt 0 ${If[${a},ABTarCnt[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AB${a} TarType " " ${If[${a},ABTarType[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AB${a} Recast FALSE ${If[${a},ABRecast[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AB${a} SpellIcon " " ${If[${a},ABSpellIcon[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AB${a} PreCondition "TRUE" ${If[${a},ABCondition[${a}],NOVAR]} ${IniFile}
   } else {    
      /call LoadINIVar AdvBuff ABGem${a} " " ${If[${a},ABGem[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AdvBuff ABSpell${a} " " ${If[${a},ABSpell[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AdvBuff ABSpellFoci${a} " " ${If[${a},ABSpellFoci[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AdvBuff ABDurMod${a} 0 ${If[${a},ABDurMod[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AdvBuff ABSpellAlias${a} " " ${If[${a},ABSpellAlias[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AdvBuff ABAnnounce${a} " " ${If[${a},ABAnnounce[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AdvBuff ABSpellMinMana${a} 0 ${If[${a},ABSpellMinMana[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AdvBuff ABTarCnt${a} 0 ${If[${a},ABTarCnt[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AdvBuff ABTarType${a} " " ${If[${a},ABTarType[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AdvBuff ABRecast${a} FALSE ${If[${a},ABRecast[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AdvBuff ABSpellIcon${a} " " ${If[${a},ABSpellIcon[${a}],NOVAR]} ${IniFile}
      /call LoadINIVar AdvBuff ABPreCondition${a} "TRUE" ${If[${a},ABCondition[${a}],NOVAR]} ${IniFile}
   }
/return


Sub ABBuffCheck
   /if (${MBPause} || ${ABCheckTimer} || ${APPullDelay} || !${DoBuffs}) /return
   /varset ABCheckTimer ${ABCheckTime}s

   /declare a int local
   /declare b int local
   /declare x int local
   /declare tSpellID int local
   /declare tGem int local
   /declare tTargetID int local
   /declare tName string local
   /declare zPetID int local
   /declare tSpellRange int local
   /for a 1 to ${ABCount}
      /call ValidateBuffCast
      /if (${Macro.Return.NotEqual[TRUE]}) /return
      /if (!${ABTarCnt[${a}]}) /continue
      /if (${ABSpell[${a}].Left[5].Equal[Yaulp]} && ${Me.Mount.ID}) /continue
      /varset tGem 0
      /varset tSpellID 0

      /if (${Select[${ABGem[${a}]},${ValidSpellGems}]}) {
|Spell
         /varset tSpellID ${ABSpellID[${a}]}
         /varset tGem ${ABGem[${a}]}
         /if (${Select[${tSpellID},${SpellsRestrictedByZone}]}) /continue
         /if (${Me.Class.ShortName.Equal[BRD]}) {
            /if (!${Me.Gem[${tGem}].Name.Length} || ${Me.Gem[${tGem}].Name.NotEqual[${ABSpell[${a}]}]}) {
               /call MemGem ${tGem} "${ABSpell[${a}]}"
               /if (${Macro.Return.NotEqual[TRUE]}) {
                  /echo Msg: Failed Memorize for: ${ABSpell[${a}]}
                  /continue
               }
            }
            /if (!${ABTarType[${a}].Find[aura]}) {
               /if (${Spell[${tSpellID}].Duration}) {
                  /if (${ABTarType[${a}].Find[cbt]} || ${Spell[${tSpellID}].SpellType.Equal[detrimental]}) {
                     /call EditTwist "${BardCombatGems}" ${tGem} add
                     /varset BardCombatGems ${Macro.Return}
                     /if (${ABTarType[${a}].Find[idle]}) {
                        /call EditTwist "${BardRestGems}" ${tGem} add
                        /varset BardRestGems ${Macro.Return}
                     }
                  } else {
                     /call EditTwist "${BardRestGems}" ${tGem} add
                     /varset BardRestGems ${Macro.Return}
                  }
                  /continue
               }  
               /if (${Me.GemTimer[${tGem}]}) /continue
               /if (${ABTarCnt[${a}]}>1 && ${Me.GroupSize}<${ABTarCnt[${a}]}) /continue
            }
         }
         /if (${Me.CurrentMana}<${Spell[${tSpellID}].Mana} || ${Me.PctMana}<${ABSpellMinMana[${a}]}) /continue
      } else /if (${ABGem[${a}].Find[item]}) {
|Item
         /if (!${FindItem[=${ABSpell[${a}]}].ID} || ${FindItem[=${ABSpell[${a}]}].Timer}) /continue
|In case we didn't have it on startup
         /varset ABSpellID[${a}] ${FindItem[=${ABSpell[${a}]}].Spell.ID}
         /varset tSpellID ${ABSpellID[${a}]}
         /varset tGem 20
         /if (${Me.Class.ShortName.Equal[BRD]}) {
            /if (!${ABBardSlot[${a}]}) {
               /call GetBardSlot "${ABSpell[${a}]}" AB|${a}
               /if (!${ABBardSlot[${a}]}) {
                  /bc Msg: No BardSlot assigned. Turning off Item casting for: ${ABSpell[${a}]}
                  /varset ABTarCnt[${a}] 0
                  /continue
               }
            }
         }
      } else /if (${ABGem[${a}].Equal[alt]}) {
|Alt
         /if (!${Me.AltAbility[${ABSpell[${a}]}].ID}) /continue
         /if (!${Me.AltAbilityReady[${ABSpell[${a}]}]} || ${Me.CurrentMana}<${Me.AltAbility[${ABSpell[${a}]}].Spell.Mana}) /continue
         /varset tSpellID ${ABSpellID[${a}]}
         /varset tGem 30
         /if (${Me.Class.ShortName.Equal[BRD]}) {
            /if (!${ABBardSlot[${a}]} && ${Spell[${tSpellID}].MyCastTime}) {
               /call GetBardSlot "${ABSpell[${a}]}" AB|${a}
               /if (!${ABBardSlot[${a}]}) {
                  /bc Msg: No BardSlot assigned. Turning off AA casting for: ${ABSpell[${a}]}
                  /varset ABTarCnt[${a}] 0
                  /continue
               }
            }
         }
      } else /if (${ABGem[${a}].Equal[script]}) {
|Script
         /if (${ABScriptTime[${a}]}<=${Macro.RunTime}) {
            /call MBScript ${ABSpell[${a}]}
            /varset ABScriptTime[${a}] ${Math.Calc[${ABDurMod[${a}]}+${Macro.RunTime}]}
         }
         /continue
      } else /if (${ABGem[${a}].Equal[disc]}) {
|Disc
         /if (${Me.PctEndurance}<${ABSpellMinMana[${a}]} || ${ABScriptTime[${a}]}>${Macro.RunTime} || !${Me.CombatAbilityReady[${ABSpell[${a}]}]}) /continue
         /varset tSpellID ${ABSpellID[${a}]}
         /varset tGem 40
      }

      /if (!${tGem}) /continue
      /if (${ABSpellIcon[${a}].Length}>1) {
         /if (!${Spell[${ABSpellIcon[${a}]}].ID}) /continue
         /varset tSpellID ${Spell[${ABSpellIcon[${a}]}].ID}
      }
      /if (${Select[${tSpellID},${SpellsRestrictedByZone}]}) /continue
      /if (${Spell[${tSpellID}].Location}==1 && !${Zone.Outdoor}) /continue
      /if (${ABTarCnt[${a}]}>1) {
         /varset tSpellRange ${If[${Spell[${tSpellID}].AERange},${Spell[${tSpellID}].AERange},${Spell[${tSpellID}].Range}]}
         /if (${ABTarType[${a}].Find[grp]}) {
            /if (${Me.GroupSize}<${ABTarCnt[${a}]}) /continue
            /if (${SpawnCount[pc group radius ${tSpellRange}]}<${ABTarCnt[${a}]}) /continue
         } else {
            /if (${SpawnCount[pc radius ${tSpellRange}]}<${ABTarCnt[${a}]}) /continue
         }
      }
      /varset tABinterruptFlag FALSE
      /if (${ABPetAB}==${a}) {
         /if (${Me.Pet.ID} || !${DoPet}) /continue
         /if (${ABTarType[${a}].Find[ sm]}) {
            /if (${Me.AltAbilityReady[Companion's Suspension]} && !${SMPetCast}) {
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Casting pet.
               /varset SMPetCast TRUE
               /call MQ2Cast "Companion's Suspension" alt 7s CastCheck
               /if (${interruptFlag}) {
                  /varset tABinterruptFlag TRUE
                  /return
               }
               /delay 1s ${Me.Pet.ID}
               /if (${Me.Pet.ID}) /continue
            }
         }
         /varset tTargetID ${Me.ID}
         /call BuffThem ${a} ${tSpellID} ${tGem} ${tTargetID} 2
         /if (${tABinterruptFlag}) /return
      } else /if (${ABMountAB}==${a}) {
         /if (${Me.Mount.ID} || !${DoMount}) /continue
         /varset tTargetID ${Me.ID}
         /call BuffThem ${a} ${tSpellID} ${tGem} ${tTargetID} 3
      } else /if (${ABSelfShrinkAB}==${a}) {
         /if (!${DoSelfShrink}) /continue
         /if (${Me.Height}<=${ABSelfShrinkSize} || ${SelfShrinkTries}>=${ABSelfShrinkTries}) /continue
         /varset tTargetID ${Me.ID}
         /call BuffThem ${a} ${tSpellID} ${tGem} ${tTargetID} 4
      } else /if (${ABPetShrinkAB}==${a}) {
         /if (!${Me.Pet.ID} || !${DoPetShrink}) /continue
         /if (${Me.Pet.Height}<=${ABPetShrinkSize} || ${PetShrinkTries}>=${ABPetShrinkTries}) /continue
         /varset tTargetID ${Me.Pet.ID}
         /call BuffThem ${a} ${tSpellID} ${tGem} ${tTargetID} 5
      } else /if (${ABTarType[${a}].Find[self]}) {
         /varset tTargetID ${Me.ID}
         /call BuffThem ${a} ${tSpellID} ${tGem} ${tTargetID} 1
      } else /if (${ABTarType[${a}].Find[mypet]}) {
         /if (${Me.Pet.ID} && ${Select[${Me.Class.ShortName},${petClasses}]}) {
            /varset tTargetID ${Me.Pet.ID}
            /call BuffThem ${a} ${tSpellID} ${tGem} ${tTargetID} 1
         }
      } else /if (${ABTarType[${a}].Find[tank]} && ${TankName.Length}) {
         /varset tTargetID ${Spawn[${TankID}].ID}
         /if (${tTargetID}) {
            /call BuffThem ${a} ${tSpellID} ${tGem} ${tTargetID} ${If[${NetBots[${TankName.Arg[1,|]}].ID},1,0]}
         }
      } else {
| The seemingly-redundant use of ${Spawn} in these sections is to make sure they are in the zone with me.
         /for b 1 to ${NetBots.Counts}
            /if (${NetBots[${NetBots.Client.Arg[${b}]}].ID} && ${Spawn[${NetBots[${NetBots.Client.Arg[${b}]}].ID}].ID}) {
               /varset tName ${NetBots.Client.Arg[${b}]}
               /if (${ABTarType[${a}].Find[grp]}) {
                  /if (!${Group.Member[${tName}].Index} && !${tName.Equal[${Me.CleanName}]}) /continue
               } else {
                  /if (${ABTarType[${a}].Find[oog]}) {
                     /if (${Group.Member[${tName}].Index} || ${tName.Equal[${Me.CleanName}]}) /continue
                  }
                  /if (${ABTarType[${a}].Find[nogroup]}) {
                     /if (${NetBots[${tName}].Grouped}) /continue
                  } else /if (${ABTarType[${a}].Find[isgrouped]}) {
                     /if (!${NetBots[${tName}].Grouped}) /continue
                  }
               }
               /if (${ABTarType[${a}].Find[${NetBots[${tName}].Class.ShortName}]}) {
                  /call BuffThem ${a} ${tSpellID} ${tGem} ${NetBots[${tName}].ID} 1
                  /if (${tABinterruptFlag}) /break
               }
               /if (${NetBots[${tName}].PetID} && ${ABTarType[${a}].Find[pet]}) {
                  /if (${Select[${NetBots[${tName}].Class.ShortName},${petClasses}]}) {
                     /if (!${Spell[${tSpellID}].TargetType.Find[group]} || ${NetBots[${tName}].Note.Find[MBPetAffinity]}) {
                        /call BuffThem ${a} ${tSpellID} ${tGem} ${NetBots[${tName}].PetID} 1
                        /if (${tABinterruptFlag}) /break
                     }
                  }
               }
            }
         /next b
         /if (${tABinterruptFlag}) /break
         /if (${Group.Members} && !${ABTarType[${a}].Find[oog]} && !${ABTarType[${a}].Find[nogroup]}) {
            /for b 1 to ${Group.Members}
               /if (!${NetBots[${Group.Member[${b}].Name}].ID} && ${Spawn[${Group.Member[${b}].Name}].ID}) {
                  /if (${ABTarType[${a}].Find[${Group.Member[${b}].Class.ShortName}]}) {
                     /call BuffThem ${a} ${tSpellID} ${tGem} ${Group.Member[${b}].ID} 0
                     /if (${tABinterruptFlag}) /break
                  }
                  /if (${Group.Member[${b}].Pet.ID} && ${ABTarType[${a}].Find[pet]}) {
                     /if (${Select[${Group.Member[${b}].Class.ShortName},${petClasses}]}) {
                        /if (!${Spell[${tSpellID}].TargetType.Find[group]}) {
                           /call BuffThem ${a} ${tSpellID} ${tGem} ${Group.Member[${b}].Pet.ID} 0
                           /if (${tABinterruptFlag}) /break
                        }
                     }
                  }
               }
            /next b
         }            
      }
      /if (${tABinterruptFlag}) /break
   /next a
/return


Sub BuffThem(int a, int tSpellID, int tGem, int tTargetID, int tBot) 
   /if (!${DoBuffs}) /return
   /declare b int local
   /declare c int local
   /declare tSpellRange int local
   /declare tSpellDuration int local
   /declare tSpellDelay int local
   /declare sSpellID string local
   /declare sVar string local
   /declare sVarT string local
   /declare tTime int local
   /declare tName string local
   /declare tABMob int local 0
   /declare ABtTargetID int local
   /declare tBuffs string local
   /declare tCastTime timer local
   /declare tRecastTime timer local
   /declare tgtLevel int local
   /declare tHeight float local
   
   /call ValidateBuffCast
   /if (${Macro.Return.NotEqual[TRUE]}) {
      /varset tABinterruptFlag TRUE
      /return
   }   
   /if (!${ABTarType[${a}].Find[cbt]}) {
      /if (${ADMobFound}) /return
   } else {
      /if (!${ABTarType[${a}].Find[idle]} && !${ADMobFound}) /return
   }
   /varset tName ${Spawn[${tTargetID}].CleanName}
   /if (!${tTargetID} || !${tName.Length}) /return
   /if (${ABCondition[${a}].Length}>4) {
      /varset PSTargetID ${tTargetID}
      /call IniCommand "${ABCondition[${a}]}"
      /if (${Macro.Return.NotEqual[TRUE]}) /return
   }
   /if (${ABTarType[${a}].Find[aura]}) {
      /if (${ABTarType[${a}].Find[self]}) { 
         /if (${ABGem[${a}].Equal[disc]}) {
            /if (!${Me.Aura.SpawnID}) {
               /disc ${Spell[${ABSpell[${a}]}].Name}
               /delay 1s ${Me.Casting.ID}
               /delay ${Math.Calc[${Spell[${ABSpell[${a}]}].MyCastTime}/100+5].Int} !${Me.Casting.ID}
            }
            /return
         }
         /for b 1 to 2
            /if (${Me.Aura[${b}].SpawnID}) {
               /if (${ABSpell[${a}].Find[${Me.Aura[${b}].Name}]}) /return
| Check the Aura Spawn's Name. Some of the Auras don't match the spell names nor do they match the Effect Names. This may catch some.
| For instance: Spell Name: Begulier's Aura, Aura Window Name: Beguliers Aura, Song Window Name: Beguiler's Aura Effect, Aura Spawn's CleanName: Beguiler's Aura
               /if (${ABSpell[${a}].Find[${Spawn[${Me.Aura[${b}].SpawnID}].CleanName}]}) /return
            }
         /next b
         /if (${Me.Aura.SpawnID}) {
            /if (!${Me.AltAbility[Auroria Mastery]} && !${Me.AltAbility[Spirit Mastery]}) /return
            /if (${Me.Aura[1].SpawnID} && ${Me.Aura[2].SpawnID}) /return
         }
      } else /if (${Me.Class.ShortName.Equal[mag]} && ${ABTarType[${a}].Find[mypet]}) { 
         /if (${Me.PetBuff[${ABSpell[${a}]} Effect]} || !${Spell[${ABSpell[${a}]} Effect].StacksPet}) /return
      } else {
         /return
      }
   } else /if (${ABGem[${a}].Equal[disc]}) {
|Disc (not aura)
      /if (${Me.PctEndurance}<${ABSpellMinMana[${a}]} || ${ABScriptTime[${a}]}>${Macro.RunTime} || !${Me.CombatAbilityReady[${ABSpell[${a}]}]}) /return
   }
   /if (!${tGem} || !${tSpellID}) /return
   /if (!${Select[${tBot},2,4,5]}) {
      /if (${Select[${Spell[${tSpellID}].TargetType},self,single]} && (${tTargetID}==${Me.ID} || ${ABTarType[${a}].Find[self]})) {
         /if (!${Me.Buff[${Spell[${tSpellID}].Name}].ID}) {
            /if (!${Spell[${tSpellID}].DurationWindow} && !${Me.FreeBuffSlots}) /goto :tBTReturn
            /if (!${Spell[${tSpellID}].Stacks}) /goto :tBTReturn
         } else {
            /if (${Me.Buff[${Spell[${tSpellID}].Name}].Duration.Ticks}>6) /goto :tBTReturn
         }
      }
      /for c 1 to ${ABMobMax}
         /if (${tName.Equal[${ABMBL[${c}].Arg[1,|]}]}) {
            /varset tABMob ${c}
            /break
         }
         /if (!${ABMBL[${c}].Length}) {
            /varset ABMBL[${c}] ${tName}|
            /varset tABMob ${c}
            /break
         }
      /next c
      /if (!${tABMob}) {
         /for c 1 to ${ABMobMax}
            /if (${ABMBL[${c}].Length} && !${Spawn[${ABMBL[${c}].Arg[1,|]}].ID}) {
               /varset ABMBL[${c}] ${tName}|
               /varset tABMob ${c}
               /break
            } 
         /next c
         /if (!${tABMob}) {
            /bc Msg: CAN NOT TRACK ANY MORE - Consider increasing your ABMobMax setting
            /return
         }
      }
      /varset sSpellID ${tSpellID}
      /varset sVar ${ABMBL[${tABMob}].Mid[${ABMBL[${tABMob}].Find[b${sSpellID}]}|,14]}
      /varset sVarT ${sVar.Arg[2,|]}
      /varset tTime ${sVarT.Mid[2,${sVarT.Length}]}
      /if (${tBot}) {
         /varset tBuffs ${If[${Spawn[${tTargetID}].Type.Equal[pc]},${NetBots[${tName}].Buff}${NetBots[${tName}].ShortBuff},${NetBots[${Spawn[${tTargetID}].Master.CleanName}].PetBuff}]}
         /if (${tBuffs.Find[${tSpellID}]}) {
            /if (!${tTime} || ${Spawn[${tTargetID}].Type.Equal[pet]}) /goto :tBTReturn
         } else {
            /if (${ABSpellIconX[${a}].Length}>1) {
               /for c 1 to ${Math.Calc[${ABSpellIconX[${a}].Count[ ]}+1]}
                  /if (${tBuffs.Find[${ABSpellIconX[${a}].Arg[${c}]}]}) /goto :tBTReturn
               /next c 
            }
            /if (${ABWontStack[${a}].Length}) {
               /for c 1 to ${Math.Calc[${ABWontStack[${a}].Count[,]}+1]}
                  /if (${tBuffs.Find[${ABWontStack[${a}].Arg[${c},,]} ]}) /goto :tBTReturn
               /next c
            }
            /if (${Spawn[${tTargetID}].Type.Equal[pc]}) {
               /if (!${NetBots[${tName}].FreeBuffSlots}) /goto :tBTReturn
            }
            /if (${tBuffs.Length} && ${ABTarType[${a}].Find[${Spawn[${tTargetID}].Class.ShortName}]} || (${ABTarType[${a}].Find[self]} && ${tTargetID}==${Me.ID}) || (${ABTarType[${a}].Find[pet]} && ${Spawn[${tTargetID}].Master.ID}) || ${ABTarType[${a}].Find[tank]}) {
               /call BuffWillStack ${tSpellID} "${tBuffs}"
               /if (${Macro.Return.Equal[false]}) {
                  /goto :tBTReturn
               }
               /varset tTime 0
            }
         }
      }
      /varset tSpellRange ${If[${Spell[${tSpellID}].AERange},${Spell[${tSpellID}].AERange},${Spell[${tSpellID}].Range}]}
      /if (${tName.Equal[${Me.CleanName}]} && ${ABTarType[${a}].Find[self]}) {
         /if (${Spell[${tSpellID}].Stacks} && ${Me.FreeBuffSlots} && !${Me.Buff[${Spell[${tSpellID}]}].ID}) /varset tTime 0
      }
      /if (${tTime}>${Macro.RunTime}) /goto :tBTReturn
      /if (${Spawn[${tTargetID}].Distance3D}>${tSpellRange}) /goto :tBTReturn
      /if (${Spawn[${tTargetID}].Type.Equal[pet]} && !${Spawn[${tTargetID}].IsSummoned}) { 
         /if (${Spell[${ABSpell[${a}]}].TargetType.Find[group]} || ${Spell[${ABSpell[${a}]}].TargetType.Find[pet2]}) /goto :tBTReturn
      }
   } else /if (${tBot}==4) {
      /varset tHeight ${Me.Height}
   } else /if (${tBot}==5) {
      /varset tHeight ${Me.Pet.Height}
   }
   /if (${tGem}<20) {
      /if (${Me.CurrentMana}<${Spell[${tSpellID}].Mana} || ${Me.PctMana}<${ABSpellMinMana[${a}]}) /return
      /if (${ABMinLevel[${a}]}) {
         /varset tgtLevel ${If[${Spawn[${tTargetID}].Type.Equal[pc]},${Spawn[${tTargetID}].Level},${Spawn[${tTargetID}].Master.Level}]}
         /if (${tgtLevel}<${ABMinLevel[${a}]}) /return
      }
      /if (!${Me.Gem[${tGem}].Name.Length} || ${Me.Gem[${tGem}].Name.NotEqual[${ABSpell[${a}]}]}) {
         /if (${ABGemInUse.Length}>4) {
            /if (${ABGemInUse.Arg[1,|].Equal[${ABGem[${a}]}]} && ${ABGemInUse.Arg[3,|]}>${Macro.RunTime}) /return
         }
         /call MemGem ${tGem} "${ABSpell[${a}]}"
         /if (${Macro.Return.NotEqual[TRUE]}) {
            /varset tABinterruptFlag TRUE
            /return
         }
      }
      /if (!${ABGemInUse.Length} || ${ABGemInUse.Arg[2,|].NotEqual[${ABSpell[${a}]}]}) {
         /varset ABGemInUse ${ABGem[${a}]}|${ABSpell[${a}]}|${Math.Calc[${Macro.RunTime}+${Spell[${ABSpell[${a}]}].RecastTime.Float}+10]}
      }
   } else /if (${tGem}<40) {
      /if (${Me.PctMana}<${ABSpellMinMana[${a}]}) /return
   } else /if (${tGem}==40) {
      /if (${Me.PctEndurance}<${ABSpellMinMana[${a}]} || ${ABScriptTime[${a}]}>${Macro.RunTime} || !${Me.CombatAbilityReady[${ABSpell[${a}]}]}) /return
      /if (${Spawn[${tTargetID}].Type.Equal[pet]} && ${Spell[${ABSpell[${a}]}].TargetType.Find[group]}) {
         /varset ABtTargetID ${Spawn[${tTargetID}].Master.ID}
      } else {
         /varset ABtTargetID ${tTargetID}
      }
      /if (${Target.ID}!=${ABtTargetID}) /squelch /tar id ${ABtTargetID}
      /delay 1s ${Target.ID}==${ABtTargetID}
      /if (${Target.ID}!=${ABtTargetID}) /goto :tBTReturn
      /disc ${Spell[${ABSpell[${a}]}].Name}
      /delay 1s ${Me.Casting.ID}
      /delay ${Math.Calc[${Spell[${ABSpell[${a}]}].MyCastTime}/100+5].Int} !${Me.Casting.ID}
      /varset ABScriptTime[${a}] ${Math.Calc[${ABDurMod[${a}]}+${Macro.RunTime}]}
      /call AddtoBuffArray ${tTargetID} ${tSpellID} ${tSpellDelay} ${a}
      /return
   }
   /if (${Spell[${tSpellID}].TargetType.Equal[self]}) {
      /varset tTargetID 0
   }
   /if (${Window[SpellBookWnd].Open}) /book close
   /varset ABtTargetID 0
   /varset tSpellDelay 0
   /varset CastResult UNKNOWN
   /varset tRecastTime 3s
   /call ResetBuffBlocks
   /call ResetOutDoors

   /while (1) {
      /if (${NetBots[${Me.CleanName}].Silenced} || ${NetBots[${Me.CleanName}].Charmed} || ${Select[${Me.State},STUN,HOVER,DEAD]}) {
         /varset tABinterruptFlag TRUE
         /return
      }
      /if (${DoHeals} && ${AHCount}) /call AHHealCheck
      /call ValidateBuffCast
      /if (${Macro.Return.NotEqual[TRUE]}) {
         /varset tABinterruptFlag TRUE
         /return
      }   
      /if (${ABtTargetID}) {
         /if (${Cast.Status.Equal[i]}) {
            /doevents OUTDOORS
            /if (${Cast.Result.Equal[CAST_OUTDOORS]} || ${OutDoorsFail}) {
               /call AddToRestrictedList ${tSpellID}
               /call BuildRestrictedList
               /return
            }
            /if (${tBot}==1) {
               /if (${tGem}<20) {
| Check for Blocked Buffs.
                  /call CheckBuffBlocks ${tSpellID} ${ABtTargetID}
                  /if (${Macro.Return.Equal[true]}) {
                     /call UpdateStackInfo ${a} ${tSpellID} "${tBuffs}"
                     /return
                  }
               }
            } else /if (${tBot}==2) {
               /if (${Me.Pet.ID}) /break
            } else /if (${tBot}==3) {
               /if (${Me.Mount.ID})) /break
            }
            /if (${Cast.Result.Equal[cast_success]} || ${Me.Class.ShortName.Equal[BRD]}) {
               /if (${tBot}==4) {
| If it doesn't look like Shrink is doing anything, don't do another.
                  /if (${Me.Height}>=${Math.Calc[${tHeight}*.9]}) {
                     /varset SelfShrinkTries ${ABSelfShrinkTries}
                  } else {
                     /varcalc SelfShrinkTries ${SelfShrinkTries}+1
                  }
               } else /if (${tBot}==5) {
| If it doesn't look like Shrink is doing anything, don't do another.
                  /if (${Me.Pet.Height}>=${Math.Calc[${tHeight}*.9]}) {
                     /varset PetShrinkTries ${ABPetShrinkTries}
                  } else {
                     /varcalc PetShrinkTries ${PetShrinkTries}+1
                  }
               }
               /break
            }
            /if (${Select[${Cast.Result},CAST_FIZZLE,CAST_RECOVER,CAST_OUTOFMANA]}) { 
               /if (${Me.CurrentMana}<${Spell[${tSpellID}].Mana} || ${Me.PctMana}<${ABSpellMinMana[${a}]}) /return
               /if (${tRecastTime}) {
                  /varset ABtTargetID 0
                  /continue
               }
            } else /if (${Select[${Cast.Result},CAST_IMMUNE,CAST_TAKEHOLD]}) { 
               /if (!${Spell[${tSpellID}].TargetType.Find[group]}) /varset tSpellDelay 900
               /break
            } else /if (${Cast.Result.Equal[CAST_COMPONENTS]}) {
               /bc Msg: Missing required components. Turning off buff [ ${ABSpell[${a}]} ]
               /docommand /${BeepCommand}
               /varset ABTarCnt[${a}] 0
               /varset tABinterruptFlag TRUE
               /return
            }
| Everything else - CAST_OUTOFRANGE,CAST_CANNOTSEE,CAST_DISTRACTED,CAST_NOTARGET,CAST_INTERRUPTED,CAST_CANCELLED,CAST_NOTREADY - Or whatever else we missed
            /goto :tBTReturn
         }
      } else /if (${Me.Class.ShortName.Equal[BRD]}) {
         /if (${tTargetID} && ${Target.ID}!=${tTargetID} && ${Spell[${tSpellID}].TargetType.NotEqual[self]}) {
            /if (${tTargetID}!=${Me.ID} && ${Spell[${tSpellID}].TargetType.Find[group]} && !${Group.Member[${tName}].Index} || ${Spell[${tSpellID}].TargetType.Equal[single]}) {
               /if (${Melee.Combat}) /attack off
               /squelch /tar id ${tTargetID}
               /delay 1s ${Target.ID}==${tTargetID}
            }
         }
         /varset ABtTargetID ${tTargetID}
         /if (${Me.State.Equal[sit]}) /stand
         /if (${tGem}==30) {
            /if (${ABBardSlot[${a}]}) {
               /stopsong
               /squelch /twist once ${ABBardSlot[${a}]}
            } else {
               /alt act ${tSpellID}
            }
            /delay ${Math.Calc[${Me.AltAbility[${ABSpell[${a}]}].Spell.MyCastTime}/100+3]}
         } else /if (${tGem}==20) {
             /stopsong
             /squelch /twist once ${ABBardSlot[${a}]}
             /delay ${Math.Calc[${FindItem[=${ABSpell[${a}]}].CastTime}/100+3]}
         } else {
            /stopsong
            /squelch /twist once ${tGem}
            /delay ${Math.Calc[${Spell[${ABSpell[${a}]}].MyCastTime}/100+3]}
         }
         /if (${tBot}==4) {
| If it doesn't look like Shrink is doing anything, don't do another.
            /if (${Me.Height}>=${Math.Calc[${tHeight}*.9]}) {
               /varset SelfShrinkTries ${ABSelfShrinkTries}
            } else {
               /varcalc SelfShrinkTries ${SelfShrinkTries}+1
            }
         } else /if (${tBot}==5) {
| If it doesn't look like Shrink is doing anything, don't do another.
            /if (${Me.Pet.Height}>=${Math.Calc[${tHeight}*.9]}) {
               /varset PetShrinkTries ${ABPetShrinkTries}
            } else {
               /varcalc PetShrinkTries ${PetShrinkTries}+1
            }
         }
         /break
      } else {
         /delay 1s ${Cast.Ready}
         /if (${Cast.Ready[${ABSpell[${a}]}|${ABGem[${a}]}]}) {
            /varset ABtTargetID ${tTargetID}
            /if (${Me.Shrouded}) /delay 1s
            /varset MBCurrentSpell AB|${a}|${ABtTargetID}
            /if (${tGem}<19) {
               /varcalc tCastTime ${Spell[${ABSpell[${a}]}].MyCastTime}/100+50
            } else /if (${tGem}==20) {
               /varcalc tCastTime ${FindItem[=${ABSpell[${a}]}].CastTime}/100+50
            } else /if (${tGem}==30) {
               /varcalc tCastTime ${Me.AltAbility[${ABSpell[${a}]}].Spell.MyCastTime}/100+50
            } else {
               /varcalc tCastTime ${Spell[${ABSpell[${a}]}].MyCastTime}/100+50
            }
            /if (${tBot}==2) {
               /if (${Me.Pet.ID}) /break
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Casting pet.
               /if (${ABTarType[${a}].Find[ sm]}) {
                  /if (${Me.AltAbilityReady[Companion's Suspension]} && !${SMPetCast}) {
                     /varset SMPetCast TRUE
                     /call MQ2Cast "Companion's Suspension" alt 10s CastCheck
                     /if (${interruptFlag}) {
                        /varset tABinterruptFlag TRUE
                        /return
                     }
                     /delay 1s ${Me.Pet.ID}
                     /if (${Me.Pet.ID}) /break
                  }
               }
            }
            /call MQ2Cast "${ABSpell[${a}]}" ${If[${Select[${ABGem[${a}]},${ValidSpellGems}]},gem,]}${ABGem[${a}]} ${tCastTime} CastCheck ${If[${ABtTargetID},${If[${Spawn[${ABtTargetID}].Type.Equal[pet]} && ${Spell[${ABSpell[${a}]}].TargetType.Find[group]} && !${Spell[${ABSpell[${a}]}].TargetType.Find[single]},-targetid|${Spawn[${ABtTargetID}].Master.ID},-targetid|${ABtTargetID}]}, ]} ${If[${ABSpellFoci[${a}].Length},-bandolier|${ABSpellFoci[${a}]},]}
            /if (!${ABtTargetID}) {
               /varset ABtTargetID ${Me.ID}
            }
         } else {
            /if (${Cast.Ready} &&  ${Spell[${tSpellID}].RecastTime.Float}>5) /return
         }
      }
      /if (${interruptFlag}) {
         /varset tABinterruptFlag TRUE
         /return
      }
   }

   /varset MiscCheckTimer 5s
   /if (${ABAnnounce[${a}].Length}) {
      /if (!${Select[${Cast.Result},cast_immune,cast_takehold]} || ${ABTarCnt[${a}]}>1 || ${Spell[${ABSpell[${a}]}].TargetType.Find[group]}) { 
         /call ParseAnnounce Buffed: "${ABAnnounce[${a}]}" "${Spawn[${ABtTargetID}].CleanName}" "${ABSpell[${a}]}" 
      }
   }
   /if (${Select[${tBot},0,1,3]}) {
      /call AddtoBuffArray ${tTargetID} ${tSpellID} ${tSpellDelay} ${a}
   } else /if (${tBot}==2) {
      /varset PetShrinkTries 0
   }
   /varset ABGemInUse
   /varset MBCurrentSpell
:tBTReturn
   /doevents
/return


Sub AddtoBuffArray(int tTargetID, string tSpellID, int tSpellDelay, int a)
|  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}AddtoString - Target: ${tTargetID} - Spell: ${Spell[${tSpellID}].Name} - Delay: ${tSpellDelay} - BuffNumber: ${a}
   /declare tABMob int local 0
   /declare c int local 0
   /declare tSpellDuration int local 0
   /declare sVarL string local
   /declare sVarR string local
   /if (!${tTargetID}) /varset tTargetID ${Me.ID}
   /for c 1 to ${ABMobMax}
      /if (${ABMBL[${c}].Arg[1,|].Equal[${Spawn[id ${tTargetID}].Name}]} || ${ABMBL[${c}].Arg[1,|].Equal[${Spawn[id ${tTargetID}].CleanName}]}) {
         /varset tABMob ${c}
         /break
      }
   /next c
   /if (!${tABMob}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Target not found in array. Buff not added.
      /return
   }
   /if (${Me.Shrouded}) {
      /varcalc tSpellDuration 900*1.${ABDurMod[${a}]}+1+${tSpellDelay}+${Macro.RunTime}-20
   } else /if (${ABDurMod[${a}]}) {
      /varcalc tSpellDuration ((${ABDurMod[${a}]}/100)*${Spell[${tSpellID}].Duration.TotalSeconds})+${Spell[${tSpellID}].Duration.TotalSeconds}+${tSpellDelay}+${Macro.RunTime}-20
   } else {
      /varcalc tSpellDuration ${Spell[${tSpellID}].MyDuration.TotalSeconds}+${tSpellDelay}+${Macro.RunTime}-20
   }
   /if (${ABMBL[${tABMob}].Find[|b${tSpellID}]}) {
      /varset sVarL ${ABMBL[${tABMob}].Left[${Math.Calc[${ABMBL[${tABMob}].Find[|b${tSpellID}]}+${tSpellID.Length}+1]}]}
      /varset sVarR ${ABMBL[${tABMob}].Right[-${Math.Calc[${ABMBL[${tABMob}].Find[|b${tSpellID}]}+${tSpellID.Length}+2]}]}
      /varset sVarR ${sVarR.Right[-${sVarR.Arg[1,|].Length}]}
|/echo ${If[${TimeStampOnEcho},[${Time}] ,]}Left: ${sVarL}  Right: ${sVarR}
      /varset ABMBL[${tABMob}] ${sVarL}|t${tSpellDuration}${sVarR}
|/echo ${If[${TimeStampOnEcho},[${Time}] ,]}${ABMBL[${tABMob}]}
   } else {
      /varset ABMBL[${tABMob}] ${ABMBL[${tABMob}]}b${tSpellID}|t${tSpellDuration}|
   }
| Testing a Delay for Netbots refresh of group spells buffs
   /delay 1
/return


Sub ValidateBuffCast
   /if (!${DoBuffs} || ${MBPause} || ${Me.State.Equal[feign]}) /return FALSE
   /call ADSpawnCheck Find
   /if (${ADMobFound} && ((${DoMelee} && !${ACState}) || !${DoBuffsCBT})) /return FALSE
   /if (${AdvPath.Following} && !${AdvPath.Paused}) {
      /if (${Me.Moving}) /return FALSE
      /if (${Spawn[${FollowID}].Distance}>${FollowDistanceCheck}) /return FALSE
   }
   /if (${Cursor.ID}) /call ClearCursor
/return TRUE


Sub ResetBuffBlocks
   /doevents flush BuffBlocked
   /varset ABBlockCount 0
   /varset ABBlockMatch
/return


Sub CheckBuffBlocks(int tSpellID,int tTargetID)
   /declare b int local
   /declare EventSpell string local
   /declare EventTarget string local   
   /varset ABBlockMatch
   /varset EventSpell ${Spell[${tSpellID}].Name}
   /varset EventTarget ${Spawn[${tTargetID}].Name},${Spawn[${tTargetID}].CleanName}
   /while (1) {
      /varset ABBuffBlocked FALSE
      /doevents BuffBlocked
      /if (!${ABBuffBlocked}) /break
   }
   /if (!${ABBlockCount}) /return FALSE
   /for b 1 to ${ABBlockCount}
      /if (${EventSpell.Find[${ABBlockSpell[${b}].Arg[1,|]}]}) {
         /if (${Select[${ABBlockSpell[${b}].Arg[2,|]},${EventTarget}]}) {
            /if (!${ABBlockMatch.Length}) {
               /varset ABBlockMatch ${ABBlockSpell[${b}].Arg[3,|]}
            } else /if (!${ABBlockMatch.Find[${ABBlockSpell[${b}].Arg[3,|]}]}) {
               /varset ABBlockMatch ${ABBlockMatch}|${ABBlockSpell[${b}].Arg[3,|]}
            }
         }
      }
   /next b
   /if (${ABBlockMatch.Length}) /return TRUE
/return FALSE   
   
   
Sub UpdateStackInfo(int a,int tSpellID,string tBuffs)
   /declare b int local
   /declare c int local
| Search his buffs for the Spell. We want to get the SpellID from here in case this Spell Name has different IDs (like Heroic Bond)
   /for c 1 to ${tBuffs.Count[ ]}
      /if (${Spell[${tBuffs.Arg[${c}]}].Name.Equal[${ABBlockMatch}]}) {
         /call AddNoStack ${tSpellID} "${Spell[${tBuffs.Arg[${c}]}].Name}|${Spell[${tBuffs.Arg[${c}]}].ID}"
         /if (!${Select[${Spell[${tBuffs.Arg[${c}]}].ID},${ABWontStack[${a}]}]}) {
            /varset ABWontStack[${a}] ${If[!${ABWontStack[${a}].Length},,${ABWontStack[${a}]},]}${Spell[${tBuffs.Arg[${c}]}].ID}
         }
| Add reciprocal NoStack
         /call AddNoStack ${Spell[${tBuffs.Arg[${c}]}].ID} "${Spell[${tSpellID}].Name}|${tSpellID}"
         /for b 1 to ${ABCount}
            /if (${Select[${ABGem[${b}]},${ValidSpellGems}]} && ${ABSpellID[${b}]}==${Spell[${tBuffs.Arg[${c}]}].ID}) {
               /if (!${Select[${tSpellID},${ABWontStack[${b}]}]}) {
                  /varset ABWontStack[${b}] ${If[!${ABWontStack[${b}].Length},,${ABWontStack[${b}]},]}${tSpellID}
               }
            }
         /next b
         /break
      }
   /next c
/return


Sub Event_BuffBlocked(string Line,string BuffName,string Toon,string BlockName)
   /if (!${Defined[Toon]}) {
      /declare Toon string local ${Me.CleanName}
   }
   /varset ABBuffBlocked TRUE
   /if (${BuffName.Right[6].Equal[ spell]}) /varset BuffName ${BuffName.Left[-6]}
   /if (${ABBlockCount}<10) { 
      /varcalc ABBlockCount ${ABBlockCount}+1
      /varset ABBlockSpell[${ABBlockCount}] ${BuffName}|${Toon}|${BlockName}
   }
/return


Sub BuildNoStack(int a)
   /declare b int local
   /declare tString1 string local
   /declare tString2 string local
   /declare tString3 string local
   /declare tWontStack string local
   /declare tSpellID int local
   /declare tSpellID2 int local
   /if (${ABSpellIcon[${a}].Length}>1) {
      /varset tSpellID ${Spell[${ABSpellIcon[${a}]}].ID}
   } else {    
      /varset tSpellID ${ABSpellID[${a}]}
   }
   /if (!${tSpellID}) /return FALSE
   /if (!${Spell[${tSpellID}].ID} || ${Spell[${tSpellID}].TargetType.Equal[self]}) /return FALSE
   /varset tString1 ${Ini["${MBCommon}",NoStack,"${tSpellID}","NOTFOUND"]}
   /if (${tString1.Length} && ${tString1.NotEqual[NOTFOUND]} && ${tString1.Find[,]}) {
      /varset tString3 ${tString1.Arg[1,,]}
      /for b 2 to ${Math.Calc[${tString1.Count[,]}+1]}
         /varset tString2 ${tString1.Arg[${b},,].Arg[2,|]}
         /if (${tString2.Length} && ${tString2.NotEqual[NULL]}) {
            /varset tSpellID2 ${tString2}
         } else {
            /varset tSpellID2 ${Spell[${tString1.Arg[${b},,].Arg[1,|]}].ID}
         }
         /if (${Spell[${tSpellID2}].ID}) {
            /varset tString3 ${tString3},${Spell[${tSpellID2}].Name}|${Spell[${tSpellID2}].ID}
            /varset tWontStack ${If[!${tWontStack.Length},,${tWontStack},]}${Spell[${tSpellID2}].ID}
         }
      /next b
      /if (${tString3.NotEqual[${tString1}]}) {
         /ini "${MBCommon}" "NoStack" "${tSpellID}" "${tString3}"
      }
   }
/return ${tWontStack}


Sub AddNoStack(int tSpellID,string tNoStack)
   /declare b int local
   /declare tString1 string local
   /declare tString3 string local
   /declare tBlockID int local ${tNoStack.Arg[2,|]}
   /if (!${tSpellID} || !${tNoStack.Length} || !${tBlockID}) /return FALSE
   /if (!${Spell[${tSpellID}].ID} || ${Spell[${tSpellID}].TargetType.Equal[self]}) /return FALSE
   /varset tString1 ${Ini["${MBCommon}",NoStack,"${tSpellID}","NOTFOUND"]}
   /if (!${tString1.Length} || ${tString1.Equal[NOTFOUND]}) {
      /varset tString3 ${Spell[${tSpellID}].Name},${tNoStack}
   } else {
      /if (${tString1.Find[${tBlockID}]}) /return FALSE
      /varset tString3 ${tString1},${tNoStack}
   }
   /if (${tString3.NotEqual[${tString1}]}) {
      /ini "${MBCommon}" "NoStack" "${tSpellID}" "${tString3}"
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Added "${tNoStack}" to the NoStack list for ${Spell[${tSpellID}].ID}=${Spell[${tSpellID}].Name}
      /return TRUE
   }
/return FALSE


Sub GetMinLevel(int tSpellID)
   /declare a int local
   /declare tMinClassLevel int local 0
   /declare tMinPCLevel int local 0
   /declare tString1 string local
   /if (!${tSpellID}) /return FALSE
   /if (${Me.Class.ShortName.Equal[BRD]}) /return FALSE
   /if (!${Spell[${tSpellID}].ID} || ${Select[${Spell[${tSpellID}].TargetType},self,pet]} || ${Spell[${tSpellID}].SpellType.Equal[detrimental]} || !${Spell[${tSpellID}].Duration}) /return FALSE
   /varset tString1 ${Ini["${MBCommon}","MinLevel","${tSpellID}","NOTFOUND"]}   
   /if (${tString1.Length} && ${tString1.NotEqual[NOTFOUND]}) {
      /varset tMinPCLevel ${tString1.Arg[2,|]}
      /if (${tMinPCLevel}>0 && ${tMinPCLevel}<255) {
         /return ${tMinPCLevel}
      }
   }
   /varset tMinClassLevel 255
   /for a 1 to ${pcClassCount}
      /if (${Spell[${tSpellID}].Level[${a}]} < ${tMinClassLevel}) {
         /varset tMinClassLevel ${Spell[${tSpellID}].Level[${a}]}
      }
   /next a
   /if (${tMinClassLevel}>=255) /return FALSE
   /if (${tMinClassLevel} > 65) {
      /varset tMinPCLevel 61
   } else /if (${tMinClassLevel} > 50) {
      /varset tMinPCLevel ${Math.Calc[${tMinClassLevel}/2+15].Int}
   }
   /if (${tMinPCLevel}>0 && ${tMinPCLevel}<255) {
      /ini "${MBCommon}" "MinLevel" "${tSpellID}" "${Spell[${tSpellID}].Name}|${tMinPCLevel}"
      /return ${tMinPCLevel}
   }
/return FALSE


Sub BuffWillStack(int tSpellID,string tBuffs)
   /if (!${tBuffs.Length}) /return TRUE
   /declare a int local
| The NetBots buff strings have a trailing space, so Count is correct without adding +1
   /for a 1 to ${tBuffs.Count[ ]}
      /if (!${Spell[${tSpellID}].WillStack[${tBuffs.Arg[${a}]}]}) {
|/echo spell ${Spell[${tSpellID}].Name} will not stack because ${Spell[${tBuffs.Arg[${a}]}].Name} exists
         /return FALSE
      }
   /next a
|/echo spell ${Spell[${tSpellID}].Name} will stack
/return TRUE
