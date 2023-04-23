
#Event SpellResist "#*#target resisted the #1# spell#*#"
#Event SpellResist "#*#resisted your #1#!#*#"
#Event SpellResist "#*#avoided your #1#!#*#"
#Event SpellResist "#*#Your #1# spell is interrupted#*#"
#Event SpellResist "#*#Your #1# combat ability is interrupted#*#"
#Event SpellResist "#*#Your #1# spell fizzles#*#"
#Event SpellResist "#*#You can't cast spells while stunned#*#"
#Event SpellResist "#*#Your #1# spell did not take hold#*#"
#Event SpellResist "#*#Your target cannot be#*#"
#Event SpellResist "#*#You are stunned#*#"
#Event SpellResist "#*#You miss a note#*#"
#Event SpellResist "#*#You are missing some required components#*#"
#Event PetNoWake "#*#I am unable to wake #1#, Master#*#"
#Event PetNoWake "#*#forgiveness, Master. That is not a legal target#*#"

Sub LoadADINIVals
   /call LoadINIVar AdvDebuff ADCount 0 ADCount "${IniFile}" int
   /call LoadINIVar AdvDebuff ADNewFormat 1 ADNewFormat "${IniFile}" int
   /call LoadINIVar AdvDebuff ADMobMax 20 ADMobMax "${IniFile}" int
   /call LoadINIVar AdvDebuff ADCheckTime 2 ADCheckTime "${IniFile}" int
   /call LoadINIVar AdvDebuff ADAggroOnly 0 ADAggroOnly "${IniFile}" int
   /call LoadINIVar AdvDebuff ADHold "0|1|1|   1=on 0=off|Debuff spell #|Time to wait for debuff|" ADHold "${IniFile}" string
   /call LoadINIVar AdvDebuff ADCoolDown1 " " ADCoolDown1 "${IniFile}" string
   /call LoadINIVar AdvDebuff ADCoolDown2 " " ADCoolDown2 "${IniFile}" string
   /call LoadINIVar AdvDebuff ADCoolDown3 " " ADCoolDown3 "${IniFile}" string
   /call LoadINIVar AdvDebuff ADCoolDown4 " " ADCoolDown4 "${IniFile}" string
   /call LoadINIVar AdvDebuff ADCoolDown5 " " ADCoolDown5 "${IniFile}" string

   /if (${ADMobMax}==0) /varset ADMobMax 1

   /declare a int local
   /declare b int local
   /declare tSpellID int local
   /declare ADMobCount int outer 0
   /declare ADCheckTimer timer outer
   /declare ADHoldTimer timer outer
   /declare ADMobList[${ADMobMax}] int outer 0
   /declare MLDebuffed[${ADMobMax}] bool outer FALSE
   /declare CurrentMobCount int outer 0
   /declare SpawnCheckTimer timer outer
   /declare AddSpawn int outer 0
   /declare ADCharmID int outer 0
   /declare ADDropID int outer 0
   /declare ADCharmAD int outer 0
   /declare ADCharmType string outer
   /declare ADCharmName string outer
   /declare ADCharmSpell string outer xxx
   /declare ADEventSpell string outer
   /declare ADSpellResisted bool outer
   /declare ADResistMatch string outer
   /declare ADResistCount int outer
   /declare ADResistSpell[10] string outer
   /declare ADCDCount int outer 0
   /declare ADCDSpell[5] string outer
   /declare ADMaxNMA int outer 0
   /declare ADMaxPRI int outer 0
   /declare ADMobFound bool outer
   /declare SpawnFindTimer timer outer

   /if (${ADCount}) {
      /declare ADMDL[${ADMobMax},${ADCount}] int outer 0
   } else {
      /declare ADMDL[${ADMobMax},1] int outer 0
   }

   /if (${ADCount}) {
      /declare ADGem[${ADCount}] string outer
      /declare ADSpell[${ADCount}] string outer
      /declare ADSpellFoci[${ADCount}] string outer
      /declare ADDurMod[${ADCount}] string outer
      /declare ADSpellMinMana[${ADCount}] int outer
      /declare ADSpellRecast[${ADCount}] string outer
      /declare ADSpellRecast1[${ADCount}] int outer
      /declare ADSpellRecast2[${ADCount}] int outer
      /declare ADSpellCastonResist[${ADCount}] string outer
      /declare ADSpellDelay[${ADCount}] int outer
      /declare ADTarCnt[${ADCount}] int outer
      /declare ADTarType[${ADCount}] int outer
      /declare ADTarBegHP[${ADCount}] int outer
      /declare ADTarEndHP[${ADCount}] int outer
      /declare ADSpellAlias[${ADCount}] string outer
      /declare ADAnnounce[${ADCount}] string outer
      /declare ADDiscTimer[${ADCount}] int outer
      /declare ADIfSpellImmune[${ADCount}] string outer
      /declare ADUseHoTT[${ADCount}] int outer
      /declare ADCondition[${ADCount}] string outer
      /declare ADSameSpell[${ADCount}] int outer
      /declare ADBardSlot[${ADCount}] int outer
      /declare aSpellName string local
      /declare bSpellName string local
      /for a 1 to ${ADCount}
         /if (${ADNewFormat}) { 
            /call LoadINIVar AD${a} Gem " " ADGem[${a}] ${IniFile}
            /call LoadINIVar AD${a} Spell " " ADSpell[${a}] ${IniFile}
            /call LoadINIVar AD${a} SpellFoci " " ADSpellFoci[${a}] ${IniFile}
            /call LoadINIVar AD${a} DurMod 0 ADDurMod[${a}] ${IniFile}
            /call LoadINIVar AD${a} SpellAlias " " ADSpellAlias[${a}] ${IniFile}
            /call LoadINIVar AD${a} Announce " " ADAnnounce[${a}] ${IniFile}
            /call LoadINIVar AD${a} SpellMinMana 0 ADSpellMinMana[${a}] ${IniFile}
            /call LoadINIVar AD${a} SpellRecast "0|0" ADSpellRecast[${a}] ${IniFile}
            /call LoadINIVar AD${a} SpellCastonResist " " ADSpellCastonResist[${a}] ${IniFile}
            /call LoadINIVar AD${a} SpellDelay 0 ADSpellDelay[${a}] ${IniFile}
            /call LoadINIVar AD${a} TarCnt 0 ADTarCnt[${a}] ${IniFile}
            /call LoadINIVar AD${a} TarType 0 ADTarType[${a}] ${IniFile}
            /call LoadINIVar AD${a} TarBegHP 0 ADTarBegHP[${a}] ${IniFile}
            /call LoadINIVar AD${a} TarEndHP 0 ADTarEndHP[${a}] ${IniFile}
            /call LoadINIVar AD${a} IfSpellImmune " " ADIfSpellImmune[${a}] ${IniFile}
            /call LoadINIVar AD${a} UseHoTT "0" ADUseHoTT[${a}] ${IniFile}
            /call LoadINIVar AD${a} PreCondition "TRUE" ADCondition[${a}] ${IniFile}
         } else { 
            /call LoadINIVar AdvDebuff ADGem${a} " " ADGem[${a}] ${IniFile}
            /call LoadINIVar AdvDebuff ADSpell${a} " " ADSpell[${a}] ${IniFile}
            /call LoadINIVar AdvDebuff ADSpellFoci${a} " " ADSpellFoci[${a}] ${IniFile}
            /call LoadINIVar AdvDebuff ADDurMod${a} 0 ADDurMod[${a}] ${IniFile}
            /call LoadINIVar AdvDebuff ADSpellAlias${a} " " ADSpellAlias[${a}] ${IniFile}
            /call LoadINIVar AdvDebuff ADAnnounce${a} " " ADAnnounce[${a}] ${IniFile}
            /call LoadINIVar AdvDebuff ADSpellMinMana${a} 0 ADSpellMinMana[${a}] ${IniFile}
            /call LoadINIVar AdvDebuff ADSpellRecast${a} "0|0" ADSpellRecast[${a}] ${IniFile}
            /call LoadINIVar AdvDebuff ADSpellCastonResist${a} " " ADSpellCastonResist[${a}] ${IniFile}
            /call LoadINIVar AdvDebuff ADSpellDelay${a} 0 ADSpellDelay[${a}] ${IniFile}
            /call LoadINIVar AdvDebuff ADTarCnt${a} 0 ADTarCnt[${a}] ${IniFile}
            /call LoadINIVar AdvDebuff ADTarType${a} 0 ADTarType[${a}] ${IniFile}
            /call LoadINIVar AdvDebuff ADTarBegHP${a} 0 ADTarBegHP[${a}] ${IniFile}
            /call LoadINIVar AdvDebuff ADTarEndHP${a} 0 ADTarEndHP[${a}] ${IniFile}
            /call LoadINIVar AdvDebuff ADIfSpellImmune${a} " " ADIfSpellImmune[${a}] ${IniFile}
            /call LoadINIVar AdvDebuff ADUseHoTT${a} "0" ADUseHoTT[${a}] ${IniFile}
            /call LoadINIVar AdvDebuff ADPreCondition${a} "TRUE" ADCondition[${a}] ${IniFile}
         }
         /varset ADSpellRecast1[${a}] ${ADSpellRecast[${a}].Arg[1,|]}
         /if (${ADSpellRecast[${a}].Arg[2,|].Length}) {
            /varset ADSpellRecast2[${a}] ${ADSpellRecast[${a}].Arg[2,|]}
         } else {
            /varset ADSpellRecast2[${a}] ${ADSpellRecast1[${a}]}
         }
         /varset tSpellID 0
         /if (${Select[${ADGem[${a}]},${ValidSpellGems}]} && ${ADSpell[${a}].Length}) {
            /if (${Me.Book[${Me.Book[${ADSpell[${a}]}]}].ID}) {
               /varset ADSpell[${a}] ${Me.Book[${Me.Book[${ADSpell[${a}]}]}].Name}
            } else /if (${Me.Book[${Me.Book[${ADSpell[${a}]} ]}].ID}) {
               /varset ADSpell[${a}] ${Me.Book[${Me.Book[${ADSpell[${a}]} ]}].Name}
            } else /if (${Me.Book[${Spell[${ADSpell[${a}]}].RankName}]}) {
               /varset ADSpell[${a}] ${Spell[${ADSpell[${a}]}].RankName}
            } else {
               /bc Msg: Spell ${ADSpell[${a}]} Not found in your book.  Check your .ini settings.
               /varset ADTarCnt[${a}] 0
            }
            /varset tSpellID ${Me.Book[${Me.Book[${ADSpell[${a}]}]}].ID}
            /if (${tSpellID} && ${Select[${ADSpellAlias[${a}].Token[1,|]},charmspell,charmcast]}) {
               /varset ADCharmAD ${a}
               /varset ADCharmType ${ADSpellAlias[${a}].Token[2,|]}
               /varset ADCharmName ${ADSpellAlias[${a}].Token[3,|]}
               /varset ADCharmSpell ${ADSpell[${a}]}
               /if (!${ADCharmType.Length} || ${ADCharmType.Equal[NULL]}) /varset ADCharmType 0
            }
         } else /if (${ADGem[${a}].Find[item]}) {
            /if (${FindItem[=${ADSpell[${a}]}].ID}) {
               /varset tSpellID ${FindItem[=${ADSpell[${a}]}].Spell.ID}
            } else {
               /bc Msg: Debuff Item not found: ${ADSpell[${a}]}. Check your .ini settings (or get more of the item).
            }
         } else /if (${ADGem[${a}].Equal[alt]}) {
            /if (!${Me.AltAbility[${ADSpell[${a}]}].ID} && !${Me.Shrouded}) {
               /bc Msg:  You don't have the AltAbility ${ADSpell[${a}]}.  Check your .ini settings.
               /varset ADTarCnt[${a}] 0
            }
            /varset tSpellID ${Me.AltAbility[${ADSpell[${a}]}].Spell.ID}
         } else /if (${ADGem[${a}].Equal[disc]}) {
            /if (${Me.CombatAbility[${Spell[${ADSpell[${a}]}].RankName}]}) {
               /varset ADSpell[${a}] ${Spell[${ADSpell[${a}]}].RankName}
            } else {
               /bc Msg: You don't have the ${ADSpell[${a}]} Disc.  Please check ADSpell${a} in your .ini
               /varset ADTarCnt[${a}] 0
            }
         } else {
            /bc Msg: Debuff #${a} is invalid. Check your .ini settings.
            /varset ADTarCnt[${a}] 0			
         }
		 /if (${Spell[${ADSpell[${a}]}].ID}) {
            /call LoadINIVar "${Spell[${ADSpell[${a}]}].Name}" "MaxLevel" "150" "MaxLevel${Spell[${ADSpell[${a}]}].ID}" "${MBCommon}" string
		 }
         /if (${tSpellID}) {
            /if (!${Defined[Imm${tSpellID}]}) /declare Imm${tSpellID} string outer
            /if (${Ini["${MBCommon}","${Spell[${ADSpell[${a}]}].Name}","${Zone.ShortName}Immune"].Find[|]}) /call LoadINIVar "${Spell[${ADSpell[${a}]}].Name}" "${Zone.ShortName}Immune" " " "Imm${tSpellID}" "${MBCommon}" string
         }
         /if (${ADTarCnt[${a}]}) { 
            /if (${Select[${ADTarType[${a}]},0,10,2,12]}) /varset ADMaxNMA ${a}
            /if (${ADTarType[${a}]}>9) /varset ADMaxPRI ${a}
         }
      /next a
      /if (${ADCharmAD}) {
         /if (${Me.Pet.ID} && !${Me.Pet.IsSummoned} && (!${ADCharmType} || (${ADCharmType}==1 && ${Select[${Spawn[${Me.Pet.ID}].Body.Name},undead,vampyre]}) || (${ADCharmType}==2 && ${Select[${Spawn[${Me.Pet.ID}].Body.Name},animal,insect]}) || (${ADCharmType}==3 && ${Spawn[${Me.Pet.ID}].Body.Name.Find[summoned]}) || (${ADCharmType}==4 && ${ADCharmName.Find[${Spawn[${Me.Pet.ID}].CleanName}]}))) {
            /varset ADCharmID ${Me.Pet.ID}
            /varset PetAssist 1
            /varset DoPet TRUE
            /varset ADMobList[1] ${Me.Pet.ID}
            /varset ADMDL[1,${a}] ${Spell[${ADCharmSpell}].Duration.TotalSeconds}
            /bc Msg: I have a Charmed pet: ${ADCharmID} - ${Spawn[${ADCharmID}].CleanName}
            /bc ExcludeID ${ADCharmID}
         }
      }
      /if (${ADCount}>1) {
         /for a 1 to ${Math.Calc[${ADCount}-1]}
            /varset aSpellName
            /if (!${ADSameSpell[${a}]}) {
               /if (${Select[${ADGem[${a}]},${ValidSpellGems}]}) {
                  /if (!${Spell[${ADSpell[${a}]}].TargetType.Find[ ae]}) { 
                     /if (${Spell[${ADSpell[${a}]}].Trigger.ID}) {	
                        /varset aSpellName ${Spell[${ADSpell[${a}]}].Trigger.BaseName}
                     } else /if (${Spell[${ADSpell[${a}]}].Trigger[1].ID}) {	
                        /varset aSpellName ${Spell[${ADSpell[${a}]}].Trigger[1].BaseName}
                     } else {
                        /varset aSpellName ${Spell[${ADSpell[${a}]}].BaseName}
                     }
                  }
               } else /if (${ADGem[${a}].Equal[alt]}) {
                  /if (!${Me.AltAbility[${ADSpell[${a}]}].Spell.TargetType.Find[ ae]}) { 
                     /if (${Me.AltAbility[${ADSpell[${a}]}].Spell.Trigger.ID}) {	
                        /varset aSpellName ${Me.AltAbility[${ADSpell[${a}]}].Spell.Trigger.BaseName}
                     } else /if (${Me.AltAbility[${ADSpell[${a}]}].Spell.Trigger[1].ID}) {	
                        /varset aSpellName ${Me.AltAbility[${ADSpell[${a}]}].Spell.Trigger[1].BaseName}
                     } else {
                        /varset aSpellName ${Me.AltAbility[${ADSpell[${a}]}].Spell.BaseName}
                     }
                  }
               } 
            }
            /if (${aSpellName.Length} && ${Spell[${aSpellName}].Duration}) {
               /for b ${Math.Calc[${a}+1]} to ${ADCount}
                  /varset bSpellName
                  /if (${Select[${ADGem[${b}]},${ValidSpellGems}]}) {
                     /if (!${Spell[${ADSpell[${b}]}].TargetType.Find[ ae]}) { 
                        /if (${Spell[${ADSpell[${b}]}].Trigger.ID}) {	
                           /varset bSpellName ${Spell[${ADSpell[${b}]}].Trigger.BaseName}
                        } else /if (${Spell[${ADSpell[${b}]}].Trigger[1].ID}) {	
                           /varset bSpellName ${Spell[${ADSpell[${b}]}].Trigger[1].BaseName}
                        } else {
                           /varset bSpellName ${Spell[${ADSpell[${b}]}].BaseName}
                        }
                     }
                  } else /if (${ADGem[${b}].Equal[alt]}) {
                     /if (!${Me.AltAbility[${ADSpell[${b}]}].Spell.TargetType.Find[ ae]}) { 
                        /if (${Me.AltAbility[${ADSpell[${b}]}].Spell.Trigger.ID}) {	
                           /varset bSpellName ${Me.AltAbility[${ADSpell[${b}]}].Spell.Trigger.BaseName}
                        } else /if (${Me.AltAbility[${ADSpell[${b}]}].Spell.Trigger[1].ID}) {	
                           /varset bSpellName ${Me.AltAbility[${ADSpell[${b}]}].Spell.Trigger[1].BaseName}
                        } else {
                           /varset bSpellName ${Me.AltAbility[${ADSpell[${b}]}].Spell.BaseName}
                        }
                     }
                  } 
                  /if (${bSpellName.Equal[${aSpellName}]}) {
                     /varset ADSameSpell[${a}] ${b}
                     /varset ADSameSpell[${b}] ${a}
                     /break
                  }
               /next b
            }
         /next a
      }
      /if (${Me.Class.ShortName.NotEqual[BRD]}) {
         /for a 1 to 5
            /if (${ADCoolDown${a}.Length}) {
               /varcalc ADCDCount ${ADCDCount}+1
               /varset ADCDSpell[${ADCDCount}] ${ADCoolDown${a}}
            }
         /next a
      }
   }
