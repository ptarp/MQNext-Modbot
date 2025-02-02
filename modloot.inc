
#Event CantLoot "You may not loot this corpse#*#"
#Event Forage "You have#*#scrounged up#*#"
|#Event InventoryFull "#*#Your inventory appears full!#*#"
|#Event InventoryFull "#*#There are no open slots for the held item in your inventory.#*#"
|#Event Lore "#*#You cannot loot this Lore Item.#*#"
|#Event UnwantedLoot "#*#No one was interested in the#*#item(s): #1#. These items#*#"
|#Event LockedLoot "#*#These item(s) are locked#*#"

Sub SetupAdvLootVars
   /declare x int local
   /declare CantLootID int outer
   /declare MasterCorpseList string outer
   /declare LootList string outer
   /declare RelootTimer timer outer
   /declare LootCleanupTimer timer outer 10m
   /declare ItemForaged bool outer FALSE
   /declare ForageIni string outer
  
   /call LoadINIVar Settings LootIni "Loot.ini" LootIni "${MBCommon}" string
   /if (!${LootIni.Find[/]} && !${LootIni.Find[\]}) /varset LootIni ${IniLocation}${LootIni}
   
|  /call LoadINIVar Settings LootMobs TRUE LootMobs "${LootIni}" bool
   /call LoadINIVar Settings CorpseRadius 100 CorpseRadius "${LootIni}" int
   /call LoadINIVar Settings MobsTooClose 50 MobsTooClose "${LootIni}" int
   /call LoadINIVar Settings CorpseRotTime 440s CorpseRotTime "${LootIni}" string
|  /call LoadINIVar Settings ReportLoot FALSE ReportLoot "${LootIni}" bool
|  /call LoadINIVar Settings LootChannel bc LootChannel "${LootIni}" string
   /call LoadINIVar Settings QuestOnly "FALSE" QuestOnly "${LootIni}" bool
   /call LoadINIVar Settings KeepScript " " KeepScript "${LootIni}" string
| Convert old ReportLoot+LootChannel to LootAnnounce
   /if (!${Ini.File[${LootIni}].Section[Settings].Key[LootAnnounce].Exists} && ${Ini.File[${LootIni}].Section[Settings].Key[LootChannel].Exists} && ${Ini.File[${LootIni}].Section[Settings].Key[ReportLoot].Value.Equal[TRUE]}) {
      /call LoadINIVar Settings LootAnnounce "${If[${Ini.File[${LootIni}].Section[Settings].Key[LootChannel].Value.Length},/${Ini.File[${LootIni}].Section[Settings].Key[LootChannel].Value}, ]}" LootAnnounce "${LootIni}" string
   } else {
      /call LoadINIVar Settings LootAnnounce " " LootAnnounce "${LootIni}" string 
   }
   /call LoadINIVar Settings ForageAnnounce " " ForageAnnounce "${LootIni}" string
   /call LoadINIVar Settings Category "Tradeskill,Spellscroll" Category "${LootIni}" string
   /for x 1 to ${Math.Calc[${Category.Count[,]}+1]}
      /call LoadINIVar Settings ${Category.Arg[${x},,]} "Toon1Name,Toon2Name" ${Category.Arg[${x},,]} "${LootIni}" string
|/echo ${If[${TimeStampOnEcho},[${Time}] ,]}Category arg ${x} : ${Category.Arg[${x},,]}
   /next x

| Look for old ForageIni key. If key and file exist, convert the file and mark it converted.
   /if (${Ini.File[${IniFile}].Section[Settings].Key[ForageIni].Exists}) {
      /if (${Ini.File[${IniFile}].Section[Settings].Key[ForageIni].Value.Length}) {
         /varset ForageIni ${Ini.File[${IniFile}].Section[Settings].Key[ForageIni].Value}
         /if (!${ForageIni.Find[/]} && !${ForageIni.Find[\]}) /varset ForageIni ${IniLocation}${ForageIni}
         /if (${Ini.File[${ForageIni}].Section[ForageList].Exists} && !${Ini.File[${ForageIni}].Section[Settings].Key[FileConverted].Value.Equal[TRUE]}) {
            /call ForageToLoot
            /ini "${ForageIni}" "Settings" "FileConverted" "TRUE"
         }
      }
   }
