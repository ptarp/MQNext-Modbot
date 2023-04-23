
#event OUTDOORS "#*#You can only cast this spell in the outdoors#*#"
#event OUTDOORS "#*#You can't levitate in this zone#*#"

Sub LoadABINIVals
   /declare a int local
   /declare b int local
   /call LoadINIVar AdvBuff ABCount 0 ABCount "${IniFile}" int
   /call LoadINIVar AdvBuff ABNewFormat 0 ABNewFormat "${IniFile}" int
   /declare ABCheckTimer timer outer
   /declare ABAliasList string outer
   /declare ABHasRecast bool outer FALSE
   /declare ABPetAB int outer 0
   /declare ABMountAB int outer 0
   /declare ABSelfShrinkAB int outer 0
   /declare ABSelfShrinkSize string outer
   /declare ABPetShrinkAB int outer 0
   /declare ABPetShrinkSize string outer
   /declare tABinterruptFlag bool outer
   /if (${ABCount}) {
      /call LoadINIVar AdvBuff ABMobMax 18 ABMobMax "${IniFile}" int
      /call LoadINIVar AdvBuff ABCheckTime 8 ABCheckTime "${IniFile}" int
      /declare DoBuffsCBT bool outer FALSE
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
      /Declare ABWontStack[${ABCount}] string outer
      /declare tSpellID int local
      /declare ABGemInUse string outer
      /declare SMPetCast bool outer FALSE
      /declare OutDoorsFail bool outer FALSE
      /declare tString1 string local
      /declare tString2 string local
      /declare ABSpellIconX[${ABCount}] string outer
      /for a 1 to ${ABCount}
         /if (${ABNewFormat}) {
            /call LoadINIVar AB${a} Gem " " ABGem[${a}] ${IniFile}
            /call LoadINIVar AB${a} Spell " " ABSpell[${a}] ${IniFile}
            /call LoadINIVar AB${a} SpellFoci " " ABSpellFoci[${a}] ${IniFile}
            /call LoadINIVar AB${a} DurMod 0 ABDurMod[${a}] ${IniFile}
            /call LoadINIVar AB${a} SpellAlias " " ABSpellAlias[${a}] ${IniFile}
            /call LoadINIVar AB${a} Announce " " ABAnnounce[${a}] ${IniFile}
            /call LoadINIVar AB${a} SpellMinMana 0 ABSpellMinMana[${a}] ${IniFile}
            /call LoadINIVar AB${a} TarCnt 0 ABTarCnt[${a}] ${IniFile}
            /call LoadINIVar AB${a} TarType "tank war shd pal rng mnk rog brd bst ber shm clr dru wiz mag enc nec self mypet grp pet cbt idle" ABTarType[${a}] ${IniFile}
            /call LoadINIVar AB${a} Recast FALSE ABRecast[${a}] ${IniFile}
            /call LoadINIVar AB${a} SpellIcon " " ABSpellIcon[${a}] ${IniFile}
            /call LoadINIVar AB${a} PreCondition "TRUE" ABCondition[${a}] ${IniFile}
         } else {    
            /call LoadINIVar AdvBuff ABGem${a} " " ABGem[${a}] ${IniFile}
            /call LoadINIVar AdvBuff ABSpell${a} " " ABSpell[${a}] ${IniFile}
            /call LoadINIVar AdvBuff ABSpellFoci${a} " " ABSpellFoci[${a}] ${IniFile}
            /call LoadINIVar AdvBuff ABDurMod${a} 0 ABDurMod[${a}] ${IniFile}
            /call LoadINIVar AdvBuff ABSpellAlias${a} " " ABSpellAlias[${a}] ${IniFile}
            /call LoadINIVar AdvBuff ABAnnounce${a} " " ABAnnounce[${a}] ${IniFile}
            /call LoadINIVar AdvBuff ABSpellMinMana${a} 0 ABSpellMinMana[${a}] ${IniFile}
            /call LoadINIVar AdvBuff ABTarCnt${a} 0 ABTarCnt[${a}] ${IniFile}
            /call LoadINIVar AdvBuff ABTarType${a} "tank war shd pal rng mnk rog brd bst ber shm clr dru wiz mag enc nec self mypet grp pet cbt idle" ABTarType[${a}] ${IniFile}
            /call LoadINIVar AdvBuff ABRecast${a} FALSE ABRecast[${a}] ${IniFile}
            /call LoadINIVar AdvBuff ABSpellIcon${a} " " ABSpellIcon[${a}] ${IniFile}
            /call LoadINIVar AdvBuff ABPreCondition${a} "TRUE" ABCondition[${a}] ${IniFile}
         }
|        /if (${ABSpellIcon[${a}].Length}>1 && !${Spell[${ABSpellIcon[${a}]}].ID}) /echo ${If[${TimeStampOnEcho},[${Time}] ,]}ABSpellIcon${a} does not evaluate to a spell name.  Please check your spelling.
         /if (${ABSpellIcon[${a}].Length}>1) {
            /varset tString1 ${ABSpellIcon[${a}].Token[1,|]}
            /if (${tString1.Length}>1) {
               /varset tString2 ${ABSpellIcon[${a}].Token[2,|]}
            } else { 
               /varset tString2 ${ABSpellIcon[${a}].Arg[1,|]}
            }
|/echo a1 - ${tString1}
|/echo a2 - ${tString2}
            /if (${tString2.NotEqual[null]} && ${tString2.Length}>1) {
               /for b 1 to ${Math.Calc[${tString2.Count[,]}+1]}
                  /if (${Spell[${tString2.Arg[${b},,]}].ID}) {
                     /varset ABSpellIconX[${a}] ${Spell[${tString2.Arg[${b},,]}].ID} ${ABSpellIconX[${a}]}
                  } else {
                     /echo ${If[${TimeStampOnEcho},[${Time}] ,]}ABSpellIcon${a} Additional (${tString2.Arg[${b},,]}) does not evaluate to a spell name.  Please check your spelling.
                  }
               /next b 
| /echo ABSpellIconX${a} ${ABSpellIconX[${a}]}
            }
            /if (${tString1.NotEqual[null]} && ${tString1.Length}>1) {
               /if (!${Spell[${tString1}].ID}) {
                  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}ABSpellIcon${a} (${tString1}) does not evaluate to a spell name.  Please check your spelling.
                  /varset ABSpellIcon[${a}]
               } else {
                  /varset ABSpellIcon[${a}] ${tString1}
               }
            } else {
               /varset ABSpellIcon[${a}]
            }
| /echo ABSpellIcon${a} ${ABSpellIcon[${a}]}
         } else {
            /varset ABSpellIcon[${a}]
         }
         /if (${ABTarType[${a}].Find[cbt]}) /varset DoBuffsCBT TRUE
         /varset ABAliasList ${ABAliasList} ${ABSpellAlias[${a}]}|
         /if (${Select[${ABGem[${a}]},${ValidSpellGems}]} && ${ABSpell[${a}].Length}) {
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
               /bc Msg: Spell ${ABSpell[${a}]} Not found in your book. Please check your .ini settings.
               /varset ABTarCnt[${a}] 0
            }
         } else /if (${ABGem[${a}].Find[item]}) {
            /if (${FindItem[=${ABSpell[${a}]}].ID}) {
               /varset tSpellID ${FindItem[=${ABSpell[${a}]}].Spell.ID}
            } else {
               /bc Msg: Buff Item not found: ${ABSpell[${a}]}. Check your .ini settings (or get more of the item).
            }
         } else /if (${ABGem[${a}].Equal[alt]}) {
            /if (!${Me.AltAbility[${ABSpell[${a}]}].ID} && !${Me.Shrouded}) {
               /bc Msg: You don't have the AltAbility ${ABSpell[${a}]}. Please check your .ini settings.
               /varset ABTarCnt[${a}] 0
            } else {
               /varset ABSpellID[${a}] ${Me.AltAbility[${ABSpell[${a}]}].Spell.ID}
            }
         } else /if (${ABGem[${a}].Equal[disc]}) {
            /if (${Me.CombatAbility[${Spell[${ABSpell[${a}]}].RankName}]}) {
               /varset ABSpell[${a}] ${Spell[${ABSpell[${a}]}].RankName}
               /varset ABSpellID[${a}] ${Me.CombatAbility[${Me.CombatAbility[${ABSpell[${a}]}]}].ID}
            } else {
               /bc Msg: You don't have the ${ABSpell[${a}]} Disc. Please check your .ini settings.
               /varset ABTarCnt[${a}] 0
            }
         }
         /if (${ABRecast[${a}]}) {
            /if (${Spell[${ABSpellID[${a}]}].TargetType.Find[Group]} || ${ABTarType[${a}].Find[self]}) /varset ABRecast[${a}] FALSE
         }
         /if (${ABRecast[${a}]}) /varset ABHasRecast TRUE
         /if (${ABTarType[${a}].Find[petspell]} || ${ABTarType[${a}].Find[petcast]}) {
            /varset ABPetAB ${a}
         } else /if (${ABTarType[${a}].Find[mountspell]} || ${ABTarType[${a}].Find[mountcast]}) {
            /varset ABMountAB ${a}
         } else /if (${ABTarType[${a}].Find[selfshrink]}) {
            /varset ABSelfShrinkAB ${a}
            /varset ABSelfShrinkSize ${If[${ABTarType[${a}].Arg[2,|].Length},${ABTarType[${a}].Arg[2,|]},1.5]}
         } else /if (${ABTarType[${a}].Find[petshrink]}) {
            /varset ABPetShrinkAB ${a}
            /varset ABPetShrinkSize ${If[${ABTarType[${a}].Arg[2,|].Length},${ABTarType[${a}].Arg[2,|]},1.4]}
         }
      /next a
   }