/return


Sub ADSpawnCheck(string scParm)
   /varset AddSpawn 0
   /if (${SpawnCheckTimer}) /return
   /if (${scParm.Equal[find]} && ${SpawnFindTimer}) /return
   /declare a int local
   /declare b int local
   /declare tNPCCount int local 0
   /declare npcid int local 0
   /declare tADAdd bool local FALSE

   /varset ADMobFound FALSE
   /if (${scParm.NotEqual[find]}) /varset ADMobCount 0

   /if (${Select[${Zone.ShortName},GuildHall,GuildLobby,PoKnowledge,Nexus,Bazaar,AbysmalSea,potranquility]}) /return FALSE
   /if (${ZoneName.NotEqual[${Zone.Name}]}) /call ProcessExcludeList
   /declare TotalNPCs int local ${SpawnCount[any radius ${Radius} zradius ${ZRadius} noalert ${AlertList}]}
   /while (1) {
      /varcalc tNPCCount ${tNPCCount}+1
      /if (${tNPCCount}>${TotalNPCs}) /break
      /varset npcid ${NearestSpawn[${tNPCCount},any radius ${Radius} zradius ${ZRadius} noalert ${AlertList}].ID}
      /if (!${npcid}) /break
|     /if (${Select[${Spawn[${npcid}].Type},pc,chest,untargetable,trigger,trap,timer,item,object,banner,campfire,mercenary,mount,aura,corpse,flyer]}) /continue
      /if (!${Select[${Spawn[${npcid}].Type},npc,pet]}) /continue
      /if (${Spawn[${npcid}].Master.Type.Equal[pc]}) /continue
      /if (${Spawn[${npcid}].Body.Name.Equal[object]} || ${Spawn[${npcid}].Race.Name.Equal[campfire]} || (${Spawn[${npcid}].Name.Left[1].Equal[_]} && ${Spawn[${npcid}].CleanName.Length}<2)) /continue
      /if (!${Spawn[${npcid}].LineOfSight}) {
         /if (${ACMATarget}!=${npcid}) /continue
         /if (${Spawn[${npcid}].Distance3D}>25) /continue
      }
      /if (!${Spawn[${npcid}].ID}) /continue
      /if (${ADAggroOnly}) {
         /if (!${Spawn[${npcid}].Aggressive}) /continue
         /if (${ADAggroOnly}==2) /goto :IncSpawn
         /if (${Target.ID}!=${npcid}) /squelch /tar id ${npcid}
         /delay 1s ${Target.ID}==${npcid}
         /if (${Target.ID}==${npcid}) {
            /delay 1s ${Me.TargetOfTarget.ID} 
            /if (${Me.TargetOfTarget.ID}) {
               /if (${Me.TargetOfTarget.Type.Equal[pc]}) {
                  /if (${Me.TargetOfTarget.ID}==${Me.ID}) /goto :IncSpawn
                  /if (${Group.Member[${Me.TargetOfTarget.CleanName}].Index}) /goto :IncSpawn
                  /if (${NetBots[${Me.TargetOfTarget.CleanName}].ID}) /goto :IncSpawn
                  /if (${Spawn[raid id ${Me.TargetOfTarget.ID}].ID}) /goto :IncSpawn
               } else /if (${Me.TargetOfTarget.Master.Type.Equal[pc]}) {
                  /if (${Me.TargetOfTarget.ID}==${Me.Pet.ID}) /goto :IncSpawn
                  /if (${Group.Member[${Me.TargetOfTarget.Master.CleanName}].Index}) /goto :IncSpawn
                  /if (${NetBots[${Me.TargetOfTarget.Master.CleanName}].ID}) /goto :IncSpawn
                  /if (${Spawn[raid id ${Me.TargetOfTarget.Master.ID}].ID}) /goto :IncSpawn
               }
            }
         }
         /continue
      }
:IncSpawn
      /varset ADMobFound TRUE
      /if (${scParm.Equal[find]}) /break
      /varcalc ADMobCount ${ADMobCount}+1
      /if (${ADMobCount}>${ADMobMax}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Too many mobs to track
         /return ${tADAdd}
      }
      /if (${ADMobList[${ADMobCount}]}) {
         /if (!${Spawn[${ADMobList[${ADMobCount}]}].ID} || ${Spawn[${ADMobList[${ADMobCount}]}].Type.Equal[Corpse]}) {
            /varset ADMobList[${ADMobCount}] 0
         } else {
            /if (${Spawn[${ADMobList[${ADMobCount}]}].Distance}>${Radius}) /varset ADMobList[${ADMobCount}] 0
         }
      }
      /for a 1 to ${ADMobMax}
         /if (${npcid}==${ADMobList[${a}]}) /break
      /next a
      /if (${a}<=${ADMobMax}) /continue
      /for a 1 to ${ADMobMax}
         /if (!${ADMobList[${a}]}) {
            /varset tADAdd TRUE
            /varcalc AddSpawn ${AddSpawn}+1
            /varset ADMobList[${a}] ${npcid}
            /varset MLDebuffed[${a}] FALSE
            /for b 1 to ${ADCount}
              /varset ADMDL[${a},${b}] 0
            /next b
            /break
         }
      /next a
      /if (${a}>${ADMobMax}) /break
   }
   /if (${scParm.NotEqual[find]}) {
      /if (${ADMobMax}>${ADMobCount}) {
         /for a ${Math.Calc[${ADMobMax}-${If[${ADMobCount},${ADMobCount},1]}]} to ${ADMobMax}
            /if (${ADMobList[${a}]}) {
               /varset MLDebuffed[${a}] FALSE
               /for b 1 to ${ADCount}
                  /varset ADMDL[${a},${b}] 0
               /next b
            }
         /next a
      }
      /if (${DoPull}) {
         /if (!${ADMobCount} && ${CurrentMobCount}==1) {
            /varset APCheckTimer ${APCheckTime}s
            /varset APPullDelay 0
         }
      }
      /if (${CurrentMobCount} && ${CurrentMobCount}>${ADMobCount}) {
         /varcalc CurrentMobCount ${CurrentMobCount}-1
      } else {
         /varset CurrentMobCount ${ADMobCount}
      }
      /varset SpawnCheckTimer 1s
   }
   /varset SpawnFindTimer 1s