/return


Sub LootMobs
   /if (!${DoLoot} || ${Me.Combat} || ${MBPause} || ${RelootTimer}) /return
|  /varset RelootTimer ${Math.Rand[20,40]}s
   /varset RelootTimer 10s
   
   /declare tSpawnParms string local corpse radius ${CorpseRadius} zradius 50 
   /declare DeadCount int local ${SpawnCount[${tSpawnParms}]}
   /if (!${DeadCount}) /return   
   /if (${SpawnCount[npc los radius ${MobsTooClose} zradius ${ZRadius} noalert ${AlertList}]}) /return
   
   /declare i int local
   /declare CorpseList string local
   /declare CorpseToLoot int local
|  /declare tZDist float local ${Math.Distance[${Target.Z}:${Me.Z}]}
   /declare ClosestCorpse int local
   /declare CorpseCount int local 0
   /declare LootMove bool local FALSE
   /declare LMStartX int local
   /declare LMStartY int local
   /declare LMStartHeading
   
   /for i 1 to ${DeadCount}
      /varset ClosestCorpse ${NearestSpawn[${i},${tSpawnParms}].ID}
      /if (!${ClosestCorpse}) /break
      /if (!${Spawn[${ClosestCorpse}].Deity.ID}) {
         /call AddToList "${CorpseList}" "${ClosestCorpse}"
         /varset CorpseList ${Macro.Return}
         /if (!${Select[${ClosestCorpse},${MasterCorpseList}]}) {
            /call AddToList "${MasterCorpseList}" "${ClosestCorpse}"
            /varset MasterCorpseList ${Macro.Return}
         }
      }
   /next i
   /if (!${CorpseList.Length}) /return   
   /for i 1 to ${Math.Calc[${CorpseList.Count[,]}+1]}
      /varset CorpseToLoot ${CorpseList.Arg[${i},,]}
      /if (!${Defined[CorpseLocked${CorpseToLoot}]}) {
         /declare CorpseLocked${CorpseToLoot} timer outer 0
      }
      /if (${Spawn[${CorpseToLoot}].Deity.ID} || !${Spawn[${CorpseToLoot}].ID} || !${Spawn[${CorpseToLoot}].LineOfSight} || ${CorpseLocked${CorpseToLoot}}) /continue
      /if (${Target.ID}!=${CorpseToLoot}) {
         /squelch /target id ${CorpseToLoot}
      }
      /if (!${Me.Standing}) /stand
      /if (${Me.Sneaking}) {
         /doability sneak
         /delay 5
      }
      /delay 3s ${Target.ID}==${CorpseToLoot} && ${Me.Standing}
      /if (${Target.Z}<${Me.Z}) /look -10
      /if (${Target.Distance}>12) {
         /if (${MakeCamp.Status.Equal[on]}) /squelch /makecamp pause   
         /if (${AdvPath.Following} && !${AdvPath.Paused}) {
            /varset FollowState 2
            /squelch /afollow pause
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Msg: Looting. Follow paused.
         } else /if (!${LootMove}) {
            /varset LMStartX ${Me.X}
            /varset LMStartY ${Me.Y}
            /varset LMStartHeading ${Me.Heading.DegreesCCW}
         }
         /if (${Me.Mount.ID}) /dismount
         /call MBMoveTo ${Target.Y} ${Target.X}
         /varset LootMove TRUE
      }
      /delay 10s ${Target.Distance}<=12
      /if (${Target.Distance}<15) { 
          /call LootCorpse ${Target.ID}
          /call RemoveCorpseID ${CorpseToLoot}
      }
      /call ADSpawnCheck Find
      /if (${ADMobFound}) /break
   /next i
   /if (${Corpse.Open}) /call CloseCorpse
   /if (${FollowState}==2) {
      /varset FollowState 1
      /squelch /afollow unpause
   } else /if (${LootMove}) {
      /call MBMoveTo ${LMStartY} ${LMStartX}
      /delay 10s !${Me.Moving}
      /if (${UseHeadings}) {
         /squelch /face nolook ${If[${MacroQuest.BuildName.Equal[emu]},fast ,]}heading ${LMStartHeading}
      }
   }
   /if (${MakeCamp.Status.Equal[paused]}) /squelch /makecamp unpause
   /doevents