| ABWontStack[${a}] - new idea.  Entry in MBCommon.ini "[Nostack]" stores spellid=spellname,spellname,spellname of spells that won't stack with the spellid.
| works the same as ABSpellIcon without having to edit every toon's .ini file.
| Initial entry is the spellid=spellname of the buff in question. 
   /declare x int local
   /declare z int local
   /declare tIniEntry string local
   /for x 1 to ${ABCount}
      /if (${Select[${ABGem[${x}]},${ValidSpellGems}]} && !${Spell[${ABSpell[${x}]}].TargetType.Equal[self]}) {
         /if (${Ini["${MBCommon}",Nostack,"${Spell[${ABSpell[${x}]}].ID}","NOTFOUND"].Equal[NOTFOUND]} || !${Ini["${MBCommon}","Nostack","${Spell[${ABSpell[${x}]}].ID}"].Length}) {
            /ini "${MBCommon}" "NoStack" "${Spell[${ABSpell[${x}]}].ID}" "${ABSpell[${x}]}"
            /varset ABWontStack[${x}] ${Spell[${ABSpell[${x}]}].ID}
         } else {
            /varset tIniEntry ${Ini["${MBCommon}","Nostack","${Spell[${ABSpell[${x}]}].ID}"]}
            /if (${tIniEntry.Find[,]}) {
               /for z 1 to ${Math.Calc[${tIniEntry.Count[,]}+1]}
                  /varset ABWontStack[${x}] ${ABWontStack[${x}]} ${Spell[${tIniEntry.Arg[${z},,]}].ID}
               /next z
            } else {
               /varset ABWontStack[${x}] ${Spell[${tIniEntry}].ID}
            }
         }
         | /echo ABWontStack[${x}] will be ${ABWontStack[${x}]}
      }
   /next x
   