/return ${tADAdd}


Sub ADDebuffCheck
   /if (${MBPause} || ${ADCheckTimer}) /return
   /call ResetSpellResists
   /varset ADCheckTimer ${ADCheckTime}s
   /varset ADHoldTimer timer 0

   /declare a int local

   /if (!${Defined[tADTime]}) {
      /declare tADTime int outer
      /declare tADCount int outer
      /declare tADAddDetected bool outer
      /declare tADexistingTar int outer
   }
   /varset tADexistingTar 0

   /call ADSpawnCheck
   /if (!${ADMobCount}) {
      /call ADResetTarType
      /return
   }
   /if (${Melee.Combat}) {
      /if (${OffTank}) {
	     /varset tADexistingTar ${Target.ID}
	  }
      /if (${Me.Class.ShortName.Equal[BRD]}) {
         /if (!${Twist} && ${BardCombatGems.Length}) {
            /if (${Me.State.Equal[sit]}) /stand
            /squelch /twist ${BardCombatGems}
            /if (${InfoMsgLevel}) {
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Msg: Combat gems
            }
         }
      }
   }

:tADTop
   /varset tADTime ${Macro.RunTime}
   /varset tADCount 1
   /varset tADAddDetected FALSE

:tADCountTop
   /for a 1 to ${ADMobMax}
      /if (${ADMobList[${a}]} && ${Spawn[${ADMobList[${a}]}].ID}) {
         /if (${Spawn[${ADMobList[${a}]}].Distance}<=${Radius}) {
            /varset MLDebuffed[${a}] TRUE
            /if (!${ACState} && ${TankName.Length}>2) {
               /if (${Spawn[pc ${TankName}].Distance}<=200) /call ADSetTarget
            }
            /call DebuffMob ${a}
            /if (${tADAddDetected}) {
               /if (${ADMaxPRI}) {
                  /if (${InfoMsgLevel}) {
                     /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Msg: Add detected - starting debuffs from the top
                  }
               }
               /call ADResetTarType
               /goto :tADTop
            }
         }
      }
      /if (!${DoDebuffs} || ${MBPause} || ${NetBots[${Me.CleanName}].Silenced} || ${NetBots[${Me.CleanName}].Charmed} || ${Select[${Me.State},STUN,HOVER,DEAD]}) /goto :tEnd
   /next a
   /if (${ADTarType[${tADCount}]}>9) {
      /varcalc tADCount ${tADCount}+1
      /goto :tADCountTop
   }
:tEnd
   /if (${ADHoldTimer}) /goto :tADTop
   /if (${OffTank}) {
      /if (${tADexistingTar}) /tar ID ${tADexistingTar}
   }
/return


Sub ADSetTarget
   /if (!${tADexistingTar} && ${ACMATarget}) /varset tADexistingTar ${ACMATarget}
   /if (${NetBots[${TankName}].ID}) {
      /varset ACMATarget ${NetBots[${TankName}].TargetID}
   } else {
      /call ACGetAssist ${TankName}
      /varset ACMATarget ${Macro.Return}
      /if (!${ACMATarget} || !${OffTank} && ${tADexistingTar} && ${tADexistingTar}!=${ACMATarget}) {
         /if (${PetAssist}) /varset PetAssist 1
         /if (${SPetAssist}) /varset SPetAssist 1
      }
   }
/return


Sub ADResetTarType
   /declare a int local
   /for a 1 to ${ADCount}
      /if (${ADTarType[${a}]}<100) /continue
      /if (${ADMobCount} && ${Select[${ADGem[${a}]},${ValidSpellGems}]} && ${Me.GemTimer[${ADGem[${a}]}]}>2900) /continue
      /varcalc ADTarType[${a}] ${ADTarType[${a}]}-100
      /if (${ADSpellDelay[${a}]}>=${Macro.RunTime}) /varset ADSpellDelay[${a}] 0
   /next a