/return


Sub LootCorpse(CorpseID)
   /declare i int local
   /declare y int local
   /declare CurrentItem string local
   /declare LootIniEntry string local
   /declare LootIniAction string local
   /declare LootIniCount int local
   /declare x int local 0
   /declare z int local 0
   /declare corpselock timer local 5s
   /declare CanLoot bool local FALSE
   /declare loottotal int local   
   /declare FirstLetter string local
   /declare tNeeded int local
   
   /call ClearCursor force
   /varset CantLootID 0
   /varset LootList
| Open Loot window
   /loot
   /delay 3s ${Window[LootWnd]}
   /delay 1s
   /doevents CantLoot
   /delay 3s ${CantLootID} || ${Window[LootWnd]}
   /if (${Target.ID}==${CantLootID} && !${Window[LootWnd]}) {
      /if (${Defined[CorpseLocked${CorpseID}]}) {
         /varset CorpseLocked${CorpseID} 150s
      }
      /return
   }
   /delay 2
   /if (${Window[LootWnd].Open} && ${Corpse.Items}) {
| Give time for Loot Lag      
      /while (1) {
         /varset loottotal ${Corpse.Items}
         /delay 1s ${loottotal}!=${Corpse.Items}
         /if (${loottotal}==${Corpse.Items} || !${corpselock}) /break
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Loot lag...
      }            
      /for i 1 to ${loottotal}
         /if (!${Corpse.Item[${i}].ID}) /continue
         /if (${Corpse.Item[${i}].Lore} && ${FindItemCount[${Corpse.Item[${i}]}]} ) /continue
         /if (${Corpse.Item[${i}].Lore} && ${FindItemBankCount[${Corpse.Item[${i}]}]} ) /continue
|/echo ${If[${TimeStampOnEcho},[${Time}] ,]}Item - ${i} - ${Corpse.Item[${i}].Name}
         /call DoLootAction Corpse ${i}
         /if (${Macro.Return.Equal[Ignore]}) {
            /call LootItem ${i} Ignore
         } else /if (${Macro.Return.Equal[Destroy]}) {
            /call LootItem ${i} Destroy left ${Corpse.Item[${i}].Stack}
         } else /if (${Macro.Return.Equal[NoRoom]}) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}No room for loot item: "${CurrentItem}" (left on corpse).
            /call LootItem ${i} Ignore
         } else /if (${Macro.Return.Find[Keep]}) {
            /varset tNeeded ${Macro.Return.Arg[2,${PipeChar}]}
            /call LootItem ${i} Keep right ${tNeeded}
         } else {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Invalid action for loot item: "${CurrentItem}" (left on corpse).
            /call LootItem ${i} Ignore
         }
      /next i
      /if (${Corpse.Items}) {
         /if (${LootAnnounce.Length} && ${LootList.Length}) {
            /docommand ${LootAnnounce} Left on corpse(#${Target.ID}): [+r+]${LootList}[+x+]
         }
         /if (${Defined[CorpseLocked${CorpseID}]}) {
            /varset CorpseLocked${CorpseID} ${CorpseRotTime}s
         }
      }
   }
   /call CloseCorpse        
   /call RemoveCorpseID ${CorpseID}
/return


Sub LootItem(int i,string DoWhat,string WhichButton,int HowMany)
| /echo ${If[${TimeStampOnEcho},[${Time}] ,]}LootItem(int i,DoWhat,WhichButton) - ${i}, ${DoWhat} ${WhichButton}
   /if (${DoWhat.Equal[Ignore]} || ${HowMany}<1) /goto :noLoot