/return



Sub ABBuffCheck
   /if (${MBPause} || ${ABCheckTimer} || ${APPullDelay}) /return
   /varset ABCheckTimer ${ABCheckTime}s

   /declare a int local
   /declare b int local
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
         /if (${Me.Class.ShortName.Equal[BRD]}) {
            /if (${Me.Gem[${tGem}].ID}!=${ABSpellID[${a}]}) {
               /echo Memming ${ABSpell[${a}]} in gem ${tGem}
               /stopsong
               /delay 1s !${Me.Casting.ID}
               /memorize "${ABSpell[${a}]}|gem${tGem}"
               /delay 3s ${Me.Gem[${tGem}].ID}==${ABSpellID[${a}]}
            }
            /if (!${ABTarType[${a}].Find[aura]}) {
               /if (${Spell[${tSpellID}].DurationWindow} || ${Spell[${tSpellID}].Trigger.DurationWindow} || ${Spell[${tSpellID}].Trigger[1].DurationWindow}) {
                  /if (${ABTarType[${a}].Find[cbt]} || ${Spell[${tSpellID}].SpellType.Equal[detrimental]}) {
                     /call EditTwist cbt ${tGem} add
                     |/varset BardCombatGems ${Macro.Return}
                     /if (${ABTarType[${a}].Find[idle]}) {
                        /call EditTwist rest ${tGem} add
                        |/varset BardRestGems ${Macro.Return}
                     }
                  } else {
                     /call EditTwist rest ${tGem} add
                     |/varset BardRestGems ${Macro.Return}
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
         /varset tSpellID ${FindItem[=${ABSpell[${a}]}].Spell.ID}
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
         /varset tSpellID ${Spell[${ABSpell[${a}]}].ID}
      }
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
         /if (${Me.Height}<=${ABSelfShrinkSize} || ${SelfShrinkTries}>2) /continue
         /varset tTargetID ${Me.ID}
         /call BuffThem ${a} ${tSpellID} ${tGem} ${tTargetID} 4
      } else /if (${ABPetShrinkAB}==${a}) {
         /if (!${Me.Pet.ID} || !${DoPetShrink}) /continue
         /if (${Me.Pet.Height}<=${ABPetShrinkSize} || ${PetShrinkTries}>2) /continue
         /varset tTargetID ${Me.Pet.ID}
         /call BuffThem ${a} ${tSpellID} ${tGem} ${tTargetID} 5
      } else /if (${ABTarType[${a}].Find[self]}) {
         /varset tTargetID ${Me.ID}
         /call BuffThem ${a} ${tSpellID} ${tGem} ${tTargetID} 1
      } else /if (${ABTarType[${a}].Find[mypet]}) {
         /if (${Me.Pet.ID} && ${Select[${Me.Class.ShortName},shm,nec,mag,bst,enc,shd]}) {
            /varset tTargetID ${Me.Pet.ID}
            /call BuffThem ${a} ${tSpellID} ${tGem} ${tTargetID} 1
         }
      } else /if (${ABTarType[${a}].Find[tank]} && ${TankName.Length}) {
         /varset tTargetID ${Spawn[=${TankName}].ID}
         /if (${tTargetID}) {
            /call BuffThem ${a} ${tSpellID} ${tGem} ${tTargetID} ${If[${NetBots[${TankName}].ID},1,0]}
         }
      } else {
| The seemingly-redundant use of ${Spawn} in these sections is to make sure they are in the zone with me.
         /for b 1 to ${NetBots.Counts}
            /if (${NetBots[${NetBots.Client.Arg[${b}]}].ID} && ${Spawn[${NetBots[${NetBots.Client.Arg[${b}]}].ID}].ID}) {
			   /varset tName ${NetBots.Client.Arg[${b}]}
               /if (${ABTarType[${a}].Find[grp]}) {
                  /if (!${Group.Member[${tName}].Index} && !${tName.Equal[${Me.CleanName}]}) /continue
               } else /if (${ABTarType[${a}].Find[oog]}) {
                  /if (${Group.Member[${tName}].Index} || ${tName.Equal[${Me.CleanName}]}) /continue
               } 
               /if (${ABTarType[${a}].Find[nogroup]}) {
                  /if (${NetBots[${Spawn[${tTargetID}].Master}].Grouped}  || ${NetBots[${tName}].Grouped}) /continue
               } else /if (${ABTarType[${a}].Find[isgrouped]}) {
                  /if (!${NetBots[${Spawn[${tTargetID}].Master}].Grouped}  || !${NetBots[${tName}].Grouped}) /continue
               }
               /if (${ABTarType[${a}].Find[${NetBots[${tName}].Class.ShortName}]}) {
 			      /call BuffThem ${a} ${tSpellID} ${tGem} ${NetBots[${tName}].ID} 1
				  /if (${tABinterruptFlag}) /break
			   } 
               /if (${NetBots[${tName}].PetID} && ${ABTarType[${a}].Find[pet]}) {
                  /if (${Select[${NetBots[${tName}].Class.ShortName},shm,nec,mag,bst,enc,shd]}) {
                     /call BuffThem ${a} ${tSpellID} ${tGem} ${NetBots[${tName}].PetID} 1
                     /if (${tABinterruptFlag}) /break
                  }
               }
            }
         /next b
		 /if (${tABinterruptFlag}) /break
         /if (${Group.Members} && !${ABTarType[${a}].Find[oog]}) {
            /for b 1 to ${Group.Members}
               /if (!${NetBots[${Group.Member[${b}].Name}].ID} && ${Spawn[${Group.Member[${b}].Name}].ID}) {
                  /if (${ABTarType[${a}].Find[${Group.Member[${b}].Class.ShortName}]}) {
			         /call BuffThem ${a} ${tSpellID} ${tGem} ${Group.Member[${b}].ID} 0
				     /if (${tABinterruptFlag}) /break
			      }
                  /if (${Group.Member[${b}].Pet.ID} && ${ABTarType[${a}].Find[pet]}) {
                     /if (${Select[${Group.Member[${b}].Class.ShortName},shm,nec,mag,bst,enc,shd]}) {
                        /call BuffThem ${a} ${tSpellID} ${tGem} ${Group.Member[${b}].Pet.ID} 0
                        /if (${tABinterruptFlag}) /break
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
            /if (!${Spell[${Me.Aura}].ID}) {
               /disc ${Spell[${ABSpell[${a}]}].ID}
               /delay 1s ${Me.Casting.ID}
               /delay ${Math.Calc[${Spell[${ABSpell[${a}]}].MyCastTime}/100+5].Int} !${Me.Casting.ID}
            }
            /return
         }
		 /if (${ABSpell[${a}].Find[${Me.Aura[1]}]} || ${ABSpell[${a}].Find[${Me.Aura[2]}]}) /return
		 /if (${Spell[${Me.Aura}].ID}) {
		    /if (!${Me.AltAbility[Auroria Mastery]} && !${Me.AltAbility[Spirit Mastery]}) /return
		    /if (${Spell[${Me.Aura[1]}].ID} && ${Spell[${Me.Aura[2]}].ID}) /return
		 }
      } else /if (${Me.Class.ShortName.Equal[mag]} && ${ABTarType[${a}].Find[mypet]}) { 
         /if (${Me.PetBuff[${ABSpell[${a}]} Effect]} || !${NetBots[${Me.CleanName}].StacksPet[${ABSpell[${a}]} Effect]}) /return
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
            /if (!${NetBots[${Me.CleanName}].Stacks[${tSpellID}]}) /goto :tBTReturn
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
| /echo ${sSpellID} - *${sVar}*${sVarT}*${tTime}*
      /if (${tBot}) {
         /varset tBuffs ${If[${Spawn[${tTargetID}].Type.Equal[pc]},${NetBots[${tName}].Buff} ${NetBots[${tName}].ShortBuff},${NetBots[${Spawn[${tTargetID}].Master.CleanName}].PetBuff}]}
         /if (${tBuffs.Find[${tSpellID}]}) {
            /if (!${tTime} || ${Spawn[${tTargetID}].Type.Equal[pet]}) /goto :tBTReturn
         } else {
            /if (${ABSpellIconX[${a}].Length}>1) {
               /for c 1 to ${Math.Calc[${ABSpellIconX[${a}].Count[ ]}+1]}
                  /if (${tBuffs.Find[${ABSpellIconX[${a}].Arg[${c}]}]}) {
| /echo skipping ABSpellIconX${a} - ${ABSpellIconX[${a}]} - ${ABSpellIconX[${a}].Arg[${c}]}
                     /goto :tBTReturn
                  }
               /next c 
| /echo ABSpellIconX${a} ${ABSpellIconX[${a}]}
            }
            /if (${ABWontStack[${a}].Length}>1 && ${Spell[${tSpellID}].TargetType.NotEqual[Self]}) {
               /if (${tBuffs.Find[${ABWontStack[${a}]}]} || ${tBuffs.Find[${tSpellID}]}) /goto :tBReturn
               /for c 1 to ${Math.Calc[${ABWontStack[${a}].Count[ ]}+1]}
                  /if (${tBuffs.Find[${ABWontStack[${a}].Arg[${c}]}]}) /goto :tBTReturn
               /next c
            }
            /if (${Spawn[${tTargetID}].Type.Equal[pc]}) {
               /if (!${NetBots[${tName}].FreeBuffSlots}) /goto :tBTReturn
            }
            /if (${tBuffs.Length} && ${ABTarType[${a}].Find[${Spawn[${tTargetID}].Class.ShortName}]} || (${ABTarType[${a}].Find[self]} && ${tTargetID}==${Me.ID}) || (${ABTarType[${a}].Find[pet]} && ${Spawn[${tTargetID}].Master.ID}) || ${ABTarType[${a}].Find[tank]}) {
               /if (${Spawn[${tTargetID}].Type.Equal[pc]}) {
                  /if (!${NetBots[${tName}].Stacks[${tSpellID}]} || ${NetBots[${tName}].Buff.Find[${tSpellID}]}) {
|                    /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${ABSpell[${a}]} - Won't stack on ${tName}
                     /goto :tBTReturn
                  }
               } else {
                  /if (!${NetBots[${Spawn[${tTargetID}].Master.CleanName}].StacksPet[${tSpellID}]}) {
|                    /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${ABSpell[${a}]} - Won't stack on ${tName}
                     /goto :tBTReturn
                  }
               }
               /varset tTime 0
            }
         }
      }
      /varset tSpellRange ${If[${Spell[${tSpellID}].AERange},${Spell[${tSpellID}].AERange},${Spell[${tSpellID}].Range}]}
      /if (${tName.Equal[${Me.CleanName}]} && ${ABTarType[${a}].Find[self]}) {
         /if (${NetBots[${Me.CleanName}].Stacks[${tSpellID}]} && ${Me.FreeBuffSlots} && !${Me.Buff[${Spell[${tSpellID}]}].ID}) /varset tTime 0
      }
      /if (${tTime}>${Macro.RunTime}) /goto :tBTReturn
      /if (${Spawn[${tTargetID}].Distance3D}>${tSpellRange}) /goto :tBTReturn
      /if (${Spawn[${tTargetID}].Type.Equal[pet]} && !${Spawn[${tTargetID}].IsSummoned}) { 
         /if (${Spell[${ABSpell[${a}]}].TargetType.Find[group]} || ${Spell[${ABSpell[${a}]}].TargetType.Find[pet2]}) /goto :tBTReturn
      }
   }
   /if (${tGem}<20) {
      /if (${Me.CurrentMana}<${Spell[${tSpellID}].Mana} || ${Me.PctMana}<${ABSpellMinMana[${a}]}) /return
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
      /disc ${Spell[${ABSpell[${a}]}].ID}
      /delay 1s ${Me.Casting.ID}
      /delay ${Math.Calc[${Spell[${ABSpell[${a}]}].MyCastTime}/100+5].Int} !${Me.Casting.ID}
      /varset ABScriptTime[${a}] ${Math.Calc[${ABDurMod[${a}]}+${Macro.RunTime}]}
      /call AddtoBuffArray ${tTargetID} ${tSpellID} ${tSpellDelay} ${a}
      /return
   }
   /if (${Spell[${tSpellID}].TargetType.Equal[self]}) {
      /varset tTargetID 0
   }
   /if (${Window[SpellBookWnd].Open}) /book
   /varset ABtTargetID 0
   /varset tSpellDelay 0
   /varset CastResult UNKNOWN
   /varset tRecastTime 3s

   /while (1) {
      /if (${NetBots[${Me.CleanName}].Silenced} || ${NetBots[${Me.CleanName}].Charmed} || ${Select[${Me.State},STUN,HOVER,DEAD]}) {
         /varset tABinterruptFlag TRUE
         /return
      }
      /if (${DoHeals} && ${AHCount}) /call AHHealCheck
      /if (${ABtTargetID}) {
         /if (${Cast.Status.Equal[i]}) {
            /doevents OUTDOORS
            /if (${Cast.Result.Equal[CAST_OUTDOORS]} || ${OutDoorsFail}) {
               /bc Msg: Outdoor only spell, turning off buff [ ${ABSpell[${a}]} ] for this zone.  Check "${MBCommon}" if incorrect setting.
               /if (!${Ini["${MBCommon}",RestrictedList,${Zone.ID}].Find[${tSpellID}|]}) {
                  /if (${Ini["${MBCommon}",RestrictedList,${Zone.ID}].Length} >0 ) {
                     /ini "${MBCommon}" RestrictedList ${Zone.ID} ${Ini["${MBCommon}",RestrictedList,${Zone.ID}]}${tSpellID}|
                  } else {
                     /ini "${MBCommon}" RestrictedList ${Zone.ID} ${tSpellID}|
                     /echo ${If[${TimeStampOnEcho},[${Time}] ,]}/ini "${MBCommon}" RestrictedList ${Zone.ID} ${tSpellID}|
                  }
               }
               /varset OutDoorsFail FALSE
               /return
            }
            /if (${tBot}>1) {
               /if (${tBot}==2) {
                  /if (${Me.Pet.ID}) /break
               } else /if (${tBot}==3) {
                  /if (${Me.Mount.ID})) /break
               } else /if (${tBot}==4) {
			      /varcalc SelfShrinkTries ${SelfShrinkTries}+1
                  /if (${Me.Height}<=${ABSelfShrinkSize} || ${SelfShrinkTries}>2) /break
               } else /if (${tBot}==5) {
			      /varcalc PetShrinkTries ${PetShrinkTries}+1
                  /if (${Me.Pet.Height}<=${ABPetShrinkSize} || ${PetShrinkTries}>2) /break
               }
            }
			/if (${Cast.Result.Equal[cast_success]} || ${Me.Class.ShortName.Equal[BRD]}) /break
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
         /if (${tTargetID} && ${Target.ID}!=${tTargetID} && ${Spell[${tSpellID}].TargetType.NotEqual[self]} && ${Spell[${tSpellID}].TargetType.Equal[Single]}) {
            /if (${Melee.Combat}) /attack off
            /squelch /tar id ${tTargetID}
            /delay 1s ${Target.ID}==${tTargetID}
         }
         /varset ABtTargetID ${tTargetID}
         /if (${Me.State.Equal[sit]}) /stand
         /if (${tGem}==30) {
            /alt act ${tSpellID}
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
		    /varcalc SelfShrinkTries ${SelfShrinkTries}+1
         } else /if (${tBot}==5) {
		    /varcalc PetShrinkTries ${PetShrinkTries}+1
		 }
         /break
      } else {
         /delay 1s ${Cast.Ready}
         /if (${Cast.Ready[${ABSpell[${a}]}|${ABGem[${a}]}]}) {
            /varset ABtTargetID ${tTargetID}
            /if (${Me.Shrouded}) /delay 1s
            /varset MBCurrentSpell AB|${a}|${ABtTargetID}
            /doevents flush OUTDOORS
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
            /call MQ2Cast "${ABSpell[${a}]}" ${If[${Select[${ABGem[${a}]},${ValidSpellGems}]},gem,]}${ABGem[${a}]} ${tCastTime} CastCheck ${If[${ABtTargetID},${If[${Spawn[${ABtTargetID}].Type.Equal[pet]} && ${Spell[${ABSpell[${a}]}].TargetType.Find[group]},-targetid|${Spawn[${ABtTargetID}].Master.ID},-targetid|${ABtTargetID}]}, ]} ${If[${ABSpellFoci[${a}].Length},-bandolier|${ABSpellFoci[${a}]},]}
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
|  /if (${ADMobFound} || ${MBPause}) /return
|   /if (${FollowFlag}) {
|      /if (${Spawn[pc ${FollowID}].Distance}>${FollowDistance}) /return
|   }
|   /if (${Cursor.ID}) /call ClearCursor
|   /call ADSpawnCheck Find
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
   /if (${FollowFlag}) {
      /if (${Me.Moving}) /return FALSE
      /if (${Spawn[${FollowID}].Distance}>${FollowDistance}) /return FALSE
   }
   /if (${Cursor.ID}) /call ClearCursor
/return TRUE


Sub Event_OUTDOORS()
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Outdoor Spell..
   /varset OutDoorsFail TRUE
   /if (${Me.Class.ShortName.Equal[brd]}) {
      /declare x int local 0
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${Cast.Stored} will be turned off for this zone.  Re-start macro or use alias to turn it back on.
      /for x 1 to ${ABCount}
          /if (${ABSpell[${x}].Equal[${Cast.Stored}]}) /varset ABTarCnt[${x}] 0
      /next x
      /if (!${ACState}) { 
         /varset BardRestGems
         /varset BardCombatGems
      }
      /call ABBuffCheck
   }
/return