/return


Sub DebuffMob(int a)
   /declare b int local
   /declare c int local
   /declare x int local
   /declare tGem int local
   /declare tSpellID int local
   /declare tSpellRange int local
   /declare tSpellDuration int local
   /declare tSpellDelay int local
   /declare tCastTime timer local
   /declare tCT int local
   /declare ADtTargetID int local
   /declare tTarCnt int local
   /declare tRecast1 int local
   /declare tRecast2 int local
   /declare tCastonRes int local
   /declare HoTTTarget bool local
   /declare tSkipToAdds bool local
   /declare tCastReady bool local
   /declare tRecharmDelay timer local
   /declare tPrevTarget int local 

   /for b ${tADCount} to ${ADCount}
      /if (!${DoDebuffs} || ${MBPause}) /return 
      /varset tCastonRes 0
      /if (${ADCharmAD}==${b}) {
         /if (${DoPet}) {
            /if (${Me.Pet.ID}) /continue
            /if (${ADCharmID}) {
               /if (!${Spawn[${ADCharmID}].ID}) {
                  /bc Msg: Charmed pet ${ADCharmID} has died.
                  /bc IncludeID ${ADCharmID}                     
                  /varset ADCharmID 0
                  /goto :tCheckforAdds
               }
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Lost pet ${ADCharmID} - ${Spawn[${ADCharmID}].CleanName}
               /varset ADMDL[${a},${b}] 0
               /if (${ADCharmID}==${ADDropID}) {
                  /varset ADCharmID 0
                  /varset tRecharmDelay 5s 
                  /continue
               }
            }
            /if (${ADDropID}) {
               /if (${ADDropID}==${Spawn[${ADMobList[${a}]}].ID} && ${tRecharmDelay}) /continue
               /varset ${ADDropID} 0
            }             
            /if (${ADCharmType}==1) {
               /if (!${Select[${Spawn[${ADMobList[${a}]}].Body.Name},undead,vampyre]}) /continue
            } else /if (${ADCharmType}==2) {
               /if (!${Select[${Spawn[${ADMobList[${a}]}].Body.Name},animal,insect]}) /continue
            } else /if (${ADCharmType}==3) {
               /if (!${Spawn[${ADMobList[${a}]}].Body.Name.Find[summoned]}) /continue
            } else /if (${ADCharmType}==4) {
               /if (!${ADCharmName.Find[${Spawn[${ADMobList[${a}]}].CleanName}]}) /continue
            }
         }
         /if (${ADMobList[${a}]}==${ADCharmID} && ${Me.Pet.ID}) /return
         /if (!${DoPet}) /continue
      }
      /if (${ADMDL[${a},${b}]}>=${tADTime} || ${ADTarType[${b}]}>99) /goto :tWhatNext
      /if (!${ADTarCnt[${b}]}) /continue
      /if (${Spawn[${ADMobList[${a}]}].Type.Equal[Corpse]} || !${Spawn[${ADMobList[${a}]}].ID}) { 
         /if (${Target.ID}==${Spawn[${ADMobList[${a}]}].ID} && ${Me.Casting.ID} && ${Spell[${Me.Casting.ID}].SpellType.Equal[detrimental]}) /stopcast
         /return
      }
      /if (${b}>${ADMaxNMA} && ${ADMobList[${a}]}!=${ACMATarget}) /return
      /if (${ADTarType[${b}]}==12) {
         /if (${ADMobList[${a}]}==${ACMATarget}) /return
         /if (${ADHold.Arg[1,|]}) {
            /if (!${ADHoldTimer} && ${b}==${ADHold.Arg[2,|]}) {
               /if (${Spawn[${ADMobList[${a}]}].ID} && ${ADMDL[${a},${b}]}<=${Macro.RunTime}+${ADHold.Arg[3,|]} && ${ADMDL[${a},${b}]}>=${Macro.RunTime}) /varset ADHoldTimer ${Math.Calc[${ADHold.Arg[3,|]}+.5]}s
            }
         }
      } else /if (${ADTarType[${b}]}==1) {
         /if (${ADMobList[${a}]}!=${ACMATarget}) /continue
      } else /if (${ADTarType[${b}]}==3) {
         /if (${ADMobList[${a}]}!=${ACMATarget} || !${DoNamed}) /continue
         /if (!${Spawn[${ACMATarget}].Named} && (!${NamedID} || (${NamedID} && ${Spawn[${ACMATarget}].ID}!=${NamedID}))) /continue
      } else /if (${ADTarType[${b}]}==11) {
         /if (${ADMobList[${a}]}!=${ACMATarget}) /return
      } else /if (${ADTarType[${b}]}==13) {
         /if (${ADMobList[${a}]}!=${ACMATarget} || !${DoNamed}) /return
         /if (!${Spawn[${ACMATarget}].Named} && (!${NamedID} || (${NamedID} && ${Spawn[${ACMATarget}].ID}!=${NamedID}))) /return
      } else /if (${ADTarType[${b}]}==2) {
         /if (${ADMobList[${a}]}==${ACMATarget}) /continue
      }
      /if (${ADTarCnt[${b}]}>1 && ${ADMobCount}<${ADTarCnt[${b}]}) /continue
      /if (${ADMDL[${a},${b}]}>=${tADTime}) /continue
      /if (${Spawn[${ADMobList[${a}]}].PctHPs}>${ADTarBegHP[${b}]} || ${Spawn[${ADMobList[${a}]}].PctHPs}<${ADTarEndHP[${b}]}) /continue
      /if (${NetBots[${TankName}].TargetID}==${ADMobList[${a}]} && ${NetBots[${TankName}].TargetHP}<${ADTarEndHP[${b}]}) /continue
      /if (${ADMobList[${a}]}==${Me.Pet.ID}) /continue
      /if (${ADUseHoTT[${b}]}==101) {
         /if (!${Spawn[id ${ADMobList[${a}]} xtarhater].ID}) /continue
      }
      /if (${ADCondition[${b}].Length}>4) {
	     /varset PSTargetID ${ADMobList[${a}]}
         /call IniCommand "${ADCondition[${b}]}"
         /if (${Macro.Return.NotEqual[TRUE]}) /goto :tWhatNext
      }
      /varset ADMDL[${a},${b}] ${tADTime}
      /varset tGem 0
      /varset tSpellDuration 0
      /if (${Select[${ADGem[${b}]},${ValidSpellGems}]}) {
|Spell
         /if (${Me.CurrentMana}+20<${Me.Book[${Me.Book[${ADSpell[${b}]}]}].Mana} || ${Me.PctMana}<${ADSpellMinMana[${b}]}) /goto :tWhatNext
         /varset tSpellID ${Me.Book[${Me.Book[${ADSpell[${b}]}]}].ID}
         /varset tGem ${ADGem[${b}]} 
         /varcalc tCT ${Spell[${tSpellID}].MyCastTime}/100+50
         /if (${Spell[${tSpellID}].Trigger.ID}) {
            /if (${Spell[${tSpellID}].Trigger.Duration}) {
               /if (${Spell[${tSpellID}].Trigger.SpellType.Find[Beneficial]} && ${Me.Buff[${Spell[${tSpellID}].Trigger.BaseName}].ID}) /goto :tWhatNext
               /varset tSpellDuration ${Spell[${tSpellID}].Trigger.Duration.TotalSeconds}
            }
         } else /if (${Spell[${tSpellID}].Trigger[1].ID}) {
            /if (${Spell[${tSpellID}].Trigger[1].Duration}) {
               /if (${Spell[${tSpellID}].Trigger[1].SpellType.Find[Beneficial]} && ${Me.Buff[${Spell[${tSpellID}].Trigger[1].BaseName}].ID}) /goto :tWhatNext
               /varset tSpellDuration ${Spell[${tSpellID}].Trigger[1].Duration.TotalSeconds}
            } 
		 } else /if (${Spell[${tSpellID}].Duration}) {
            /if (${Spell[${tSpellID}].SpellType.Find[Beneficial]} && ${Me.Buff[${Spell[${tSpellID}].BaseName}].ID}) /goto :tWhatNext
            /varset tSpellDuration ${Spell[${tSpellID}].Duration.TotalSeconds}
         }
         /if (!${Me.Gem[${tGem}].Name.Length} || ${Me.Gem[${tGem}].Name.NotEqual[${ADSpell[${b}]}]}) {
            /call MemGem ${tGem} "${ADSpell[${b}]}"
			/if (${Macro.Return.NotEqual[TRUE]}) {
               /goto :tWhatNext
            }
            /varcalc tCT ${Spell[${tSpellID}].RecastTime}/100+${tCT}
         }
|        /if (${Me.Class.ShortName.Equal[BRD]} && ${Select[${ADTarType[${b}]},1,11]}) {
|           /if (${BardCombatGems.Find[${tGem}]}) /goto :tWhatNext
|           /varset BardCombatGems ${BardCombatGems} ${tGem}
|           /goto :tWhatNext
|        }
      } else /if (${ADGem[${b}].Find[item]}) {
|Item
         /if (!${FindItem[=${ADSpell[${b}]}].ID} || ${FindItem[=${ADSpell[${b}]}].Timer}) /goto :tWhatNext
         /varset tSpellID ${FindItem[=${ADSpell[${b}]}].Spell.ID}
         /varset tGem 20 
         /varcalc tCT ${FindItem[=${ADSpell[${b}]}].CastTime}/100+50
         /if (${Spell[${tSpellID}].Duration}) /varset tSpellDuration ${Spell[${tSpellID}].Duration.TotalSeconds}
         /if (${Me.Class.ShortName.Equal[BRD]}) {
            /if (!${ADBardSlot[${b}]}) {
               /call GetBardSlot "${ADSpell[${b}]}" AD|${b}
               /if (!${ADBardSlot[${b}]}) {
                  /bc Msg: No BardSlot assigned. Turning off Item casting for: ${ADSpell[${b}]}
                  /varset ADTarCnt[${b}] 0
                  /goto :tWhatNext
               }
            }
         }
      } else /if (${ADGem[${b}].Equal[alt]}) {
|Alt
         /if (${Me.Shrouded}) { 
            /delay 3s ${Me.AltAbilityReady[${ADSpell[${b}]}]}
         }
         /if (!${Me.AltAbilityReady[${ADSpell[${b}]}]}) /continue
         /if (${Me.AltAbility[${ADSpell[${b}]}].Spell.Trigger.ID}) {
            /if (${Me.CurrentMana}<${Me.AltAbility[${ADSpell[${b}]}].Spell.Trigger.Mana}) /continue
            /if (${Me.AltAbility[${ADSpell[${b}]}].Spell.Trigger.Duration}) {
               /if (${Me.AltAbility[${ADSpell[${b}]}].Spell.Trigger.SpellType.Find[Beneficial]} && ${Me.Buff[${Me.AltAbility[${ADSpell[${b}]}].Spell.Trigger.BaseName}].ID}) /goto :tWhatNext
               /varset tSpellDuration ${Me.AltAbility[${ADSpell[${b}]}].Spell.Trigger.Duration.TotalSeconds}
            }
		 } else /if (${Me.AltAbility[${ADSpell[${b}]}].Spell.Trigger[1].ID}) {
            /if (${Me.CurrentMana}<${Me.AltAbility[${ADSpell[${b}]}].Spell.Trigger[1].Mana}) /continue
            /if (${Me.AltAbility[${ADSpell[${b}]}].Spell.Trigger[1].Duration}) {
               /if (${Me.AltAbility[${ADSpell[${b}]}].Spell.Trigger[1].SpellType.Find[Beneficial]} && ${Me.Buff[${Me.AltAbility[${ADSpell[${b}]}].Spell.Trigger[1].BaseName}].ID}) /goto :tWhatNext
               /varset tSpellDuration ${Me.AltAbility[${ADSpell[${b}]}].Spell.Trigger[1].Duration.TotalSeconds}
            }
         } else {
            /if (${Me.CurrentMana}<${Me.AltAbility[${ADSpell[${b}]}].Spell.Mana}) /continue
            /if (${Me.AltAbility[${ADSpell[${b}]}].Spell.Duration}) {
               /if (${Me.AltAbility[${ADSpell[${b}]}].Spell.SpellType.Find[Beneficial]} && ${Me.Buff[${Me.AltAbility[${ADSpell[${b}]}].Spell.BaseName}].ID}) /goto :tWhatNext
               /varset tSpellDuration ${Me.AltAbility[${ADSpell[${b}]}].Spell.Duration.TotalSeconds}
            }
         }
         /varset tSpellID ${Me.AltAbility[${ADSpell[${b}]}].Spell.ID}
         /varset tGem 30 
         /varcalc tCT ${Me.AltAbility[${ADSpell[${b}]}].Spell.CastTime}/100+40
      } else /if (${ADGem[${b}].Equal[script]}) {
|Script
         /varset PSTargetID ${ADMobList[${a}]}
         /call MBScript ${ADSpell[${b}]}
         /varset tSpellDelay 0
         /varset tGem 40
         /varset ADtTargetID ${Target.ID}
         /varset tSpellID 0
         /goto :tContDebuff
      } else /if (${ADGem[${b}].Equal[ability]}) {
|Ability
         /if (!${Me.AbilityReady[${ADSpell[${b}]}]}) /continue
         /if (${Target.ID}!=${ADMobList[${a}]}) /squelch /tar id ${ADMobList[${a}]}
         /delay 1s ${Target.ID}==${ADMobList[${a}]}
         /if (${Target.ID}!=${ADMobList[${a}]}) /goto :tWhatNext
         /call ResetSpellResists
         /doability ${ADSpell[${b}]}
         /varset tSpellDelay 0
         /varset tGem 50
         /varset ADtTargetID ${Target.ID}
         /varset tSpellID 0
         /goto :tContDebuff
      } else /if (${ADGem[${b}].Equal[disc]}) {
|Disc
         /if (!${Me.CombatAbilityReady[${ADSpell[${b}]}]}) /continue
         /if (${Me.PctEndurance}<${ADSpellMinMana[${b}]} || ${ADDiscTimer[${b}]}>${Macro.RunTime}) /continue
         /varset ADDiscTimer[${b}] ${Macro.RunTime}+${ADSpellDelay[${b}]}
         /if (${Target.ID}!=${ADMobList[${a}]}) /squelch /tar id ${ADMobList[${a}]}
         /delay 1s ${Target.ID}==${ADMobList[${a}]}
         /if (${Target.ID}!=${ADMobList[${a}]}) /goto :tWhatNext
         /call ResetSpellResists
         /if (${Me.Class.ShortName.Equal[BRD]}) /stopsong
         /disc ${Spell[${ADSpell[${b}]}].ID}
         /varset tSpellDelay 0
         /varset tGem 50
         /varset ADtTargetID ${Target.ID}
         /varset tSpellID 0
         /goto :tContDebuff
      }
      /if (!${tGem} || ${Spawn[${ADMobList[${a}]}].Type.Equal[corpse]} || !${Spawn[${ADMobList[${a}]}].ID}) /goto :tWhatNext 
      /if (${Spell[${tSpellID}].Range} && !${Select[${Spell[${tSpellID}].TargetType},PB AE,self]}) {
         /if (${Spawn[${ADMobList[${a}]}].Distance}>${Spell[${tSpellID}].MyRange}) /goto :tWhatNext
      }
      /if (${Defined[MaxLevel${Spell[${ADSpell[${b}]}].ID}]}) {
         /if (${Spawn[${ADMobList[${a}]}].Level}>${MaxLevel${Spell[${ADSpell[${b}]}].ID}}) /goto :tWhatNext
      }
      /if (${tSpellID} && ${Defined[Imm${tSpellID}]}) {
         /if (${Imm${tSpellID}.Find[|${Spawn[${ADMobList[${a}]}].CleanName}|]} && !${Imm${tSpellID}.Find[|${Spawn[${ADMobList[${a}]}].CleanName}*|]}) {
            /if (${ADIfSpellImmune[${b}].Length}>3) {
			   /varset PSTargetID ${ADMobList[${a}]}
               /call IniCommand "${ADIfSpellImmune[${b}]}" ${ADMobList[${a}]}
            }
            /goto :tWhatNext
         }
      }