| You can't split stacks of no-drop items if you have confirmations on.
| The confirmation dialogue will pop up first. When you click OK, it will loot the whole stack.
   /if (${Corpse.Item[${i}].NoDrop} && ${HowMany}<${Corpse.Item[${i}].Stack} && ${Window[OptionsWindow].Child[OGP_NoDropItemCombobox].GetCurSel}<3) /goto :noLoot
   /declare CorpseItemID int local ${Corpse.Item[${i}].ID}
   /declare CorpseStack int local ${Corpse.Item[${i}].Stack}
   /if (${HowMany}>=${Corpse.Item[${i}].Stack}) {
      /nomodkey /shift /itemnotify Loot${i} ${WhichButton}mouseup
   } else {
      /nomodkey /itemnotify Loot${i} ${WhichButton}mouseup
      /delay 2s ${Window[QuantityWnd].Open}
      /if (!${Window[QuantityWnd].Open}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with Loot Quantity selection - ${Corpse.Item[${i}].Name}
         /goto :noLoot
      }
      /nomodkey /notify QuantityWnd QTYW_Slider newvalue ${HowMany}
      /delay 5
      /nomodkey /notify QuantityWnd QTYW_Accept_button leftmouseup         
   }
   /delay 2
   /if (${Corpse.Item[${i}].NoDrop}) {
      /delay 5s ${Window[ConfirmationDialogBox].Open} || ${Window[OptionsWindow].Child[OGP_NoDropItemCombobox].GetCurSel}>2
      /if (${Window[ConfirmationDialogBox].Open}) /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
      /delay 2
   }
   /delay 3s !${Corpse.Item[${i}].ID} || ${Corpse.Item[${i}].Stack}!=${CorpseStack}
   /if (${WhichButton.Equal[left]}) {
      /delay 5s ${Cursor.ID}
      /if (${DoWhat.Equal[Destroy]} && ${Cursor.ID}==${CorpseItemID}) {
         /destroy
         /delay 2
      }
   }      
   /if (${Cursor.ID}) /call ClearCursor force
   /delay 2
:noLoot
   /if (${Corpse.Item[${i}].ID}) {
      /varset LootList ${LootList}${If[!${LootList.Length},,,]}${Corpse.Item[${i}].Name}${If[${Corpse.Item[${i}].Stack}>1,x${Corpse.Item[${i}].Stack},]}
   }
/return


Sub Event_CantLoot
   /varset CantLootID ${Target.ID}
/return


Sub RemoveCorpseID(CListID)
   /if (${Defined[CorpseLocked${CListID}]}) {
      /if (!${CorpseLocked${CListID}} && ${Select[${CListID},${MasterCorpseList}]}) {
         /deletevar CorpseLocked${CListID}
         /call RemoveFromList "${MasterCorpseList}" "${CListID}"
         /varset MasterCorpseList ${Macro.Return}         
      }
   }
/return


Sub CloseCorpse
   /declare corpselock timer local 10s
   /while (${Corpse.Open} && ${corpselock}) {
      /nomodkey /notify LootWnd LW_DoneButton leftmouseup
      /delay 5 !${Corpse.Open}
   }
   /if (${Corpse.Open}) /nomodkey /keypress esc
/return


Sub CheckFreeInventory(string TLO)
   /if (${Me.FreeInventory}) /return TRUE
   /if (!${${TLO}.Stackable}) /return FALSE
   /if (!${FindItemCount[=${${TLO}.Name}]}) /return FALSE
   /if (${${TLO}.FreeStack}>=${${TLO}.Stack}) /return TRUE
/return FALSE


Sub DoLootAction(string Source,int CorpseSlot)
   /declare x int local
   /declare y int local
   /declare z int local
   /declare CurrentItem string local
   /declare LootIniEntry string local
   /declare LootIniAction string local
   /declare LootIniCount int local
   /declare CanLoot bool local FALSE
   /declare FirstLetter string local
   /declare tNeeded int local
   /declare TLO string local
   
   /if (${Source.Equal[Corpse]}) {
      /if (!${CorpseSlot}) /return Invalid
      /varset TLO Corpse.Item[${CorpseSlot}]
   } else /if (${Source.Equal[Cursor]}) {
      /varset TLO Cursor
   } else {
      /return Invalid
   }
   /if (!${${TLO}.ID}) /return Invalid