| AE - No range/PB AE, see if mob too far away. If range, make sure mob isn't within AE Range so as not to cast on yourself, etc.
|    - Adding an additional check to ignore triggered spells on the too close check.  For something like Shaman Turgur's Virulent Swarm AA.
|      The AA is Targeted AE. But it is a Trigger of a Single Target spell and only hits NPC's so it doesn't matter how close we are.
      /if (${Spell[${tSpellID}].AERange}) {
         /if (${Spell[${tSpellID}].TargetType.Equal[PB AE]}) {
            /if (${Spawn[${ADMobList[${a}]}].Distance}>${Spell[${tSpellID}].AERange}) /goto :tWhatNext
         } else /if (${Spell[${tSpellID}].TargetType.NotEqual[single]}) {
            /if (!${Spell[${tSpellID}].SpellType.Find[Beneficial]} && !${Spell[${tSpellID}].CastOnYou.Find[drowsy]}) {
               /if (!${Spell[${tSpellID}].Range}) {
                  /if (${Spawn[${ADMobList[${a}]}].Distance}>${Spell[${tSpellID}].AERange}) /goto :tWhatNext
               } else /if (!${Spell[${tSpellID}].Trigger.ID} && !${Spell[${tSpellID}].Trigger[1].ID} && !${Spell[${tSpellID}].Name.Find[Explosion of Hatred]}) {
                  /if (${Spawn[${ADMobList[${a}]}].Distance}<${Spell[${tSpellID}].AERange}) /goto :tWhatNext
               }
            }
         }
      }
      /if (${Select[${ADUseHoTT[${b}]},0,101]}) {
         /varset HoTTTarget TRUE
      } else { 
         /if (${Target.ID}!=${ADMobList[${a}]}) {
           /squelch /tar id ${ADMobList[${a}]}
           /delay 3s ${Target.ID}==${ADMobList[${a}]}
| Have to wait for ToT to update on new target.
           /delay 1s !${Me.TargetOfTarget.ID}
         }
         /delay 2s ${Me.TargetOfTarget.ID} 
         /if (${Target.ID} && (${Group.Member[${Me.TargetOfTarget.Name}].Index} || ${Spawn[raid id ${Me.TargetOfTarget.ID}].ID} || ${Me.TargetOfTarget.ID}==${Me.ID} || ${NetBots[${Me.TargetOfTarget.Name}].ID})) /varset HoTTTarget TRUE
         /if (!${Me.TargetOfTarget.ID}) /varset HoTTTarget FALSE
         /if (!${HoTTTarget}) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Skipping mob - ${Spawn[${ADMobList[${a}]}].CleanName}
            /varset tGem 60
            /varset tSpellDelay ${ADUseHoTT[${b}]}
            /goto :tContDebuff
         }
      }
      /if (${tGem}<20) {
         /if (${Me.Class.ShortName.NotEqual[BRD]}) {
            /if (${ADTarType[${b}]}<10 && ${Me.GemTimer[${ADGem[${b}]}]}>2000) /continue
         } else {
            /if (!${Cast.Ready[${ADGem[${b}]}]}) /continue
         } 
      }
      /if (${ADCharmAD} && ${DoPet}) {
         /if (!${Me.Pet.ID} && ${ADCharmID} && ${tSpellID}!=${Spell[${ADCharmSpell}].ID}) {
            /stopcast
            /if (${Me.Class.ShortName.Equal[BRD]}) /squelch /stoptwist
            /goto :tCheckforAdds
         }
      }

      /varset ADtTargetID 0
      /varset tRecast1 0
      /varset tRecast2 0
      /varset tSpellDelay 0
      /varset tCastTime ${tCT}
      /varset CastResult UNKNOWN
      /varset tSkipToAdds FALSE
      /varset interruptFlag FALSE

      /while (1) {
         /if (${DoHeals} && ${AHCount}) /call AHHealCheck
         /if (${PetAssist} || ${SPetAssist}) {
            /doevents PetNoWake
            /if (${ACMATarget} && ${DoMelee} && ${Spawn[${ACMATarget}].PctHPs}<=${ACAssistPct}) {
               /if (${PetAssist} && ${Me.Pet.ID} && ${PetAssist}!=${ACMATarget}) {
                  /pet attack ${ACMATarget}
                  /varset PetAssist ${ACMATarget}
               }
               /if (${SPetAssist} && ${Target.ID}==${ACMATarget} && ${SPetAssist}!=${ACMATarget}) {
                  /pet swarm
                  /varset SPetAssist ${ACMATarget}
               }
            }
         }
         /doevents eqbc
         /if (${Spawn[${ADMobList[${a}]}].Type.Equal[corpse]} || !${Spawn[${ADMobList[${a}]}].ID}) /return
         /if (!${ADtTargetID}) {
            /if (${NetBots[${Me.CleanName}].Silenced} || ${NetBots[${Me.CleanName}].Charmed} || ${Select[${Me.State},STUN,HOVER,DEAD]}) /return
            /if (${Me.Class.ShortName.Equal[BRD]}) {
               /if (${Target.ID}!=${ADMobList[${a}]}) {
                  /if (!${Spell[${tSpellID}].SpellType.Find[Beneficial]}) {
                     /varset tPrevTarget ${Target.ID}
                     /squelch /stoptwist
                     /if (${Melee.Combat}) /attack off
                     /squelch /tar id ${ADMobList[${a}]}
                     /delay 1s ${Target.ID}==${ADMobList[${a}]}
                  } else {
                     /varset tPrevTarget 0
                  }
               } else {
                  /varset tPrevTarget 0
               }
               /if (${Me.State.Equal[sit]}) /stand
               /varset ADtTargetID ${ADMobList[${a}]}
               /if (${tGem}==30) {
                  /alt act ${Me.AltAbility[${ADSpell[${b}]}].ID}
                  /delay 2
                  /if (${tPrevTarget}) /target id ${tPrevTarget}
                  /break
               } else /if (${tGem}==20) {
                  /stopsong
                  /squelch /twist once ${ADBardSlot[${b}]}
                  /delay 3
                  /if (${tPrevTarget}) /target id ${tPrevTarget}
                  /delay ${Math.Calc[${tCT}-50]}
                  /break
               } else {
                  /call ResetSpellResists
                  /varset CastResult PENDING
                  /stopsong
                  /squelch /twist once ${ADGem[${b}]}
                  /delay 5
                  /if (${tPrevTarget}) /target id ${tPrevTarget}
                  /varcalc tCastTime ${tCT}-50
               }
            } else {
               /if (${tGem}<20) {
                  /if (${Cast.Status.Equal[i]}) {
                     /varset tCastReady TRUE
                  } else {
                     /delay 2s ${Cast.Status.Equal[i]}
                     /varset tCastReady ${If[${Cast.Status.Equal[i]},TRUE,FALSE]}
                  }
               } else {
                  /if (${Cast.Ready[${ADSpell[${b}]}|${ADGem[${b}]}]}) {
                     /varset tCastReady TRUE
                  } else {
                     /delay 2s ${Cast.Ready[${ADSpell[${b}]}|${ADGem[${b}]}]}
                     /varset tCastReady ${If[${Cast.Ready[${ADSpell[${b}]}|${ADGem[${b}]}]},TRUE,FALSE]}
                  }
               }
               /if (${tCastReady}) {
                  /if (${Target.ID}!=${ADMobList[${a}]}) {
                     /if (${Melee.Combat}) /attack off
                     /squelch /tar id ${ADMobList[${a}]}
                     /delay 1s ${Target.ID}==${ADMobList[${a}]}
                  }
                  /call ResetSpellResists
                  /varset ADtTargetID ${ADMobList[${a}]}
                  /varset tCastTime ${tCT}
                  /varset MBCurrentSpell AD|${b}|${ADtTargetID}${If[${ADTarType[${b}]}>9 || (${tGem}==30 && ${tCT}<51),|NHI,]}
                  /call MQ2Cast "${ADSpell[${b}]}" ${If[${Select[${ADGem[${b}]},${ValidSpellGems}]},gem,]}${ADGem[${b}]} ${tCastTime} CastCheck -targetid|${ADtTargetID} ${If[${ADSpellFoci[${b}].Length},-bandolier|${ADSpellFoci[${b}]},]} 
               }
            }
         }
         /if (${ADtTargetID}) {
            /if (${interruptFlag}) {
               /varset SpawnCheckTimer 0
               /varset tSkipToAdds TRUE
               /break
            }
            /if (${Spawn[${ADtTargetID}].Type.Equal[Corpse]} || !${Spawn[${ADtTargetID}].ID}) {
               /stopcast
               /return
            }
            /if (${Select[${ADTarType[${b}]},2,12]} && !${Spell[${tSpellID}].TargetType.Find[ ae]}) {
               /if (!${Me.Class.ShortName.Equal[brd]} && ${NetBots[${TankName}].ID} && ${Target.ID}==${NetBots[${TankName}].TargetID}) {
                  /stopcast
                  /return
               }
            }
            /if (${ADCharmAD} && ${DoPet}) { 
               /if (!${Me.Pet.ID} && ${ADCharmID} && ${tSpellID}!=${Spell[${ADCharmSpell}].ID}) {
                  /if (${Me.Class.ShortName.Equal[BRD]}) /squelch /stoptwist
                  /stopcast
                  /varset tSkipToAdds TRUE
                  /break
               }
            }
            /if (${Me.Class.ShortName.Equal[BRD]}) {
               /if (!${CastResult.Equal[PENDING]}) {
                  /varset ADEventSpell ${Me.Gem[${tGem}]}
                  /call CheckSpellResists
                  /if (${ADResistMatch.Length}) {
                     /if (${ADResistMatch.Equal[STUNNED]}) {
                        /varset CastResult ${ADResistMatch}
                     } else /if (${ADResistMatch.Find[RESIST]}) {
                        /if (${ADResistMatch.Find[IMMUNE]}) {
                           /varset CastResult IMMUNE
                        } else {
                           /varset CastResult RESIST
                        }
                     }
                  }
                  /if (${CastResult.Equal[SUCCESS]}) /break
                  /if (${CastResult.Equal[IMMUNE]}) {
                     /bc Msg: ${Spawn[${ADMobList[${a}]}].CleanName} [+r+]IMMUNE[+x+] to ${ADSpell[${b}]}
                     /docommand /${BeepCommand}
                     /if (${ADIfSpellImmune[${b}].Length}>3) {
					    /varset PSTargetID ${ADMobList[${a}]}
					    /call IniCommand "${ADIfSpellImmune[${b}]}" ${ADMobList[${a}]}
					 }
                     /if (${tSpellID}) {
					    /if (!${Defined[Imm${tSpellID}]}) /declare Imm${tSpellID} string outer
                        /if (!${Imm${tSpellID}.Find[|${Spawn[${ADtTargetID}].CleanName}|]} && !${Imm${tSpellID}.Find[|${Spawn[${ADtTargetID}].CleanName}*|]}) {
                           /bc Msg: Adding ${Spawn[${ADtTargetID}].CleanName} to the immune list.
                           /varset Imm${tSpellID} ${Imm${tSpellID}}|${Spawn[${ADtTargetID}].CleanName}|
                           /ini "${MBCommon}" "${Spell[${tSpellID}].Name}" "${Zone.ShortName}Immune" "${Imm${tSpellID}}"
                        }
                     }
                     /varset tSpellDelay 4000
                     /break
                  }
                  /if (${Select[${ADTarType[${b}]},2,12]} && ${Select[${CastResult},RESIST,STUNNED]} && ${ADtTargetID}==${ACMATarget}) {
                     /varset tSkipToAdds TRUE
                     /break
                  }
                  /if (${Select[${CastResult},RESIST,STUNNED]} && ${ADSpellRecast2[${b}]}>${tRecast2}) {
                     /varcalc tRecast2 ${tRecast2}+1
                     /if (${InfoMsgLevel}) {
                        /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Recasting ${ADSpell[${b}]} - ${tRecast2}
                     }
                     /varset ADtTargetID 0
                     /continue
                  }
                  /varset tSkipToAdds TRUE
                  /break               
               }
               /delay 5
               /if (${DoMelee} && ${ACState} && ${ACMATarget} && (${Target.ID}!=${ACMATarget} || ${tPrevTarget}==${ACMATarget})) {
                  /if (${Target.ID}) /squelch /target clear
                  /tar id ${ACMATarget}
                  /delay 5
                  /squelch /stick off
                  /killthis
               }
               /if (${Me.Casting.ID}!=${Spell[${Me.Gem[${tGem}]}].ID}) {
                  /varset CastResult SUCCESS
                  /delay 1s
               }
            } else /if (${Cast.Status.Equal[i]}) {
               /if (${Select[${Cast.Result},CAST_IMMUNE,CAST_RESIST,CAST_TAKEHOLD,CAST_COMPONENTS]}) {
                  /varset ADEventSpell ${Cast.Stored.Name}${If[${Cast.Stored.Trigger.ID},|${Cast.Stored.Trigger.Name},${If[${Cast.Stored.Trigger[1].ID},|${Cast.Stored.Trigger[1].Name},]}]}
                  /call CheckSpellResists
               } else {
                  /varset ADResistMatch
               }
               /if (${Cast.Result.Equal[CAST_RECOVER]}) {
                  /varset tCastTime ${tCT}+30
                  /varset ADtTargetID 0
                  /delay 5
                  /continue
               }
               /if (${Spell[${tSpellID}].TargetType.Find[ ae]}) {
                  /if (${Cast.Result.Equal[CAST_SUCCESS]} || (${Select[${Cast.Result},CAST_IMMUNE,CAST_RESIST,CAST_TAKEHOLD]} && (!${ADResistMatch.Length} || ${ADResistMatch.Equal[TAKEHOLD]} || (${ADResistMatch.Find[RESIST]} && !${ADSpellRecast2[${b}]})))) {
                     /varcalc ADTarType[${b}] ${ADTarType[${b}]}+100
                     /if (${tSpellDuration}>12 && !${ADSpellDelay[${b}]} && ${ADTarCnt[${b}]}>1) /varcalc ADSpellDelay[${b}] ${Macro.RunTime}+${Math.Calc[${tSpellDuration}*10]}
                     /break
                  }
               }
               /if (${Cast.Result.Equal[CAST_SUCCESS]}) /break
| If the resist wasn't from the actual spell we cast, consider it a success. We checked the meessages above.
               /if (${Select[${Cast.Result},CAST_IMMUNE,CAST_RESIST,CAST_TAKEHOLD,CAST_COMPONENTS]} && !${ADResistMatch.Length}) /break
               /if (${Me.Stunned}) { 
                  /delay 3s !${Me.Stunned}
               }
               /if (${Select[${Cast.Result},CAST_FIZZLE,CAST_INTERRUPTED,CAST_STUNNED]} || ${Select[${ADResistMatch},FIZZLES,INTERRUPTED,STUNNED]}) {
                  /if (${ADSpellRecast1[${b}]}>${tRecast1}) {
                     /if (${ADMaxPRI} && ${b}>1) {
                        /varset SpawnCheckTimer 0
                        /call ADSpawnCheck
                        /if (!${ADMobCount}) /return
                        /if (${AddSpawn}) { 
                           /varset tADAddDetected TRUE
                           /return
                        }
                     }
                     /delay 2s ${Cast.Status.Equal[i]}
                     /varcalc tRecast1 ${tRecast1}+1
                     /if (${InfoMsgLevel}) {
                        /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Retrying ${ADSpell[${b}]} - ${tRecast1}
                     }
                     /varset tCastTime ${tCT}+30
                     /varset ADtTargetID 0
                     /continue
                  }
                  /varset tSkipToAdds TRUE
                  /break
               }
               /if (${Cast.Result.Equal[CAST_IMMUNE]} && ${ADResistMatch.Find[IMMUNE]} && ${ADResistMatch.Find[RESIST]}) {
                  /bc Msg: ${Spawn[${ADMobList[${a}]}].CleanName} [+r+]IMMUNE[+x+] to ${ADSpell[${b}]}
                  /docommand /${BeepCommand}
                  /if (${ADIfSpellImmune[${b}].Length}>3) {
				     /varset PSTargetID ${ADMobList[${a}]}
				     /call IniCommand "${ADIfSpellImmune[${b}]}" ${ADMobList[${a}]}
				  }
                  /if (${tSpellID}) {
				     /if (!${Defined[Imm${tSpellID}]}) /declare Imm${tSpellID} string outer
                     /if (!${Imm${tSpellID}.Find[|${Spawn[${ADtTargetID}].CleanName}|]} && !${Imm${tSpellID}.Find[|${Spawn[${ADtTargetID}].CleanName}*|]}) {
                        /bc Msg: Adding ${Spawn[${ADtTargetID}].CleanName} to the immune list.
                        /varset Imm${tSpellID} ${Imm${tSpellID}}|${Spawn[${ADtTargetID}].CleanName}|
                        /ini "${MBCommon}" "${Spell[${tSpellID}].Name}" "${Zone.ShortName}Immune" "${Imm${tSpellID}}"
                     }
                  }
                  /varset tSpellDelay 4000
                  /break
               }
               /if (${Cast.Result.Equal[CAST_TAKEHOLD]} && ${ADResistMatch.Equal[TAKEHOLD]}) {
                  /if (${InfoMsgLevel}) { 
                     /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${ADSpell[${b}]} did not take hold
                  }
                  /varset tSpellDelay 30
                  /break
               }
               /if (${Select[${Cast.Result},CAST_RESIST,CAST_IMMUNE]} && ${ADResistMatch.Find[RESIST]}) {
                  /if (!${ADSpellRecast2[${b}]}) {
                     /varset tSkipToAdds TRUE
                     /break
                  }
                  /if (${ADSpellRecast2[${b}]}>${tRecast2}) {
                     /if (${ADMaxPRI} && ${b}>1) {
                        /varset SpawnCheckTimer 0
                        /call ADSpawnCheck
                        /if (!${ADMobCount}) /return
                        /if (${AddSpawn}) { 
                           /varset tADAddDetected TRUE
                           /return
                        }
                     }
                     /if (${ADSpellCastonResist[${b}].Length} && ${tCastonRes}<1) {
                        /call CastCall ${Me.CleanName} "coresist ${ADSpellCastonResist[${b}]} ${ADMobList[${a}]}"
                        /if (${Macro.Return.Arg[2].Equal[ad]} && ${Macro.Return.Arg[5]}==${ADMobList[${a}]}) {
                           /if (${Select[${Macro.Return.Arg[1]},CAST_IMMUNE,CAST_RESIST]}) {
							  /varset ADEventSpell ${Cast.Stored.Name}${If[${Cast.Stored.Trigger.ID},|${Cast.Stored.Trigger.Name},${If[${Cast.Stored.Trigger[1].ID},|${Cast.Stored.Trigger[1].Name},]}]}
                              /call CheckSpellResists
                           } else {
                              /varset ADResistMatch
                           }
                           /if (${Macro.Return.Arg[1].Equal[cast_success]} || (${Select[${Macro.Return.Arg[1]},CAST_IMMUNE,CAST_RESIST,CAST_TAKEHOLD]} && !${ADResistMatch.Length})) {
                              /if (${Spell[${Macro.Return.Arg[4]}].Duration}) {
                                 /varset ADMDL[${a},${Macro.Return.Arg[3]}] ${Math.Calc[${Macro.RunTime}+${Spell[${Macro.Return.Arg[4]}].Duration.TotalSeconds}-5]}
                              } else {
                                 /varset ADMDL[${a},${Macro.Return.Arg[3]}] ${Macro.RunTime}
                              }
                              /if (${ADSameSpell[${Macro.Return.Arg[3]}]}) /varset ADMDL[${a},${ADSameSpell[${Macro.Return.Arg[3]}]}] ${ADMDL[${a},${Macro.Return.Arg[3]}]}
                           }
                        }
                        /varcalc tCastonRes ${tCastonRes}+1
                     }
                     /delay 2s ${Cast.Status.Equal[i]}
                     /varcalc tRecast2 ${tRecast2}+1
                     /if (${InfoMsgLevel}) {
                        /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Recasting ${ADSpell[${b}]} - ${tRecast2}
                     }
                     /varset tCastTime ${tCT}+30
                     /varset ADtTargetID 0
                     /continue
                  }
                  /varset tSpellDelay 30
                  /break
               }
               /if (${Cast.Result.Equal[CAST_COMPONENTS]} && ${ADResistMatch.Find[COMPONENTS]}) {
                  /bc Msg: Missing required components. Turning off debuff [ ${ADSpell[${b}]} ]
                  /docommand /${BeepCommand}
                  /varset ADTarCnt[${b}] 0
                  /break
               }
               /if (${Cast.Result.Equal[CAST_OUTDOORS]}) {
                  /bc Msg: Outdoor only spell. Turning off debuff [ ${ADSpell[${b}]} ]
                  /varset ADTarCnt[${b}] 0
                  /break
               }
| Everything else - CAST_OUTOFRANGE,CAST_CANNOTSEE,CAST_DISTRACTED,CAST_NOTARGET,CAST_CANCELLED,CAST_NOTREADY,CAST_OUTOFMANA (and anything else we missed)
               /if (${MBDebug}) {
                  /echo DEBUFF_other - ${Time} - ${ADSpell[${b}]} - ${MBCurrentSpell} - ${interruptFlag} - ${Cast.Status} - ${Cast.Result} - ${Cast.Stored} - ${NetBots[${TankName}].TargetID} - ${ADtTargetID} - ${tSpellID} - ${tCT} - ${tCastTime} - ${Spell[${tSpellID}].TargetType.Find[ ae]}
               }
               /varset tSkipToAdds TRUE
               /break
            }
            /if (${MBDebug}) {
               /echo DEBUFF_not_i - ${Time} - ${ADSpell[${b}]} - ${MBCurrentSpell} - ${interruptFlag} - ${Cast.Status} - ${Cast.Result} - ${Cast.Stored} - ${NetBots[${TankName}].TargetID} - ${ADtTargetID} - ${tSpellID} - ${tCT} - ${tCastTime} - ${Spell[${tSpellID}].TargetType.Find[ ae]}
            }
         }
         /if (${tCastTime} || ${Cast.Timing}) /continue
         /if (${MBDebug}) {
            /echo DEBUFF_too_long - ${Time} - ${ADSpell[${b}]} - ${MBCurrentSpell} - ${interruptFlag} - ${Cast.Status} - ${Cast.Result} - ${Cast.Stored} - ${NetBots[${TankName}].TargetID} - ${ADtTargetID} - ${tSpellID} - ${tCT} - ${tCastTime} - ${Spell[${tSpellID}].TargetType.Find[ ae]}
         }
         /if (${AHHealList.Find[${Cast.Stored}]}) {
            /if (${Cast.Status.Equal[I]} && ${ADtTargetID} && ${Spawn[${ADtTargetID}].ID} && ${Spawn[${ADtTargetID}].Type.NotEqual[Corpse]}) { 
               /varset ADtTargetID 0
               /continue
            }
         }
         /varset tSkipToAdds TRUE
         /break
      }

      /if (${tSkipToAdds}) /goto :tCheckforAdds
      /if (!${ADTarCnt[${b}]}) /continue

:tContDebuff
      /varset MiscCheckTimer 5s
      /if (${ADAnnounce[${b}].Length}) {
         /if (${tGem}>30 || (${HoTTTarget} && (!${Select[${Cast.Result},cast_immune,cast_takehold]} && !${CastResult.Equal[immune]} || ${ADTarCnt[${b}]}<=1))) {
            /call ParseAnnounce Debuffed: "${ADAnnounce[${b}]}" "${Spawn[${ADtTargetID}].CleanName}" "${ADSpell[${b}]}"
         }
      }
      /if (${ADCDCount} && !${Me.Casting.ID} && ${Me.SpellInCooldown}) {
         /for x 1 to ${ADCDCount}
            /if (${Cast.Ready[${ADCDSpell[${x}]}]}) {
               /casting "${ADCDSpell[${x}]}" -targetID|${ADtTargetID}
               /break
            }
         /next x
      }
      /if (${tSpellDuration} && ${tGem}<40) {
         /if (${ADDurMod[${b}]}) {
            /varcalc tSpellDuration ((${ADDurMod[${b}]}/100)*${tSpellDuration})+${tSpellDuration}-${Spell[${tSpellID}].MyCastTime.TotalSeconds}-5
         } else {
            /varcalc tSpellDuration ${tSpellDuration}-${Spell[${tSpellID}].MyCastTime.TotalSeconds}-5
         }
         /varcalc tSpellDelay ${tSpellDelay}+${ADSpellDelay[${b}]}+${tSpellDuration}+${Macro.RunTime}
      } else {
         /varcalc tSpellDelay ${tSpellDelay}+${ADSpellDelay[${b}]}+${Macro.RunTime}-${Spell[${tSpellID}].MyCastTime.TotalSeconds}-5
      }
      /if (${ADSpellDelay[${b}]}) {
         /if (!${tSpellDelay} || ${tSpellDelay}<${ADSpellDelay[${b}]}) /varcalc tSpellDelay ${ADSpellDelay[${b}]}+${Macro.RunTime}
      }
      /if (${Spell[${tSpellID}].RecastTime.TotalSeconds} && (${tSpellDelay}<=${Macro.RunTime} || ${Math.Calc[${tSpellDelay}-${Macro.RunTime}]}<${Spell[${tSpellID}].RecastTime.TotalSeconds})) /varset tSpellDelay ${Spell[${tSpellID}].RecastTime.TotalSeconds}
      /varset ADMDL[${a},${b}] ${tSpellDelay}
      /if (${ADSameSpell[${b}]}) /varset ADMDL[${a},${ADSameSpell[${b}]}] ${tSpellDelay}
      /if (${ADCharmAD} && ${DoPet}) {
         /if (${ADCharmAD}==${b}) {
            /varset ADCharmID ${ADMobList[${a}]}
            /varset PetAssist 1
            /bc Msg: Charmed a pet: ${ADCharmID} - ${Spawn[${ADCharmID}].CleanName}
            /bc ExcludeID ${ADCharmID}
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Charmed a pet: ${ADCharmID} - ${Spawn[${ADCharmID}].CleanName}
         }
      }
      /varset MBCurrentSpell
      /doevents