|/echo ${If[${TimeStampOnEcho},[${Time}] ,]}Item - ${TLO} - ${${TLO}.Name}
   /varset CurrentItem ${If[${${TLO}.Name.Find[,]},${${TLO}.Name.Arg[1,,]}${${TLO}.Name.Arg[2,,]},${${TLO}.Name}]}
   /varset FirstLetter ${${TLO}.Name.Left[1]}
   /varset LootIniEntry ${Ini["${LootIni}","${FirstLetter}","${CurrentItem}"]}
   /if (!${LootIniEntry.Length} || ${LootIniEntry.Equal[NULL]}) {
      /ini "${LootIni}" "${FirstLetter}" "${CurrentItem}" ${If[${${TLO}.NoDrop},Ignore,Keep]}
      /varset LootIniEntry ${Ini["${LootIni}","${FirstLetter}","${CurrentItem}"]}
      /if (${Source.Equal[Corpse]}) {
         /if (${LootAnnounce.Length}) {
            /docommand ${LootAnnounce} [+y+]New loot item[+x+] added: "${CurrentItem}" (${LootIniEntry}).
         }
      } else {
         /if (${ForageAnnounce.Length}) {
            /docommand ${ForageAnnounce} [+y+]New forage item[+x+] added: "${CurrentItem}" (${LootIniEntry}).
         }
      }
   }
   /varset LootIniAction ${LootIniEntry.Arg[1,${PipeChar}]}
   /varset LootIniCount ${LootIniEntry.Arg[2,${PipeChar}]}
   /if (${LootIniAction.Equal[Ignore]}) {
      /return Ignore
   }
   /if (${LootIniAction.Equal[Destroy]}) {
      /if (!${QuestOnly}) {
         /return Destroy
      }
      /return Ignore
   }
   /if (${LootIniAction.Equal[Keep]} && ${LootIniCount}) {
      /if (${TLO.Equal[Cursor]}) {
         /if (${FindItemCount[=${${TLO}.Name}]}>${LootIniCount}) {
            /return Ignore
         }
      } else /if (${FindItemCount[=${${TLO}.Name}]}>=${LootIniCount}) {
         /return Ignore
      }
   }
   /if (${TLO.NotEqual[Cursor]} && ${${TLO}.Lore}) {
      /if (${FindItem[=${${TLO}.Name}].ID} || ${FindItemBank[=${${TLO}.Name}].ID}) {
         /return Ignore
      }
   }
   /call CheckFreeInventory ${TLO}
   /if (${Macro.Return.Equal[FALSE]}) {
      /return NoRoom
   }
   /if (${Select[${LootIniAction},Keep,Sell,Buy,${Me.Name}]} || (${Select[${LootIniEntry},${Category}]} && !${LootIniEntry.Find[Quest${PipeChar}]})) {
      /if (!${QuestOnly} && ${Select[${LootIniAction},Keep,Sell]}) {
         /varset CanLoot TRUE
      } else {
         /for z 1 to ${Math.Calc[${LootIniEntry.Count[${PipeChar}]} + 1]}
            /if (${LootIniEntry.Arg[${z},${PipeChar}].Equal[${Me.Name}]} || ${LootIniEntry.Arg[${z},${PipeChar}].Equal[Keep]}) {
               /varset CanLoot TRUE
               /break
            }
            /if (${Select[${LootIniEntry.Arg[${z},${PipeChar}]},${Category}]} && !${LootIniEntry.Find[Quest${PipeChar}]}) {
               /if (!${${LootIniEntry.Arg[${z},${PipeChar}]}.Find[${Me.Name}]} && !${${LootIniEntry.Arg[${z},${PipeChar}]}.Find[${Me.Class.ShortName}]} && !${${TLO}.NoDrop}) {
                  /for y 1 to ${Math.Calc[${${LootIniEntry.Arg[${z},${PipeChar}]}.Count[,]}+1]}
| this loop is just here to loot the item if no-one matching the category params in in group.  If toon not here, loot, otherwise check quests.
| /echo ${If[${TimeStampOnEcho},[${Time}] ,]}LootCheck:  Category: ${LootIniEntry.Arg[${z},|]}  Spawncheck for: ${${LootIniEntry.Arg[${z},|]}.Arg[${y}, ]}
                     /if (${Spawn[pc group ${${LootIniEntry.Arg[${z},${PipeChar}]}.Arg[${y}, ]}].ID}) {
                        /break
                     }
                  /next y
                  /if (${y}>${Math.Calc[${${LootIniEntry.Arg[${z},${PipeChar}]}.Count[,]}+1]}) /break
                  /if (!${QuestOnly}) {
                     /varset CanLoot TRUE
                     /break
                  }
               } else /if (${${LootIniEntry.Arg[${z},${PipeChar}]}.Find[${Me.Name}]} || ${${LootIniEntry.Arg[${z},${PipeChar}]}.Find[${Me.Class.ShortName}]}) {
                  /if (!${QuestOnly}) {
                     /varset CanLoot TRUE
                     /break
                  }
               } else /if (!${Select[${LootIniEntry},${Category}]} && !${LootIniEntry.Find[Quest${PipeChar}]} && !${QuestOnly}) {
                  /if (${KeepScript.Length} && ${LootIniEntry.Find[Sell]} || ${LootIniEntry.Find[Script]}) {
                     /call MBScript ${KeepScript} ${TLO}
                  } else {
                     /varset CanLoot TRUE
                     /break
                  }
               }               
            }
         /next z
      }
   }
   /if (!${CanLoot} && (${DoQuest} || ${QuestOnly}) && ${LootIniAction.Equal[Quest${PipeChar}]}) {
      /if (${FindItemCount[=${${TLO}.Name}]}<${LootIniCount} || (${TLO.Equal[Cursor]} && ${FindItemCount[=${${TLO}.Name}]}==${LootIniCount})) {
         /if (${LootIniEntry.Arg[3,${PipeChar}].Length}) {
            /if (${Window[TaskWnd].Child[TASK_TaskList].Items}) {
               /for x 1 to ${Window[TaskWnd].Child[TASK_TaskList].Items}
                  /if (${Window[TaskWnd].Child[TASK_TaskList].List[${x},2].Equal[${LootIniEntry.Arg[3,${PipeChar}]}]}) {
                     /varset CanLoot TRUE
                     /break
                  }
               /next x
            }
         } else {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Please specify quest name for ${${TLO}.Name}. Format for "${LootIni}" is =Quest${PipeChar}<count>${PipeChar}<quest name>${PipeChar}  Looting item.
            /varset CanLoot TRUE
         }
      }
   }
   /if (${CanLoot}) {
      /if ((${LootIniEntry.Find[Keep${PipeChar}]} || ${LootIniEntry.Find[Quest${PipeChar}]}) && ${${TLO}.Stack}>1) {
         /varcalc tNeeded ${LootIniCount}-${FindItemCount[=${${TLO}.Name}]}${If[${TLO.Equal[Cursor]},+1,]}
         /if (${${TLO}.Stack}<${tNeeded}) {
            /varset tNeeded ${${TLO}.Stack}
         }
      } else {
         /varset tNeeded ${${TLO}.Stack}
      }
      /return Keep${PipeChar}${tNeeded}
   }
| Guess we don't want it
/return Ignore


Sub LootCleanup(string Options)
   /varset LootCleanupTimer 10m
   /if (!${MasterCorpseList.Length}) /return
   /declare i int local
   /for i 1 to ${Math.Calc[${MasterCorpseList.Count[,]}+1]}
      /if (${Defined[CorpseLocked${MasterCorpseList.Arg[${i},,]}]}) {
         /if (!${CorpseLocked${MasterCorpseList.Arg[${i},,]}} || ${Options.Find[zone]}) {
            /deletevar CorpseLocked${MasterCorpseList.Arg[${i},,]}
            /call RemoveFromList "${MasterCorpseList}" "${MasterCorpseList.Arg[${i},,]}"
            /varset MasterCorpseList ${Macro.Return}
         }
      }
   /next i
/return


Sub Event_Forage(string eLine)
   /varset ItemForaged TRUE
/return


Sub ForageStuff
   /if (${Window[InventoryWindow].Open} || ${Window[BigBankWnd].Open}) /return
   /if (!${Me.AbilityReady[forage]}) /return
   /if (${Cursor.ID}) {
      /call ClearCursor
      /if (${Cursor.ID}) /return
   }
   /declare CursorItemID local int
   /if (!${Me.Mount.ID} && !${Me.Standing}) {
      /stand
      /delay 5
   }
   /doevents flush forage
   /doability forage
   /delay 1s || ${Cursor.ID}
   /delay 2
   /varset ItemForaged FALSE
   /doevents Forage
   /while (${ItemForaged}) {
      /delay 5 ${Cursor.ID}
      /if (!${Cursor.ID}) /break
      /varset CursorItemID ${Cursor.ID}
      /call DoLootAction Cursor
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You have foraged: "${Cursor.Name}" (${Macro.Return.Arg[1,${PipeChar}]})
      /if (${Macro.Return.Equal[Destroy]}) {
         /if (${Cursor.ID}==${CursorItemID}) {
            /destroy
         } else {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Destroy Foraged item mis-match: "${Cursor.Name}" (dropped).
            /drop
         }
      } else /if (${Macro.Return.Equal[Ignore]}) {
         /if (${Cursor.NoDrop}) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Cannot drop No-Drop foraged item: "${Cursor.Name}" (destroying).
            /destroy
         } else {
            /drop
         }
      } else /if (${Macro.Return.Find[Keep]}) {
         /timed 5 /autoinventory
         /delay 2s !${Cursor.ID}
      } else /if (${Macro.Return.Equal[NoRoom]}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}No room for foraged item: "${Cursor.Name}" (dropped).
         /drop
      } else {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Invalid action for foraged item: "${Cursor.Name}" (dropped).
         /drop
      }
      /delay 5
      /varset ItemForaged FALSE
      /doevents Forage
   }
   /delay 2s !${Cursor.ID}
   /doevents flush forage