:tCheckforAdds
      /call ADSpawnCheck
      /if (!${ADMobCount}) /return
      /if (${AddSpawn}) {
         /varset tADAddDetected TRUE
         /if (${ADMaxPRI}) /return
      }
:tWhatNext
      /if (${ADTarType[${b}]}>9 && ${ADTarType[${b}]}<99 && ${ADTarCnt[${b}]}>0) /return
   /next b
/return


Sub Event_PetNoWake(string Line,string MobName)
   /if (${PetAssist}) /varset PetAssist 1
   /if (${SPetAssist}) /varset SPetAssist 1
   /doevents flush PetNoWake
/return


| We have these SpellResist checks in place because of the ever-increasing number of Proc spells, trigger spells, and 'extra' spells that can go off during play.
| Each of these can be resisted (and other messages) and will trigger the CAST_xxxx in MQ2Cast. We are trying to discover whether the particular spell we cast was resisted, 
| interrupted, fizzled, or if the character was stunned, or a triggered a spell that was resisted/blocked (like the Enchanter self-ward proc, etc).
Sub ResetSpellResists
   /doevents flush SpellResist
   /varset ADResistCount 0
   /varset ADResistMatch
/return


Sub CheckSpellResists
   /declare a int local
   /varset ADResistMatch
   /while (1) {
      /varset ADSpellResisted FALSE
      /doevents SpellResist
      /if (${ADResistMatch.Length}) /return
      /if (!${ADSpellResisted}) /break
   }
   /if (!${ADResistCount}) /return
   /for a 1 to ${ADResistCount}
      /if (${ADEventSpell.Find[${ADResistSpell[${a}].Arg[1,|]}]}) {
         /if (!${ADResistMatch.Length}) {
            /varset ADResistMatch ${ADResistSpell[${a}].Arg[2,|]}
         } else /if (!${ADResistMatch.Find[${ADResistSpell[${a}].Arg[2,|]}]}) {
            /varset ADResistMatch ${ADResistMatch}|${ADResistSpell[${a}].Arg[2,|]}
         }
      }
   /next a
| /echo CSR - ${Time} - ${Cast.Result} - ${ADEventSpell} - ${ADResistCount} - ${ADResistMatch}
/return


Sub Event_SpellResist(string Line,string SpellName)
   /if (!${Defined[SpellName]}) {
      /declare SpellName string local
   }
   /varset ADSpellResisted TRUE
   /if (${Line.Find[fizzles]}) {
      /varset ADResistMatch FIZZLES
   } else /if (${Line.Find[while stunned]}) {
      /varset ADResistMatch STUNNED
   } else /if (${Me.Class.ShortName.Equal[BRD]} && (${Line.Find[stunned]} || ${Line.Find[miss a note]})) {
      /varset ADResistMatch STUNNED
   } else /if (${ADResistCount}<10) { 
      /varcalc ADResistCount ${ADResistCount}+1
      /if (${Line.Find[take hold]}) {
         /varset ADResistSpell[${ADResistCount}] ${SpellName}|TAKEHOLD
      } else /if (${Line.Find[target cannot be]}) {
         /varset ADResistSpell[${ADResistCount}] xxx|IMMUNE
      } else /if (${Line.Find[are missing]}) {
         /varset ADResistSpell[${ADResistCount}] xxx|COMPONENTS
      } else /if (${Line.Find[interrupted]}) {
         /if (${ADResistCount}>1) {
            /if (${ADResistSpell[${Math.Calc[${ADResistCount}-1]}].Equal[xxx|COMPONENTS]}) {
               /varset ADResistSpell[${Math.Calc[${ADResistCount}-1]}] ${SpellName}|COMPONENTS
            }
         }
         /varset ADResistSpell[${ADResistCount}] ${SpellName}|INTERRUPTED
      } else {
         /if (${ADResistCount}>1) {
            /if (${ADResistSpell[${Math.Calc[${ADResistCount}-1]}].Equal[xxx|IMMUNE]}) {
               /varset ADResistSpell[${Math.Calc[${ADResistCount}-1]}] ${SpellName}|IMMUNE
            }
         }
         /varset ADResistSpell[${ADResistCount}] ${SpellName}|RESIST
      }
| /echo ESRa - ${Time} - ${Cast.Result} - ${SpellName} - ${ADResistSpell[${ADResistCount}]}
   }
| /echo ESRb - ${Time} - ${Cast.Result} - ${SpellName} - ${ADResistMatch}
/return