/return


| One-Time Migrate forage.ini to loot.ini
Sub ForageToLoot
   /declare i int local
   /declare ForageIniEntry string local
   /declare ForageKey string local
   /declare ForageKeyValue string local
   /declare LootKeyValue string local
   /declare LootIniEntry string local
   /declare FirstLetter string local
   /declare ForageIniCount int local
   /declare ForageIniConverted int local
   /declare ForageIniExisted int local
   /declare ForageIniIgnored int local

   /varset ForageIniCount ${Ini.File[${ForageIni}].Section[ForageList].Key.Count}
   /if (!${ForageIniCount}) /return

   /echo One-Time Converting ForageINI data to LootINI...Starting
   
| We're going to Skip the entries with -1. This was the default, which also forces a Keep. However, this happened any time
| something was on the cursor, not just from foraging. So, the file is full of -1 values unrelated to foraging.
| If they are foraged in the future, they will get added back as Keep (the default).   
   /for i 1 to ${ForageIniCount}
      /varset ForageKey ${Ini.File[${ForageIni}].Section[ForageList].Key.KeyAtIndex[${i}]}
      /varset ForageKeyValue ${Ini.File[${ForageIni}].Section[ForageList].Key.ValueAtIndex[${i}]}
      /if (${ForageKey.Length} && ${ForageKey.NotEqual[NULL]} && ${ForageKeyValue.Length} && ${ForageKeyValue.NotEqual[-1]}) {
         /varset LootKeyValue ${ForageKeyValue}
         /if (${LootKeyValue.Equal[0]}) {
            /varset LootKeyValue Ignore
         } else {
            /call IsNumber ${LootKeyValue}
            /if (${Macro.Return.Equal[TRUE]}) {
               /varset LootKeyValue Keep${PipeChar}${LootKeyValue}
            }
         }
         /varset FirstLetter ${ForageKey.Left[1]}
         /varset LootIniEntry ${Ini["${LootIni}","${FirstLetter}","${ForageKey}"]}
         /if (!${LootIniEntry.Length} || ${LootIniEntry.Equal[NULL]}) {
            /echo ForageINI: ${ForageKey}=${ForageKeyValue} --> LootINI: ${ForageKey}=${LootKeyValue}
            /ini "${LootIni}" "${FirstLetter}" "${ForageKey}" "${LootKeyValue}"
            /varcalc ForageIniConverted ${ForageIniConverted}+1
         } else {
            /if (${LootKeyValue.NotEqual[${LootIniEntry}]}) {
               /echo ForageINI: ${ForageKey}=${ForageKeyValue} - Already exists in LootINI as: ${LootIniEntry}
            }
            /varcalc ForageIniExisted ${ForageIniExisted}+1
         }   
      } else {
         /varcalc ForageIniIgnored ${ForageIniIgnored}+1
      }
   /next i

   /echo Forage keys found: ${ForageIniCount}
   /echo Forage keys with value Null/-1 (Ignored): ${ForageIniIgnored}
   /echo Forage keys already in LootINI (Ignored): ${ForageIniExisted}
   /echo Forage keys converted to LootINI: ${ForageIniConverted}
   /echo One-Time Converting ForageINI data to LootINI...Completed  
/return