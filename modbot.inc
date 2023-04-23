	
#Event Invite "#*#invites you to join a #1#.#*#"
#Event Slain "#*#You have been slain by#*#"
#Event Slain "#*#Returning to Bind Location#*#"
#Event Slain "You died."
#Event DelayOnZone "#*#You have entered#*#"
#Event DelayOnZone "#*#LOADING, PLEASE WAIT.#*#"
#Event SummonFW "You are low on #1#."
#Event SummonFW "You are out of #1#."
#Event WornOff "#*#Your #1# spell has worn off of #2#."
#Event Camping "#*#more seconds to prepare your camp#*#"
#Event GoM "#*#granted#*#gift of #1# to #2#!"
#Event OutOfStuff "Sorry, but you don't have everything you need for this recipe in your general inventory."
#Event Broke "#*#you can#*#t afford#*#"
#Event EQBC "<#1#> #2#"
#Event EQBC "[#1#(msg)] #2#"
#Event EQBC "[MQ2] mb- #2#"
#Event EQBC "mb- #2#"
#Event AAEarned "#*#You have gained#*#ability point#*#"
#Event AACapped "You have reached the AA point cap#*#"
|#event ResetMelee "You cannot see your target."

Sub Startup
   /if (!${Plugin[MQ2Eqbc].Name.Length}) {
      /squelch /plugin MQ2Eqbc noauto
      /if (!${Plugin[MQ2Eqbc].Name.Length}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You must have MQ2EQBC plugin to use this macro!!
         /endmacro
      }
   }
   /if (!${Plugin[MQ2Netbots].Name.Length}) {
      /squelch /plugin MQ2Netbots noauto
      /if (!${Plugin[MQ2Netbots].Name.Length}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You must have MQ2Netbots plugin to use this macro!!
         /endmacro
      }
   }
   /if (!${Plugin[MQ2Melee].Name.Length}) {
      /squelch /plugin MQ2Melee noauto
      /if (!${Plugin[MQ2Melee].Name.Length}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You must have MQ2Melee plugin to use this macro!!
         /endmacro
      }
      /bc Msg: You should configure MQ2Melee if you want to melee
   }
   /if (!${Plugin[MQ2Cast].Name.Length}) {
      /squelch /plugin MQ2Cast noauto
      /if (!${Plugin[MQ2Cast].Name.Length}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You must have MQ2Cast plugin to use this macro!!
         /endmacro
      }
   }
   /if (!${Plugin[MQ2Exchange].Name.Length}) {
      /squelch /plugin MQ2Exchange noauto
      /if (!${Plugin[MQ2Exchange].Name.Length}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You must have MQ2Exchange plugin to use this macro!!
         /endmacro
      }
   }
   /if (!${Plugin[MQ2AdvPath].Name.Length}) {
      /squelch /plugin MQ2AdvPath noauto
      /if (!${Plugin[MQ2AdvPath].Name.Length}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You must have MQ2AdvPath plugin to use this macro!!
         /endmacro
      }
   }
   /if (!${Plugin[MQ2MoveUtils].Name.Length}) {
      /squelch /plugin MQ2MoveUtils noauto
      /if (!${Plugin[MQ2MoveUtils].Name.Length}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You must have MQ2MoveUtils plugin to use this macro!!
         /endmacro
      }
   }
   /if (${Me.Class.ShortName.Equal[BRD]}) {
      /if (!${Plugin[MQ2Twist].Name.Length} ) {
         /squelch /plugin MQ2Twist noauto
         /if (!${Plugin[MQ2Twist].Name.Length} ) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Bards must have MQ2Twist plugin to use this macro!!
            /endmacro
         }
      }
   }

   /if (!${NetBots.Enable} || !${NetBots.Listen} || !${NetBots.Output}) {
      /netbots on send=on grab=on
   }
   /squelch /stick off
   /declare CurMerchant int outer 0
   /declare CurrentEQBCCommand string outer
   /declare AlertList int outer 20
   /declare MiscCheckTimer timer outer
   /declare SitCheckTimer timer outer
   /declare AACheckTimer timer outer
   /declare SenderName string outer
   /declare FollowFlagZoneDelay int outer 20
   /declare DoBuffBot bool outer
   /declare CastResult string outer UNKNOWN
   /declare CampStatus int outer
   /declare MakeCampX int outer
   /declare MakeCampY int outer
   /declare MakeCampD int outer
   /declare MBPause bool outer
   /declare QueueCount int outer
   /declare DoBuffQueue[25,2] int outer
   /declare MBCurrentSpell string outer
   /declare DoQuest bool outer TRUE
   /declare DoCombines bool outer FALSE
   /declare TankName string outer
   /declare PullerName string outer
   /declare EchoScripts bool outer False
   /declare NamedID int outer
   /declare BardCombatGems string outer
   /declare BardRestGems string outer
   /declare GemStuckTimer timer outer
   /declare GemStuckID int outer
   /declare GemStuckRetry timer outer
   /declare SpellWasCast bool outer FALSE
   /declare ZoneName string outer
   /declare ExcludeListID string outer
   /declare NextBardSlot int outer 40
   /declare PSTargetID int outer 0
   /declare ValidSpellGems string outer 1,2,3,4,5,6,7,8,9,10,11,12,13
   /declare pcClasses string outer enc wiz mag nec clr dru shm pal shd war bst rng ber rog brd mnk
   /declare IgnorePrefixes string outer Cast:,Buffed:,Debuffed:,Healing:,Meleeing:,Curing:,Selling:,Setting,attacking,Msg:,Evented:
| Simple On/Off Toggle Commands - Upper/Lower Case is important here. It has to match the variable declaration
   /declare OnOffCommands string outer DoAfk,DoBuffBot,DoBuffs,DoCombines,DoCures,DoDebuffs,DoEvents,DoForage,DoFW,DoHeals,DoLoot,DoMelee,DoMount,DoNamed,DoPet,DoPetShrink,DoPetToys,DoRez,DoSelfShrink,DoSit,DoTells,OffTank
| Action Commands - Case is unimportant. Will call a Subroutine named "Command_" + uppercase version of it.   
   /declare ActionCommands string outer Abort,AddBuff,AddDebuff,Attack,AutoBank,Backoff,Buffup,Campout,DoAA,DoCommand,DoList,DoPull,DoQuest,DropPet,Duck,Exclude,ExcludeID,Follow,GetBank,Give,Goto,HailDelay,HandIn,Help,Holdup,Include,IncludeID,InviteAll,LetsRoll,MakeCamp,Mana,MBPause,MBWayPlay,MBWayRec,PetToys,Recast,SayDelay,SayTarget,Script,Search,Sell,SetINIvar,SetPuller,SetTank,Stop,Switch,TankAndHeals,
| PetToys items
   /declare PTCount int outer 0
   /declare PTSpell[5] string outer
   /declare PTItem1[5] string outer
   /declare PTItem2[5] string outer
| MQ2Cast items
   /declare castReturn string outer NULL
   /declare noFeigning bool outer FALSE
   /declare noInvis bool outer FALSE
   /declare interruptFlag bool outer FALSE
| Equipment Slots (23) - Order is Important
   /declare slotNames string outer charm,leftear,head,face,rightear,neck,shoulder,arms,back,leftwrist,rightwrist,ranged,hands,mainhand,offhand,leftfinger,rightfinger,chest,legs,feet,waist,powersource,ammo
   /declare numSlots int outer ${Math.Calc[${slotNames.Count[,]}+1]}
   /declare equippedArray[${numSlots}] string outer

   /declare ExcludeList string outer ${If[${Ini["${MBCommon}","ExcludeList","${Zone.ShortName}",NOTFOUND].NotEqual[NOTFOUND]},${Ini["${MBCommon}","ExcludeList","${Zone.ShortName}",NOTFOUND]},|]}
   /declare FollowFlag int outer
   /declare PauseFlag int outer 0
   /declare FollowID int outer
   /declare FollowName string outer
   /declare ManualPuller bool outer FALSE
   /declare ManualTank bool outer FALSE
   
   /declare a int local 0 

   /call LoadINIVar Settings AAAnnounce " " AAAnnounce "${IniFile}" string
   /call LoadINIVar Settings AABank 0 AABank "${IniFile}" int
   /call LoadINIVar Settings AAtoNormal FALSE AAtoNormal "${IniFile}" bool
   /call LoadINIVar Settings AAVerbose FALSE AAVerbose "${IniFile}" bool
   /call LoadINIVar Settings AAWarning 90 AAWarning "${IniFile}" int
   /call LoadINIVar Settings AfkMessage " " AFKMessage "${IniFile}" string
   /call LoadINIVar Settings AllowBuffRequests " " AllowBuffRequests "${IniFile}" string
   /call LoadINIVar Settings BeepCommand "beep ding.wav" BeepCommand "${MBCommon}" string
   /call LoadINIVar Settings DeathSlot "FALSE" DeathSlot "${IniFile}" string
   /call LoadINIVar Settings DoAA FALSE DoAA "${IniFile}" bool
   /call LoadINIVar Settings DoAfk FALSE DoAfk "${IniFile}" bool
   /call LoadINIVar Settings DoBuffs FALSE DoBuffs "${IniFile}" bool
   /call LoadINIVar Settings DoCures FALSE DoCures "${IniFile}" bool
   /call LoadINIVar Settings DoDebuffs FALSE DoDebuffs "${IniFile}" bool
   /call LoadINIVar Settings DoEvents FALSE DoEvents "${IniFile}" bool
   /call LoadINIVar Settings DoForage FALSE DoForage "${IniFile}" bool
   /call LoadINIVar Settings DoFW FALSE DoFW "${IniFile}" bool
   /call LoadINIVar Settings DoHeals FALSE DoHeals "${IniFile}" bool
   /call LoadINIVar Settings DoLoot FALSE DoLoot "${IniFile}" bool
   /call LoadINIVar Settings DoMelee FALSE DoMelee "${IniFile}" bool
   /call LoadINIVar Settings DoMount FALSE DoMount "${IniFile}" bool   
   /call LoadINIVar Settings DoNamed TRUE DoNamed "${IniFile}" bool
   /call LoadINIVar Settings DoPet FALSE DoPet "${IniFile}" bool
   /call LoadINIVar Settings DoPetShrink TRUE DoPetShrink "${IniFile}" bool
   /call LoadINIVar Settings DoPull FALSE DoPull "${IniFile}" bool   
   /call LoadINIVar Settings DoRez TRUE DoRez "${IniFile}" bool
   /call LoadINIVar Settings DoSelfShrink FALSE DoSelfShrink "${IniFile}" bool
   /call LoadINIVar Settings DoSit FALSE DoSit "${IniFile}" bool
   /call LoadINIVar Settings DoStuckGems FALSE DoStuckGems "${IniFile}" bool
   /call LoadINIVar Settings DoTells TRUE DoTells "${IniFile}" bool
   /call LoadINIVar Settings FollowDistance 20 FollowDistance "${IniFile}" string
   /call LoadINIVar Settings FollowStick "${FollowDistance} hold uw" FollowStick "${IniFile}" string
   /call LoadINIVar Settings ForageIni "forage.ini" ForageIni "${IniFile}" string
   /call LoadINIVar Settings GoMNuke " " GoMNuke "${IniFile}" string
   /call LoadINIVar Settings InfoMsgLevel 0 InfoMsgLevel "${IniFile}" int
   /call LoadINIVar Settings MasterList "Netbots" MasterList "${IniFile}" string
   /call LoadINIVar Settings NetworkINI " " NetworkINI "${IniFile}" string
   /call LoadINIVar Settings PetAggro FALSE PetAggro "${IniFile}" bool
   /call LoadINIVar Settings PetAssist 0 PetAssist "${IniFile}" int
   /call LoadINIVar Settings PullerName " " AllPullerName "${IniFile}" string
   /call LoadINIVar Settings Radius 100 Radius "${IniFile}" int   
   /call LoadINIVar Settings RezAcceptCall FALSE RezAcceptCall "${IniFile}" bool
   /call LoadINIVar Settings RezGiveUpSeconds 6000 RezGiveUpSeconds "${IniFile}" int
   /call LoadINIVar Settings RezMinPct 96 RezMinPct "${IniFile}" int
   /call LoadINIVar Settings SitAggroRadiusCheck 30 SitAggroRadiusCheck "${IniFile}" int
   /call LoadINIVar Settings SitEndurPct ${If[${Select[${Me.Class.ShortName},CLR,DRU,SHM,ENC,NEC,MAG,WIZ,BRD]},0,95]} SitEndurPct "${IniFile}" int   
   /call LoadINIVar Settings SitManaPct ${If[${Me.MaxMana}==0,0,95]} SitManaPct "${IniFile}" int
   /call LoadINIVar Settings SPetAssist 1 SPetAssist "${IniFile}" int
   /call LoadINIVar Settings StuckGemsAA " " StuckGemsAA "${IniFile}" string
   /call LoadINIVar Settings SummonFood " " SumFood "${IniFile}" string
   /call LoadINIVar Settings SummonDrink " " SumDrink "${IniFile}" string   
   /call LoadINIVar Settings TankName " " AllTankName "${IniFile}" string
   /call LoadINIVar Settings TraderName " " TraderName "${IniFile}" string
   /call LoadINIVar Settings ZRadius 50 ZRadius "${IniFile}" int
   /if (${Me.Class.ShortName.Equal[MAG]}) {
      /call LoadINIVar Settings DoPetToys FALSE DoPetToys "${IniFile}" bool
      /call LoadINIVar Settings PetToys1 " " PetToys1 "${IniFile}" string
      /call LoadINIVar Settings PetToys2 " " PetToys2 "${IniFile}" string
      /call LoadINIVar Settings PetToys3 " " PetToys3 "${IniFile}" string
      /call LoadINIVar Settings PetToys4 " " PetToys4 "${IniFile}" string
      /call LoadINIVar Settings PetToys5 " " PetToys5 "${IniFile}" string
      /for a 1 to 5
         /if (${PetToys${a}.Length}) {
            /varcalc PTCount ${PTCount}+1
            /varset PTSpell[${PTCount}] ${PetToys${a}.Arg[1,,]}
            /varset PTItem1[${PTCount}] ${PetToys${a}.Arg[2,,]}
            /varset PTItem2[${PTCount}] ${PetToys${a}.Arg[3,,]}
         }
      /next a
   }
   
   /if (${AllPullerName.Left[6].Equal[manual]}) {
      /varset ManualPuller TRUE
	  /if (${AllPullerName.Arg[2,|].Length}) {
	     /varset AllPullerName ${AllPullerName.Arg[2,|]}
	  } else {
	     /varset AllPullerName
	  }
   }
   /call SetPuller

   /if (${AllTankName.Left[6].Equal[manual]}) {
      /varset ManualTank TRUE
	  /if (${AllTankName.Arg[2,|].Length}) {
	     /varset AllTankName ${AllTankName.Arg[2,|]}
	  } else {
	     /varset AllTankName
	  }
   }
   /call SetTank

   /if (!${Select[${DeathSlot},TRUE,FALSE]}) /ini "${IniFile}" "Settings" "DeathSlot" "FALSE"
   /if (!${ForageIni.Find[/]} && !${ForageIni.Find[\]}) /varset ForageIni ${IniLocation}${ForageIni}

| Adjustments for Casters / Non-Casters
   /if (${Me.MaxMana}) {
      /varcalc a ${ValidSpellGems.Find[${Me.NumGems}]}+1
      /if (${ValidSpellGems.Mid[${a},1].Equal[,]}) /varcalc a ${a}-1
      /varset ValidSpellGems ${ValidSpellGems.Left[${a}]}
   } else {
      /varset ValidSpellGems 0   
      /varset DoStuckGems FALSE
   }
|/echo ${ValidSpellGems}   

   /if (${Me.Class.ShortName.Equal[BRD]}) {
      /squelch /stoptwist
      /squelch /twist reset
      /squelch /twist reload
   }   
   
   /squelch /makecamp leash 200
   /squelch /makecamp radius 10
   /if (!${MakeCamp.Leash}) /squelch /makecamp leash
   /squelch /makecamp off

   /call LoadINIVar Settings LoadAlias 1 LoadAlias "${MBCommon}" int
   /if (${LoadAlias}) {
      /alias /mb /echo MB-
      /ini "${MBCommon}" "Settings" "LoadAlias" 0
   }

   /call ProcessExcludeList zone
   /varset ZoneName ${Zone.Name}

   /doevents flush
/return


Sub Event_EQBC(EQBCSay,EQBCSender,EQBCCommand)
   /if (!${Defined[EQBCSender]}) {
      /declare EQBCSender string local
   }
   /if (!${DoBuffBot} && !${Select[${EQBCSender},${MasterList}]} && ${EQBCSender.Length} && !${MasterList.Equal[Netbots]} && ${NetBots.Client.Find[${EQBCSender}]}) /return
   /varset EQBCCommand ${EQBCSay.Right[-${Math.Calc[${EQBCSay.Find[${EQBCCommand}]}-1]}]}
   /if (${Select[${EQBCSender},${MasterList}]} || (${EQBCSender.Length} && (${MasterList.Equal[Netbots]} && ${NetBots.Client.Find[${EQBCSender}]}) || ${EQBCSender.Equal[${Me.Name}]})) {
      /if (${Select[${EQBCCommand.Arg[1]},${IgnorePrefixes}]}) /return
      /if (${Select[${EQBCCommand.Arg[2]},${IgnorePrefixes}]}) /return
      /if (${EQBCCommand.Find[immune to]}) /return
      /if (${EQBCCommand.Left[9].Equal[Tell from]}) /return
   }
   /if (!${EQBCSender.Length}) /varset EQBCSender LOCAL  
   /if (${EQBCCommand.Arg[1].Equal[${Me.Name}]}) {
      /varset EQBCCommand ${EQBCCommand.Mid[${Math.Calc[${Me.Name.Length}+1]},${Math.Calc[${EQBCCommand.Length}-${Me.Name.Length}]}]}
   }
   /varset CurrentEQBCCommand ${EQBCCommand}
   /call Event_chat "eqbc" "${EQBCSender}" "${EQBCCommand}"
/return


Sub Event_chat(MsgType,MsgFrom,MsgText)
   /if (${Select[${MsgText.Arg[1]},${IgnorePrefixes}]}) /return
   /if (${MsgText.Find[ master]} || ${MsgFrom.Equal[${Me.Pet.Name}]} || ${Spawn[npc ${MsgFrom}].ID}) /return
   /if (${MsgType.Equal[eqbc]}) /varset MsgText ${CurrentEQBCCommand}
   /if (${MsgType.Equal[group]} && ${MsgText.Find[reconnect]}) {
      /if (${MsgText.Arg[2, ].Length}>0) {
         /if (${EQBC.Connected}) /bccmd quit
         /delay 5 !${EQBC.Connected}
         /delay 5
         /bccmd connect ${MsgText.Arg[2, ]} ${If[${MsgText.Arg[3, ].Length},${MsgText.Arg[3, ]},]}
      } else {	
         /bccmd reconnect
      }
      /return
   }
   /declare a int local
   /if (${MsgType.NotEqual[EQBC]}) {
      /if (${MsgFrom.Left[1].Compare[ ]}<0) {
	     /varset MsgFrom ${MsgFrom.Right[-2].Left[-1]}
	  }
   }
   /if (!${DoBuffBot} && !${Select[${MsgFrom},${MasterList},LOCAL]} && (!${MasterList.Equal[Netbots]} || !${NetBots.Client.Find[${MsgFrom}]}) || ${MsgFrom.Equal[${Me.Name}]}) {
      /if (!${NetBots[${MsgFrom}].ID} && ${MsgType.Equal[TELL]} && ${DoTells}) {
         /if (!${MsgFrom.Equal[${Me.Name}]} && !${MsgFrom.Equal[${Me.Pet.CleanName}]} && !${Spawn[npc ${MsgFrom}].ID}) /call TellEvent ${MsgFrom} "${MsgText}"
      }
      /if (${AllowBuffRequests.Length}) {
         /if (${MsgType.NotEqual[eqbc]} && !${Select[${MsgFrom},${MasterList},LOCAL]} && (!${MasterList.Equal[Netbots]} || !${NetBots.Client.Find[${MsgFrom}]})) {
            /if (${AllowBuffRequests.Find[All]} || (${AllowBuffRequests.Find[Group]} && ${Group.Member[${MsgFrom}].Index}) || (${AllowBuffRequests.Find[Raid]} && ${Raid.Member[${MsgFrom}].Level}) || (${AllowBuffRequests.Find[Guild]} && ${Me.Guild.Length} && ${Me.Guild.Equal[${Spawn[${MsgFrom}].Guild}]}) || (${AllowBuffRequests.Find[Fellowship]} && ${Me.Fellowship.Member[${MsgFrom}].Level})) {
               /for a 1 to ${Math.Calc[${MsgText.Count[ ]}+1]}
                  /if (${ABAliasList.Find[${MsgText.Arg[${a}]}]}) /call CastCall ${MsgFrom} "cast ${MsgText.Arg[${a}]}"
               /next a
            }
         }
      }
      /return
   }

   /if (${MsgFrom.Equal[local]}) {
      /varset SenderName ${Me.Name}
   } else {
      /varset SenderName ${MsgFrom}
   }
| If BuffBot, add 'cast'
   /if (${DoBuffBot}) {
      /varset MsgText cast ${MsgText}
   }
| Check for CastCall variations   
   /if (${Select[${MsgText.Arg[1]},cast,qcast,scast]} || (${Me.Class.ShortName.Equal[brd]} && ${Select[${MsgText.Arg[1]},sing,sung]})) {
      /call CastCall "${SenderName}" "${MsgText}"
      /return
   }
| Handle MBP Abbreviation (Expand it)
   /if (${MsgText.Arg[1].Equal[MBP]}) {
      /varset MsgText MBPause${MsgText.Right[-3]}
   }
| Check our Command Lists
   /if (${Select[${MsgText.Arg[1]},${ActionCommands}]}) {
      /call Command_${ActionCommands.Arg[${Select[${MsgText.Arg[1]},${ActionCommands}]},,].Upper} "${MsgText}"
   } else /if (${Select[${MsgText.Arg[1]},${OnOffCommands}]}) {
      /call TFToggle ${OnOffCommands.Arg[${Select[${MsgText.Arg[1]},${OnOffCommands}]},,]} ${MsgText.Arg[2]}
| Handle our legacy DoSongs alias
   } else /if (${MsgText.Arg[1].Equal[DoSongs]}) {
      /call TFToggle DoBuffs ${MsgText.Arg[2]}
   }
/return


Sub LoadINIVar(IniSection,IniVar,IniValue,MacroVar,MyIni,VarType)
   /if (!${Defined[${MacroVar}]} && ${VarType.Length}) /declare ${MacroVar} ${VarType} outer
   /declare IniString string local ${Ini["${MyIni}",${IniSection},${IniVar},NOTFOUND]}
   /varset ${MacroVar} ${IniString}
   /if (${IniString.Equal["NOTFOUND"]}) {
      /varset ${MacroVar} ${IniValue}
      /ini "${MyIni}" "${IniSection}" "${IniVar}" "${${MacroVar}}"
   }
/return


Sub CharmBroke
   /if (${Me.Pet.ID}) /return
   /bc Msg: LOST PET - LOST PET
   /if (!${ADCharmAD} || !${DoPet}) /return
   /if (${PetAssist}) /varset PetAssist 1
   /if (${SPetAssist}) /varset SPetAssist 1
   /if (!${Me.Mount.ID} && ${Me.Sitting}) /sit off
   /varset ADCheckTimer 0
   /if (${DoDebuffs} && ${ADCount}) /call ADDebuffCheck
/return


Sub AFK
   /docommand ${If[(${AFKMessage.Length} && !${AFKMessage.Equal[AFK Message]}),/afk ${AFKMessage},/afk]}
/return


Sub Event_Broke
   /if (${Defined[Broke]}) /varset Broke TRUE
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Error: You are out of money!
   /docommand /${BeepCommand}
/return TRUE


Sub Event_SummonFW(Line,ExtraStuff)
   /if (!${DoFW} || ${MBPause}) /return
   /if (${ExtraStuff.Find[food]} && ${SumFood.Length}>2) {
      /if (${Me.Invis}) {
         /bc Msg: Cannot summon food while Invis
      } else {
	     /if (${Me.Casting.ID} && !${Me.Class.ShortName.Equal[brd]}) {
            /delay 5s !${Me.Casting.ID}
         }
         /call MQ2Cast "${SumFood.Arg[1,|]}" ${SumFood.Arg[2,|]} 10s CastCheck
         /delay 10s ${Cursor.ID}
         /call ClearCursor force
      }
   }
   /if (${ExtraStuff.Find[drink]} && ${SumDrink.Length}>2) {
      /if (${Me.Invis}) {
         /bc Msg: Cannot summon drink while Invis
      } else {
	     /if (${Me.Casting.ID} && !${Me.Class.ShortName.Equal[brd]}) {
            /delay 5s !${Me.Casting.ID}
         }
         /call MQ2Cast "${SumDrink.Arg[1,|]}" ${SumDrink.Arg[2,|]} 10s CastCheck
         /delay 10s ${Cursor.ID}
         /call ClearCursor force
      }
   }	  
/return


Sub Event_Invite((string eLine,string eType)
   /if (!${Select[${eType},group,raid]}) /return
   /if (${Select[${eLine.Arg[1]},${MasterList}]} || ${NetBots[${eLine.Arg[1]}].ID}) {
      /if (!${Me.Class.ShortName.Equal[brd]} && ${Me.Casting.ID}) {
	     /delay 10s !${Me.Casting.ID}
	  }
      /invite
      /delay 1s
      /if (${Window[ConfirmationDialogBox].Open}) /notify ConfirmationDialogBox CD_Yes_Button leftmouseup
   }
/return


Sub Event_DelayOnZone(ZoneText)
   /if (${ZoneText.Find[Drunken Monkey]} || ${ZoneText.Find[effects]}) /return
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Zoning Detected.
   /declare a int local
   /declare b int local
   /varset DoDebuffs FALSE
   /varset DoMelee FALSE
   /varset DoPull FALSE
   /if (${FollowFlag}) {
      /varset FollowFlag 10
      /stick off
   }
   /if (${MakeCamp.Status.Equal[on]} || ${CampStatus}) {
      /makecamp off
      /varset CampStatus 0
   }
   /if (${PetAssist}) /varset PetAssist 1
   /if (${SPetAssist}) /varset SPetAssist 1
   /if (${Melee.Combat}) /melee reset
   /varset SelfShrinkTries 0
   /delay 600s ${Me.ID}
   /delay 5s
   /if (${DoAfk} && !${Me.AFK}) /call AFK
   /varset ACStartY ${Me.Y}
   /varset ACStartX ${Me.X}
   /varset ACState 0
   /varset ACMATarget 0
   /varset ADMobFound FALSE
   /varset ADCharmID 0
   /call ProcessExcludeList zone
   /varset ZoneName ${Zone.Name}
   /if (${ADCount}) {
      /for a 1 to ${ADCount}
         /if (${Ini["${MBCommon}","${Spell[${ADSpell[${a}]}].Name}","${Zone.ShortName}Immune"].Find[|]}) {
            /call LoadINIVar "${Spell[${ADSpell[${a}]}].Name}" "${Zone.ShortName}Immune" " " "Imm${Spell[${ADSpell[${a}]}].ID}" "${MBCommon}" string
         } else /if (${Defined[Imm${Spell[${ADSpell[${a}]}].ID}]}) {
            /if (${Imm${Spell[${ADSpell[${a}]}].ID}.Length}) /varset Imm${Spell[${ADSpell[${a}]}].ID}
         }
      /next a
   }
   /doevents flush
   /squelch /alert clear 25
/return


Sub Event_Slain
   /docommand /${BeepCommand}
   /bc Msg: DEATH DETECTED, putting all functions on hold. -
   /bc Msg: You must issue the appropriate command to restart each function or restart the macro. -
   /varset DoDebuffs FALSE
   /varset DoEvents FALSE
   /varset DoHeals FALSE
   /varset DoMelee FALSE
   /varset DoBuffs FALSE
   /varset DoCures FALSE
   /varset DoPull FALSE
   /consent group
   /delay 3s
   /consent raid
   /delay 3s
   /consent guild
   /stick off
   /varset FollowFlag 0
   /nomodkey /keypress left
   /nomodkey /keypress right
   /doevents flush
   /call Wait4Rez
   /delay 600s ${Me.ID}
   /delay 6s
   /delay 20s ${Cast.Status.Equal[i]}
/return


Sub Wait4Rez
   /declare rezPct int local
   /declare callFlag bool local
   /declare RezWaitTimer timer ${RezGiveUpSeconds}s
   /stick off
   /makecamp off
   /delay 5s
   /if (!${DoRez}) {
      /bc Msg: DoRez is FALSE. Taking no actions.
      /return
   }
   /if (${RezMinPct}>100 && !${RezAcceptCall}) {
      /bc Msg: Rez settings do not allow for auto-accepting of rez/call. Taking no actions.
      /return
   }
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Awaiting rez
   /if (${RezMinPct}<101) /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Auto-accepting rez of ${RezMinPct}% and above
   /if (${RezAcceptCall}) /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Auto-accepting Call to Corpse for rez later
:checkforrez 
   /if (!${Window[ConfirmationDialogBox].Open} && !${RezWaitTimer}) {
      /bc Msg: Rez timer expired. Camping to give time to rez me later.
      /ini "${IniFile}" "Settings" "DeathSlot" "TRUE"
      /camp desk
      /delay 5s
      /endmacro
   }
   /delay 10s ${Window[ConfirmationDialogBox].Open} 
   /if (!${Window[ConfirmationDialogBox].Open}) /goto :checkforrez
   /delay 1s
   /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(0 percent)"]}]}) {
       /varset rezPct 0
       /varset callFlag FALSE
   } else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(10 percent)"]}]}) { 
       /varset rezPct 10
       /varset callFlag FALSE
   } else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(20 percent)"]}]}) { 
       /varset rezPct 20
       /varset callFlag FALSE
   } else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(35 percent)"]}]}) { 
       /varset rezPct 35
       /varset callFlag FALSE
   } else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(50 percent)"]}]}) { 
       /varset rezPct 50
       /varset callFlag FALSE
   } else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(60 percent)"]}]}) { 
       /varset rezPct 60
       /varset callFlag FALSE
   } else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(75 percent)"]}]}) { 
       /varset rezPct 75
       /varset callFlag FALSE
   } else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(90 percent)"]}]}) { 
       /varset rezPct 90
       /varset callFlag FALSE
   } else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(96 percent)"]}]}) { 
       /varset rezPct 96
       /varset callFlag FALSE
   } else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(100 percent)"]}]}) {
       /varset rezPct 100
       /varset callFlag FALSE
   } else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["you will still be able to get a resurrection later"]}]}) {
       /varset rezPct 0
       /varset callFlag TRUE
   } else {
       /varset rezPct 0
       /varset callFlag FALSE
   }
   /if (${rezPct}>=${RezMinPct} || (${RezAcceptCall} && ${callFlag})) /goto :acceptrez
   /if (${callFlag}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Declining call to corpse. Continuing to wait.
   } else {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Declining ${rezPct}% rez. Continuing to wait.
   }
   /nomodkey /notify ConfirmationDialogBox No_Button leftmouseup	
   /goto :checkforrez
:acceptrez
   /if (${callFlag}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Accepting call to corpse.
   } else {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Accepting ${rezPct}% rez.
   }
   /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup 
   /delay 2s 
   /if (${Window[RespawnWnd].Open}) /nomodkey /notify RespawnWnd RW_OptionsList listselect 2  
   /delay 1s 
   /nomodkey /notify RespawnWnd RW_SelectButton leftmouseup 
   /delay 2s 
   /ini "${IniFile}" "Settings" "DeathSlot" "FALSE"
   /delay 3s !${Window[RespawnWnd].Open}
/return


Sub ClearCursor(string ccParm)
   /if (${Window[InventoryWindow].Open}) {
      /if (!${DoCombines} && ${ccParm.NotEqual[force]}) /return
   }
   /declare x int local
   /declare tTimer timer local 100

   /while (${Cursor.ID}) {
      /if (${Cursor.Container}) {
         /for x 1 to ${Me.NumBagSlots}
            /if (!${Me.Inventory[pack${x}].ID}) {
			   /nomodkey /itemnotify pack${x} leftmouseup
			   /break
			}
         /next x
         /if (${Cursor.Container}) /break
      } else {
         /call ItemSort
      }
	  /delay 2
      /if (!${tTimer}) /break
   }
/return


Sub Buy(string itemToBuy, int amount, string vendor)
   /if (!${Defined[Broke]}) /declare Broke bool outer FALSE
   /if (!${vendor.Length} || !${itemToBuy.Length} || ${amount}==0 || ${Broke}) /return FALSE
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Buying ${itemToBuy} from ${vendor}!
   /declare QTY int local 0
   /declare l2 int local 0
   /declare mItems int local 0
   /tar npc ${vendor}
   /delay 1s ${Target.ID}==${Spawn[Merchant].ID}
   /nomodkey /click right target
   /delay 5s ${Window[MerchantWnd].Open}
   /varset QTY ${Math.Calc[${amount}-${FindItemCount[=${itemToBuy}]}]}
   /if (${FindItemCount[=${itemToBuy}]}>= ${amount}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Done!
      /return TRUE
   }
:itemCheck
   /varset mItems ${Merchant.Items}
   /delay 2
   /if (${mItems}<${Merchant.Items}) /goto :itemCheck 
 	
   /if (!${Window[MerchantWnd].Open}) /return FALSE
   /varset l2 ${Window[MerchantWnd].Child[itemList].List[=${itemToBuy},2]}
   /if (!${l2}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}couldn't find ${itemToBuy}
      /return FALSE
   } else {
      /notify MerchantWnd itemList listselect ${l2}
      /notify MerchantWnd itemList leftmouse ${l2}
      /delay 2
   }
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Buying ${itemToBuy} Till I get ${amount}
:Loop
   /if (${Broke}) /return FALSE
   /if (${QTY}>19) {
      /buyitem 20
      /delay 1s
      /doevents
      /varset QTY ${Math.Calc[${amount}-${FindItemCount[=${itemToBuy}]}]}
      /goto :Loop
   } else {
      /if (${QTY}>0 && ${QTY}<20) {
         /buyitem ${QTY}
         /delay 1s
         /doevents
         /varset QTY ${Math.Calc[${amount}-${FindItemCount[=${itemToBuy}]}]}
      }
   }
   /if (${QTY}==0) {
      /nomodkey /notify MerchantWnd MW_Done_Button leftmouseup
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Done!
      /return TRUE
   }
   /goto :Loop
/return TRUE


Sub SellCheck(VendorName)
   /call ClearCursor force
   /if (!${VendorName.Length}) /return FALSE
   /squelch /target ${VendorName}
   /delay 3s ${Target.Name.Find[${VendorName}]} || ${Target.CleanName.Find[${VendorName}]}
   /if (!${Target.Name.Find[${VendorName}]} && !${Target.CleanName.Find[${VendorName}]} || ${Spawn[${Target.ID}].Distance3D}>30) /return FALSE
|  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}selling
|  /squelch /face
   /click right target
   /delay 2s ${Window[MerchantWnd].Open}
   /declare tPack int local
   /declare tItem int local
   /declare mItem int local
   /declare mItemName string local
   /declare sItemName string local
   /nomodkey /keypress OPEN_INV_BAGS
:loop
   /if (${Window[TradeSkillWnd].Open}) /notify TradeSkillWnd ExperimentButton leftmouseup
   /delay 4 !${Window[TradeSkillWnd].Open}
   /if (${Window[TradeSkillWnd].Open}) /goto :loop
   /varset mItem ${Merchant.Items}
   /delay 2s
   /if (${Merchant.Items}!=${mItem}) /goto :loop
   /for tPack 1 to ${Me.NumBagSlots}
      /if (${Me.Inventory[pack${tPack}].Container}) {
         /for tItem 1 to ${Me.Inventory[pack${tPack}].Container}
            /varset sItemName ${Me.Inventory[pack${tPack}].Item[${tItem}].Name}
            /if (${sItemName.NotEqual[NULL]} && ${Ini["${LootIni}","${sItemName.Left[1]}","${sItemName}"].Equal[Sell]}) {
			   /call Sell "${sItemName}" ${Me.Inventory[pack${tPack}].Item[${tItem}].ItemSlot} ${Me.Inventory[pack${tPack}].Item[${tItem}].ItemSlot2} ${Me.Inventory[pack${tPack}].Item[${tItem}].Stack}
			}
         /next tItem
         /if (${Window[Pack${tPack}].Open}) /nomodkey /itemnotify pack${tPack} rightmouseup
         /delay 3s !${Window[Pack${tPack}].Open}
      } else {
         /varset sItemName ${Me.Inventory[pack${tPack}].Name}
         /if (${sItemName.NotEqual[NULL]} && ${Ini["${LootIni}","${sItemName.Left[1]}","${sItemName}"].Equal[Sell]}) {
		    /call Sell "${sItemName}" ${Me.Inventory[pack${tPack}].ItemSlot} ${Me.Inventory[pack${tPack}].ItemSlot2} ${Me.Inventory[pack${tPack}].Stack}
	     }
      }
   /next tPack
   /for mItem 1 to ${Merchant.Items}
      /varset mItemName ${Merchant.Item[${mItem}].Name}
      /if (${Ini["${LootIni}","${mItemName.Left[1]}","${mItemName}"].EqualCS[KEEP]}) /bc Msg: ${mItemName} found on ${VendorName}
   /next mItem
   /nomodkey /notify MerchantWnd MW_Done_Button leftmouseup
   /squelch /target clear
/return TRUE


Sub Sell(string ItemToSell, int iSlot, int iSlot2, int iCount)
   /declare OldMoney int local ${Me.Cash}
   /if (!${ItemToSell.Length} || !${Window[MerchantWnd].Open}) /return
   /call ClearCursor force
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Selling: ${ItemToSell} - ${iCount}
   /if (${iSlot2}<0) {
      /nomodkey /itemnotify pack${Math.Calc[${iSlot}-22].Int} leftmouseup
   } else {
      /nomodkey /itemnotify in pack${Math.Calc[${iSlot}-22].Int} ${Math.Calc[${iSlot2}+1].Int} leftmouseup
   }
   /delay 2s ${SelectedItem.ID}
   /if (${SelectedItem.ID}==${FindItem[=${ItemToSell}].ID} && ${SelectedItem.Value}) {
      /delay 1s
      /nomodkey /shift /notify MerchantWnd MW_Sell_Button leftmouseup
      /delay 2s (${Me.Cash}!=${OldMoney})
   }
   /if (${Me.Cash}==${OldMoney}) /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Looks like we were unable to sell ${ItemToSell}
/return


Sub GetBankItem(ItemName,InStacks,int Quant)
   /if (!${ItemName.Length} || !${Window[BankWnd].Open} && !${Window[BigBankWnd].Open}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Error! Usage - Bank Window must be Open - getbank <ItemName> <Quantity> <InStacks>
      /return
   }
   /if (!${InStacks.Length}) /varset InStacks TRUE
:CursorCheck
   /if (${Cursor.ID}) /autoinv
   /if (${Cursor.ID}) /goto :CursorCheck
   /declare BankSlots int local
   /declare i int local
   /declare j int local
   /declare Gathered int local
   /if (${Window[BankWnd].Open}) {    
      /varset BankSlots 16 
   } else { 
      /if (${Window[BigBankWnd].Open}) { 
         /varset BankSlots 24 
      }
   }
   /if (${Category.Find[${ItemName}]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${ItemName} is a Category
   } else /if (!${FindItemBankCount[=${ItemName}]}) { 
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Can't Find ${ItemName} in Bank.
      /return
   }
   /call OpenAllBankContainers ${BankSlots}
   /for i 1 to ${BankSlots}
      /for j 1 to ${Me.Bank[${i}].Container}
         /if ((${Me.Bank[${i}].Item[${j}].Name.Equal[${ItemName}]} || ${Category.Find[${ItemName}]}) {
		    /if (${Ini["${LootIni}",${Me.Bank[${i}].Item[${j}].Name.Left[1]},${Me.Bank[${i}].Item[${j}].Name}].Find[${ItemName}]}) {
			   /if (!${FindItem[=${Me.Bank[${i}].Item[${j}].Name}].FreeStack} && !${Me.FreeInventory}) {
			      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}No Room for ${Me.Bank[${i}].Item[${j}].Stack} - ${Me.Bank[${i}].Item[${j}].Name}
			   } else {
                  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Getting ${Me.Bank[${i}].Item[${j}].Stack} - ${Me.Bank[${i}].Item[${j}].Name} From Bank
                  /if (${Bool[${InStacks}]}) {
                     /shiftkey /itemnotify in bank${i} ${j} leftmouseup
                  } else {
                     /ctrlkey /itemnotify in bank${i} ${j} leftmouseup
				  }
			   }
            }
            /delay 2s ${Cursor.ID}		 		
            /call ClearCursor force
:CursorClear
            /if (${Cursor.ID}) /autoinv
            /if (${Cursor.ID}) /goto :CursorClear
            /varset Gathered Gathered+1
            /if (${Quant}>0) { 
               /if (${Gathered}>=${Quant}) {
                  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Got ${Gathered} - ${ItemName}'s from Bank.
                  /return
               }
            }
         }
      /next j
   /next i
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Done getting Items  with names matching ${ItemName} from Bank.
/return


Sub OpenAllBankContainers(int BankSlots)
   /declare i int local 1
   /for i 1 to ${BankSlots} 
      /if (${Int[${Me.Bank[${i}].Container}]}) { 
         /if (!${Window[bank${i}].Open}) { 
            /nomodkey /itemnotify bank${i} rightmouseup 
            /delay 5s ${Window[bank${i}].Open} 
         } 
         /if (!${Window[bank${i}].Open}) /echo ${If[${TimeStampOnEcho},[${Time}] ,]}ERROR -- Possible problem opening bank container at # ${i}. 
         /delay 2 
      } 
   /next i 
/return 1 


Sub AutoBank(string BankerName,string ItemNames)
   /call ClearCursor force
   /if (!${BankerName.Length}) /return FALSE
   /squelch /target ${BankerName}
   /delay 3s ${Target.Name.Find[${BankerName}]} || ${Target.CleanName.Find[${BankerName}]}
   /if (!${Target.Name.Find[${BankerName}]} && !${Target.CleanName.Find[${BankerName}]} || ${Spawn[${Target.ID}].Distance3D}>30) /return FALSE

   /click right target
   /delay 2s (${Window[BankWnd].Open} || ${Window[BigBankWnd].Open})
   /if (!${Window[BankWnd].Open} && !${Window[BigBankWnd].Open}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Failure to open Bank Window
      /return FALSE
   }

   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}AutoBanking
   /declare a int local 0
   /declare iCount int local 0
   /declare tPack int local
   /declare tItem int local
   /declare tItemID int local
   /declare tItemName string local

   /nomodkey /keypress OPEN_INV_BAGS
   /delay 1s

   /for a 1 to ${Math.Calc[${ItemNames.Count[|]}+1]}
      /if (${Category.Find[${ItemNames.Arg[${a},|]}]}) {
| Search for items marked as Category in loot.ini file.
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Searching inventory for items marked ${ItemNames.Arg[${a},|]} in your loot file.
         /for tPack 1 to ${Me.NumBagSlots}
            /if (${Me.Inventory[pack${tPack}].Container}) {
               /for tItem 1 to ${Me.Inventory[pack${tPack}].Container}
                  /varset tItemName ${Me.Inventory[pack${tPack}].Item[${tItem}].Name}
                  /if (${tItemName.NotEqual[NULL]} && ${Ini["${LootIni}","${tItemName.Left[1]}","${tItemName}"].Find[${ItemNames.Arg[${a},|]}]}) {
                     /varset tItemID ${Me.Inventory[pack${tPack}].Item[${tItem}].ID}
                     /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Found matching item - ${tItemID} : ${tItemName}
                     /call BankIt "${tItemID}" ${Me.Inventory[pack${tPack}].Item[${tItem}].ItemSlot} ${Me.Inventory[pack${tPack}].Item[${tItem}].ItemSlot2}
                     /if (${Macro.Return.Equal[false]}) /return FALSE
                  }
               /next tItem
            } else {
               /varset tItemName ${Me.Inventory[pack${tPack}].Name}
               /if (${tItemName.NotEqual[NULL]} && ${Ini["${LootIni}","${tItemName.Left[1]}","${tItemName}"].Find[${ItemNames.Arg[${a},|]}]}) {
                  /varset tItemID ${Me.Inventory[pack${tPack}].ID}
                  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Found matching item - ${tItemID} : ${tItemName}
                  /call BankIt "${tItemID}" ${Me.Inventory[pack${tPack}].ItemSlot} ${Me.Inventory[pack${tPack}].ItemSlot2}
                  /if (${Macro.Return.Equal[false]}) /return FALSE
               }
            } 
         /next tPack       
      } else {
         /varset tItemID ${FindItem[=${ItemNames.Arg[${a},|]}].ID}
         /varset tItemName ${FindItem[=${ItemNames.Arg[${a},|]}].Name}
         /if (${tItemID}) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}AutoBanking - ${ItemNames.Arg[${a},|]}
:aItemFind
            /call BankIt "${tItemID}" ${FindItem[=${ItemNames.Arg[${a},|]}].ItemSlot} ${FindItem[=${ItemNames.Arg[${a},|]}].ItemSlot2}
            /if (${Macro.Return.Equal[false]}) /return FALSE
            /if (${FindItemCount[=${ItemNames.Arg[${a},|]}]}>0) /goto :aItemFind
         } else {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}No ${ItemNames.Arg[${a},|]}'s found
         }
      }
   /next a
   /nomodkey /keypress CLOSE_INV_BAGS
   /nomodkey /keypress esc
   /delay 1s
/return TRUE


Sub BankIt(int iID, int iSlot, int iSlot2)
   /if (${iSlot2}<0) {
      /nomodkey /shift /itemnotify pack${Math.Calc[${iSlot}-22].Int} leftmouseup
   } else {
      /nomodkey /shift /itemnotify in pack${Math.Calc[${iSlot}-22].Int} ${Math.Calc[${iSlot2}+1].Int} leftmouseup
   }
:AutoIt
   /if (${Window[BankWnd].Open}) {    
      /notify BankWnd bw_autobutton leftmouseup
   } else { 
      /if (${Window[BigBankWnd].Open}) { 
         /notify BigBankWnd bigb_autobutton leftmouseup 
      }
   } 
   /delay 1s !${Cursor.ID}
   /if (${Cursor.ID}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Failure to Bank - ${Cursor.Name}
      /return FALSE
   }
/return TRUE


Sub GiveCheck(string TargetName,string ItemNames,string ItemCount,string InStacks)
|  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${TargetName} : ${ItemNames} : ${ItemCount} : ${InStacks}
   /if (!${TargetName.Length} || !${Spawn[${TargetName}].ID} || !${ItemNames.Length}) /return FALSE
   /declare tID int local ${Spawn[${TargetName}].ID}
   /declare a int local
   /declare b int local
   /declare NumItems int local 1
   /declare tPack int local
   /declare tItem int local
   /declare tItemID int local
   /declare tItemName string local
   /declare sItemName string local
   /declare tStacks bool local TRUE
   /declare sCount int local
   /declare tCount int local
   /declare startPack int local
   /declare endPack int local
   /if (!${Defined[gx]}) /declare gx int outer
   /if (!${Defined[gy]}) /declare gy int outer
   /if (!${Defined[gCount]}) /declare gCount int outer
   /if (!${Defined[gPetToys]}) /declare gPetToys bool outer

   /varset gx ${Me.X.Int}
   /varset gy ${Me.Y.Int}
   /varset gCount 0

   /varset a ${Math.Calc[${ItemNames.Count[|]}+1]}
   /varset b ${Math.Calc[${ItemCount.Count[|]}+1]}
   /if (${b}>${a} || (${b}>1 && ${b}!=${a})) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}GiveCheck Parameter mismatch - Aborting
      /return FALSE
   }
   /if (${InStacks.Length} && ${Select[${InStacks},false,no,nostacks]}) /varset tStacks FALSE

:GTargetCheck
   /target id ${tID}
   /delay 2s ${Target.ID}
   /if (${Target.ID}!=${tID}) /goto :tEnd
   /if (${Target.Distance3D}>200) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}GiveCheck - Target is too far away
      /return FALSE
   }
   /if (${Target.Distance3D}>12) {
      /call MBMoveTo ${Target.Y} ${Target.X}
      /if (!${Macro.Return.Equal[TRUE]}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}GiveCheck - Problem getting to ${TargetName}
         /return FALSE
      }
   }
   /if (${ItemCount.Length} && ${ItemCount.Equal[pettoys]}) {
      /if (${a}>1 || ${b}>1) /return
      /if (!${InStacks.Length}) /return
      /varset gPetToys TRUE
      /varset ItemCount 1
      /varset startPack ${InStacks}
      /varset endPack ${InStacks}
   } else {
      /varset gPetToys FALSE
      /varset startPack 1
      /varset endPack ${Me.NumBagSlots}
      /call ClearCursor force
      /if (${Target.ID}!=${tID}) {
         /squelch /target id ${tID}
         /delay 1s ${Target.ID}==${tID}
      }
      /nomodkey /keypress OPEN_INV_BAGS
      /delay 2s
   }

   /for a 1 to ${Math.Calc[${ItemNames.Count[|]}+1]}
      /if (!${gPetToys} && ${Category.Find[${ItemNames.Arg[${a},|]}]}) {
| Search for items marked as Category in loot.ini file.
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Searching inventory for items marked ${ItemNames.Arg[${a},|]} in your loot file.
         /for tPack 1 to ${Me.NumBagSlots}
            /if (${Me.Inventory[pack${tPack}].Container}) {
               /for tItem 1 to ${Me.Inventory[pack${tPack}].Container}
                  /varset tItemName ${Me.Inventory[pack${tPack}].Item[${tItem}].Name}
                  /if (${tItemName.NotEqual[NULL]} && ${Ini["${LootIni}","${tItemName.Left[1]}","${tItemName}"].Find[${ItemNames.Arg[${a},|]}]}) {
                     /varset tItemID ${Me.Inventory[pack${tPack}].Item[${tItem}].ID}
                     /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Found matching item - ${tItemID} : ${tItemName}
                     /nomodkey /shift /itemnotify in pack${Math.Calc[${Me.Inventory[pack${tPack}].Item[${tItem}].ItemSlot}-22].Int} ${Math.Calc[${Me.Inventory[pack${tPack}].Item[${tItem}].ItemSlot2}+1].Int} leftmouseup
                     /delay 2s ${Cursor.ID}==${tItemID}
                     /call Give ${tItemID} ${tID}
                     /if (${Macro.Return.Equal[false]}) {
                        /call GiveCleanup
                        /return FALSE
                     }
                  }
               /next tItem
            } else {
               /varset tItemName ${Me.Inventory[pack${tPack}].Name}
               /if (${tItemName.NotEqual[NULL]} && ${Ini["${LootIni}","${tItemName.Left[1]}","${tItemName}"].Find[${ItemNames.Arg[${a},|]}]}) {
                  /varset tItemID ${Me.Inventory[pack${tPack}].ID}
                  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Found matching item - ${tItemID} : ${tItemName}
                  /nomodkey /shift /itemnotify pack${Math.Calc[${Me.Inventory[pack${tPack}].ItemSlot}-22].Int} leftmouseup
                  /delay 2s ${Cursor.ID}==${tItemID}
                  /call Give ${tItemID} ${tID}
                  /if (${Macro.Return.Equal[false]}) {
                     /call GiveCleanup
                     /return FALSE
                  }
               }
            } 
         /next tPack       
      } else {
         /if (${b}==1) {
            /if (${ItemCount.Length}) {
               /if (${ItemCount.Find[all]}) {
                  /varset NumItems ${FindItemCount[=${ItemNames.Arg[${a},|]}]}
               } else {
                  /varset NumItems ${ItemCount}
               }
            } else {
               /varset NumItems 1
            }
         } else {
            /if (${ItemCount.Arg[${a},|].Length}) {
               /if (${ItemCount.Arg[${a},|].Find[all]}) {
                  /varset NumItems ${FindItemCount[=${ItemNames.Arg[${a},|]}]}
               } else {
                  /varset NumItems ${ItemCount.Arg[${a},|]}
               }
            } else {
               /varset NumItems 1
            }
         }
         /for tPack ${startPack} to ${endPack}
            /if (${Me.Inventory[pack${tPack}].Container}) {
               /for tItem 1 to ${Me.Inventory[pack${tPack}].Container}
                  /varset tItemName ${Me.Inventory[pack${tPack}].Item[${tItem}].Name}
                  /if (${tItemName.NotEqual[NULL]} && ${tItemName.Equal[${FindItem[=${ItemNames.Arg[${a},|]}].Name}]}) {
                     /varset tItemID ${Me.Inventory[pack${tPack}].Item[${tItem}].ID}
                     /if (!${tStacks}) {
                        /call PickUpItem ${tItemID} ${Me.Inventory[pack${tPack}].Item[${tItem}].ItemSlot} ${Me.Inventory[pack${tPack}].Item[${tItem}].ItemSlot2} 1
                     } else /if (${Me.Inventory[pack${tPack}].Item[${tItem}].Stack}<=${NumItems}) {
                        /call PickUpItem ${tItemID} ${Me.Inventory[pack${tPack}].Item[${tItem}].ItemSlot} ${Me.Inventory[pack${tPack}].Item[${tItem}].ItemSlot2} ${Me.Inventory[pack${tPack}].Item[${tItem}].Stack}
                     } else {
                        /call PickUpItem ${tItemID} ${Me.Inventory[pack${tPack}].Item[${tItem}].ItemSlot} ${Me.Inventory[pack${tPack}].Item[${tItem}].ItemSlot2} ${NumItems}
                     }
                     /if (${Macro.Return.Equal[false]}) {
                        /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with GiveCheck - PickUpItem failed
                        /call GiveCleanup
                        /return FALSE
                     }
                     /varset sCount ${Cursor.Stack}
                     /varcalc NumItems ${NumItems}-${Cursor.Stack}
                     /call Give ${tItemID} ${tID}
                     /if (${Macro.Return.Equal[false]}) {
                        /call GiveCleanup
                        /return FALSE
                     }
                  }
                  /if (${NumItems}<=0) /break
               /next tItem
            } else {
               /varset tItemName ${Me.Inventory[pack${tPack}].Name}
               /if (${tItemName.NotEqual[NULL]} && ${tItemName.Equal[${FindItem[=${ItemNames.Arg[${a},|]}].Name}]}) {
                  /varset tItemID ${Me.Inventory[pack${tPack}].ID}
                  /if (!${tStacks}) {
                     /call PickUpItem ${tItemID} ${Me.Inventory[pack${tPack}].ItemSlot} ${Me.Inventory[pack${tPack}].ItemSlot2} 1
                  } else /if (${Me.Inventory[pack${tPack}].Stack}<=${NumItems}) {
                     /call PickUpItem ${tItemID} ${Me.Inventory[pack${tPack}].ItemSlot} ${Me.Inventory[pack${tPack}].ItemSlot2} ${Me.Inventory[pack${tPack}].Stack}
                  } else {
                     /call PickUpItem ${tItemID} ${Me.Inventory[pack${tPack}].ItemSlot} ${Me.Inventory[pack${tPack}].ItemSlot2} ${NumItems}
                  }
                  /if (${Macro.Return.Equal[false]}) {
                     /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with GiveCheck - PickUpItem failed
                     /call GiveCleanup
                     /return FALSE
                  }
                  /varcalc sCount ${Cursor.Stack}
                  /varcalc NumItems ${NumItems}-${Cursor.Stack}
                  /call Give ${tItemID} ${tID}
                  /if (${Macro.Return.Equal[false]}) {
                     /call GiveCleanup
                     /return FALSE
                  }
               }
            }
            /if (${NumItems}<=0) /break 
         /next tPack       
      }
   /next a
   /call DoGive
   /if (${gPetToys}) {
      /delay 5
      /if (${Target.Type.Equal[pet]} && ${Cursor.ID} && (${Cursor.NoRent} || ${Cursor.Name.Find[muzzle]} || ${Cursor.Name.Find[visor]} || ${Cursor.Name.Find[belt]})) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Pet didn't need ${Cursor.Name} - Destroying
         /destroy
         /delay 10
      }
   } else {
      /call GiveCleanup
   }
/return TRUE


Sub Give(int iItemID,int tID)
   /if (${Cursor.ID}!=${iItemID}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with Give - Cursor check
      /return FALSE
   }
   /if (!${tID}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with Give - No Target
      /return FALSE
   }
   /if (${Target.ID}!=${tID}) {
      /squelch /target id ${tID}
      /delay 1s ${Target.ID}==${tID}
      /if (${Target.ID}!=${tID}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with Give - Target check
         /return FALSE
      }
   }
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Giving ${Cursor.Stack} ${Cursor.Name} -to- ${Target.Name}
   /if (${Target.ID}) /click left target
   /delay 10s (${Window[GiveWnd].Open} || ${Window[TradeWnd].Open})
   /if ((!${Target.Type.Equal[pc]} && !${Window[GiveWnd].Open}) || (${Target.Type.Equal[pc]} && !${Window[TradeWnd].Open})) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with Give - Window/type check
      /return FALSE
   }
   /varcalc gCount ${gCount}+1
   /if (${Window[GiveWnd].Open}) {
      /if (${gCount}<4) {
         /return TRUE
      }
      /call DoGive
   } else /if (${Window[TradeWnd].Open}) {
      /if (${gCount}<8) {
         /return TRUE
      }
      /call DoGive
   }
   /varset gCount 0
/return TRUE


Sub DoGive
   /if (${gCount}) {
      /if (${Window[GiveWnd].Open}) {
         /notify GiveWnd GVW_Give_Button LeftMouseUp
         /delay 5s !${Window[GiveWnd].Open}
      }
      /if (${Window[TradeWnd].Open}) {
         /if (!${Window[TradeWnd].MyTradeReady}) {
            /delay 2
            /notify TradeWnd TRDW_Trade_Button LeftMouseUp
         }
         /delay 15s !${Window[TradeWnd].Open}
      } 
   }
/return


Sub GiveCleanup
   /nomodkey /keypress CLOSE_INV_BAGS
   /nomodkey /keypress esc
   /delay 1s
   /if (!${Defined[gx]} || !${Defined[gy]}) /return
   /if (${Me.X.Int}!=${gx} || ${Me.Y.Int}!=${gy}) /call MBMoveTo ${gy} ${gx}
/return


Sub PickUpItem(int iID, int iSlot, int iSlot2, int iCount)
   /if (${iSlot2}<0) {
      /if (${iCount}==${Me.Inventory[pack${Math.Calc[${iSlot}-22].Int}].Stack}) {
         /nomodkey /shift /itemnotify pack${Math.Calc[${iSlot}-22].Int} leftmouseup
         /goto :pCursorCheck
      } else {
         /nomodkey /itemnotify pack${Math.Calc[${iSlot}-22].Int} leftmouseup
      }
   } else {
      /if (${iCount}==${Me.Inventory[pack${Math.Calc[${iSlot}-22].Int}].Item[${Math.Calc[${iSlot2}+1].Int}].Stack}) {
         /nomodkey /shift /itemnotify in pack${Math.Calc[${iSlot}-22].Int} ${Math.Calc[${iSlot2}+1].Int} leftmouseup
         /goto :pCursorCheck
      } else {
         /nomodkey /itemnotify in pack${Math.Calc[${iSlot}-22].Int} ${Math.Calc[${iSlot2}+1].Int} leftmouseup
      }
   }
   /delay 2s ${Window[QuantityWnd].Open}
   /if (!${Window[QuantityWnd].Open}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with PickUpItem - Quantity Window check
      /return FALSE
   }
   /nomodkey /notify QuantityWnd QTYW_Slider newvalue ${iCount}
   /delay 5
   /nomodkey /notify QuantityWnd QTYW_Accept_button leftmouseup
:pCursorCheck
   /delay 2s ${Cursor.ID}==${iID}
   /if (${Cursor.ID}!=${iID} || ${Cursor.Stack}!=${iCount}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with PickUpItem - Cursor check
      /return FALSE
   }
/return TRUE


Sub MBScript(P0,P1,P2)
|  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${P0} ${P1} ${P2}
  /declare a int local
  /declare b int local
  /declare c int local
  /declare Timer timer local
  /declare sVarL string local
  /declare sVarC string local
  /declare sVarM string local
  /declare ScriptINI string local ${IniFile}
  /if (!${P0.Length}) {
    /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Syntax Error
    /return
  }
  /if (${Select[${P0.Arg[1,-]},net,network]}) {
    /varset ScriptINI ${NetworkINI}
    /if (!${P0.Find[ ]}) {
      /varset P0 ${P0.Right[${Math.Calc[${P0.Length}-${P0.Find[-]}]}]}
    } else /varset P0 ${P0.Arg[2]}
  }
  /if (!${Defined[${P0}C]}) {
    /declare ${P0}C int outer ${Ini["${ScriptINI}",Script-${P0},Commands]}
    /if (!${${P0}C}) {
       /declare IniString string local ${Ini["${ScriptINI}",Script-${P0},Commands,NOTFOUND]}
       /if (${IniString.Equal[NOTFOUND]}) {
         /ini "${ScriptINI}" "Script-${P0}" "Commands" "0"
         /ini "${ScriptINI}" "Script-${P0}" "C1" "/return"
         /if (!${Select[${P0},MBStartup,Defense]}) /echo ${If[${TimeStampOnEcho},[${Time}] ,]}MBScript - INI Entry Made for: Script-${P0}
       }
       /return
    }
    /declare ${P0}[${${P0}C}] string outer
    /for a 1 to ${${P0}C}
      /varset ${P0}[${a}] ${Ini["${ScriptINI}",Script-${P0},C${a}]}
    /next a
  }
  /if (!${${P0}C}) /return
  /if (${P0.Equal[Defense]}) {
     /if (${P1.Equal[Startup]}) /return 
  }
  /varset Timer 15s
:Top
  /for a 1 to ${${P0}C}
    /varset sVarC
    /for b 1 to ${${P0}[${a}].Count[{]}
      /varset sVarL ${${P0}[${a}].Arg[${b},{]}
      /varset sVarC ${sVarC}${sVarL}${
    }
    /next b
    /varset sVarC ${sVarC}${${P0}[${a}].Arg[${b},{]}
    /if (!${${P0}[${a}].Count[{]}) /varset sVarC ${${P0}[${a}]}
    /varset sVarM
    /for b 1 to ${sVarC.Count[']}
      /if (${sVarC.Find['']}) {
        /varset sVarL ${sVarC.Mid[1,${Math.Calc[${sVarC.Find['']}-1]}]}"
| "
        /varset sVarC ${sVarC.Mid[${Math.Calc[${sVarC.Find['']}+2]},${sVarC.Length}]}
        /varset sVarM ${sVarM}${sVarL}
      }
    /next b
    /varset sVarM ${sVarM} ${sVarC}
    /if (${sVarM.Length}) /varset sVarC ${sVarM}
    /if (${EchoScripts}) {
	   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}/docommand ${sVarC}
	}
    /docommand ${sVarC}
    /if (!${Timer}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Script [${P0}] took too long, exiting
      /return
    }
  /next a
/return


Sub CastCall(CCMsgFrom,CCMsgTxt)
|/echo Msg: ${CCMsgFrom} - ${CCMsgTxt}
   /declare a int local	1
   /declare b int local	1
   /declare c int local	1
   /declare d int local	1
   /declare amax int local 5
   /declare bmin int local 1
   /declare bmax int local 1
   /declare cmax int local 1
   /if (${CCMsgTxt.Arg[1].Equal[coresist]}) {
      /declare tCastClass string local AD AH AB AQ AE
   } else {
      /declare tCastClass string local AH AD AB AQ AE
   } 
   /declare tCastCnt int local 0
   /declare tGrpMbr int local 0
   /declare sGem string local
   /declare sReturn string local
   /declare tGem int local 0
   /declare tSpell string local
   /declare tSpellID int local 0
   /declare tSpellRange int local 0
   /declare tCastTime timer local 0
   /declare tCT int local
   /declare tTargetID int local 0
   /declare CCtTargetID int local 0
   /declare tCCMsgTxt string local
   /declare ProjectIllu bool local FALSE
   /declare tFoci string local
   /declare CastCount int local 0
   /declare tCCMsgArg1 string local ${CCMsgTxt.Arg[1]}
   /declare tTwistGems string local
   /declare tSCast bool local FALSE
   /declare sNBClnt string local ${NetBots.Client}
   /declare tPrevTarget int local

   /if (${Me.Class.ShortName.Equal[brd]}) {
      /varset tTwistGems ${Twist.List}
   }
   /if (${CCMsgTxt.Arg[1].Equal[scast]}) {
      /varset tCastClass ${CCMsgTxt.Arg[2].Arg[1,|]}
      /if (!${Select[${tCastClass},AD,AH,AB,AQ,AE]}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}CastCall - Bad Class on scast - ${tCastClass}
         /return
      }
      /varset amax 1
      /varset sGem ${CCMsgTxt.Arg[2].Arg[2,|]}
      /if (!${${tCastClass}Gem[${sGem}].Length}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}CastCall - Bad Item on scast - ${sGem}
         /return
      }
      /varset bmin ${sGem}
      /varset bmax ${sGem}
      /varset cmax 1
      /varset tSCast TRUE
   }
:AliasLoop
   /varset tGrpMbr 0
   /varset CastCount 0    
   /for a 1 to ${amax}
      /if (${${tCastClass.Arg[${a}]}Count}) {
         /if (!${tSCast}) {
		    /varset bmax ${${tCastClass.Arg[${a}]}Count}
		 }
         /for b ${bmin} to ${bmax}
            /if (!${tSCast}) {
			   /varset cmax ${Math.Calc[${${tCastClass.Arg[${a}]}SpellAlias[${b}].Count[|]}+1]}
			}
            /for c 1 to ${cmax}
|			   /if (${NetBots[${Me.CleanName}].Silenced} || ${NetBots[${Me.CleanName}].Charmed} || ${Select[${Me.State},STUN,HOVER,DEAD]}) /return
               /if (${tSCast} || ${CCMsgTxt.Arg[2].Equal[${${tCastClass.Arg[${a}]}SpellAlias[${b}].Arg[${c},|]}]}) {
                  /varset sReturn ${tCastClass.Arg[${a}]} ${b}
                  /varset tSpell ${${tCastClass.Arg[${a}]}Spell[${b}]}
                  /varset sGem ${${tCastClass.Arg[${a}]}Gem[${b}]}
                  /varset tFoci ${${tCastClass.Arg[${a}]}SpellFoci[${b}]}
                  /if (${tCastClass.Arg[${a}].NotEqual[ab]}) {
                     /if (!${MasterList.Find[${CCMsgFrom}]} && ${CCMsgFrom.NotEqual[${Me.CleanName}]} && (!${MasterList.Equal[Netbots]} && ${NetBots.Client.Find[${CCMsgFrom}]})) /return
                  }
                  /if (${CCMsgTxt.Arg[3].Equal[off]}) {
                     /varset ${tCastClass.Arg[${a}]}TarCnt[${b}] 0
                     /bc Msg: ${CCMsgTxt.Arg[2]}-${tSpell} is Off
                     /if (${b}<${bmax}) {
                        /varcalc CastCount ${CastCount}+1
                        /continue
                     }
                     /if (${Me.Class.ShortName.NotEqual[brd]}) /return SUCCESS
                     /varset CCMsgTxt sung ${${tCastClass.Arg[${a}]}SpellAlias[${b}].Arg[${c},|]}
                  }
                  /if (${CCMsgTxt.Arg[3].Equal[on]}) {
                     /if (${MasterList.Find[${CCMsgFrom}]} || ${MasterList.Find[Netbots]} && ${NetBots[${CCMsgFrom}].ID} || ${CCMsgFrom.Equal[${Me.CleanName}]}) {
                        /if (${${tCastClass.Arg[${a}]}NewFormat}) {
                           /call LoadINIVar ${tCastClass.Arg[${a}]}${b} TarCnt 0 ${tCastClass.Arg[${a}]}TarCnt[${b}] ${IniFile}
                        } else {
                           /call LoadINIVar ${If[${tCastClass.Arg[${a}].Equal[ah]},AdvHeal,${If[${tCastClass.Arg[${a}].Equal[ab]},AdvBuff,${If[${tCastClass.Arg[${a}].Equal[ad]},AdvDebuff,${If[${tCastClass.Arg[${a}].Equal[ae]},AdvEvent,${If[${tCastClass.Arg[${a}].Equal[aq]},AdvCure,NOTFOUND]}]}]}]}]} ${tCastClass.Arg[${a}]}TarCnt${b} 0 ${tCastClass.Arg[${a}]}TarCnt[${b}] ${IniFile}
                        }
                        /if (${${tCastClass.Arg[${a}]}TarCnt[${b}]}==0) /varset ${tCastClass.Arg[${a}]}TarCnt[${b}] 1
                        /bc Msg: ${CCMsgTxt.Arg[2]}-${tSpell} is On
                        /if (${b}<${bmax}) {
                           /varcalc CastCount ${CastCount}+1
                           /continue
                        }
                        /return SUCCESS
                     }
                  }
                  /varset tGem 0
                  /if (${Select[${sGem},${ValidSpellGems}]}) {
|Spell
                     /varset tSpellID ${Me.Book[${Me.Book[${tSpell}]}].ID}
                     /if (${Me.CurrentMana}<${Spell[${tSpellID}].Mana}) /goto :CantCast
                     /varset tGem ${sGem}
                     /varcalc tCT ${Spell[${tSpellID}].MyCastTime}/100+50.99
                     /if (${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) /delay 2s !${Me.Casting.ID}
                     /if (!${Me.Gem[${tGem}].Name.Length} || ${Me.Gem[${tGem}].Name.NotEqual[${tSpell}]}) {
                        /call MemGem ${tGem} "${tSpell}"
                        /if (${Macro.Return.NotEqual[TRUE]}) {
                           /goto :CantCast
                        }
                        /varcalc tCT ${Spell[${tSpellID}].RecastTime}/100+${tCT}
                     } else /if (!${Cast.Ready[${tSpellID}|gem${tGem}]}) {
                        /varcalc tCT ${Spell[${tSpellID}].RecastTime}/100+${tCT}
                     }
                     /if (${Me.Class.ShortName.Equal[brd]}) {
                        /if (!${CCMsgTxt.Find[sing]} && !${CCMsgTxt.Find[sung]} && (${MBPause} || !${tTwistGems.Length})) {
                           /varset tTwistGems ${tGem}
                        } else {
                           /if (${CCMsgTxt.Find[sing]}) {
                              /varset ${sReturn.Arg[1]}TarCnt[${sReturn.Arg[2]}] 1
                              /if (${Spell[${tSpellID}].SpellType.Equal[detrimental]}) {
                                 /call EditTwist "${BardCombatGems}" ${tGem} add
                                 /varset BardCombatGems ${Macro.Return}
                              } else {
                                 /call EditTwist "${BardRestGems}" ${tGem} add
                                 /varset BardRestGems ${Macro.Return}
                              }
                           } else /if (${CCMsgTxt.Find[sung]}) {
                              /varset ${sReturn.Arg[1]}TarCnt[${sReturn.Arg[2]}] 0
                              /call EditTwist "${BardCombatGems}" ${tGem} delete
                              /varset BardCombatGems ${Macro.Return}
                              /call EditTwist "${BardRestGems}" ${tGem} delete
                              /varset BardRestGems ${Macro.Return}
                              /call EditTwist "${tTwistGems}" ${tGem} delete
                              /varset tTwistGems ${Macro.Return}
                              /varset tSpell Removed [${Spell[${tSpellID}].Name}] from list and Tarcnt=0(off).
                              /varcalc tCT 0
                              /goto :tContCast
                           }
                        }
                     }
                  } else /if (${sGem.Equal[item]} && ${FindItem[=${tSpell}].ID}) {
|Item
                     /varset tSpellID ${FindItem[=${tSpell}].Spell.ID}
                     /if (${FindItem[=${tSpell}].Timer}) /goto :CantCast
                     /varset tGem 20
                     /varcalc tCT ${FindItem[=${tSpell}].CastTime}/100+50.99
                     /if (${tSpell.Equal[Fellowship Registration Insignia]}) /makemevisible
                  } else /if (${sGem.Equal[alt]} && ${Me.AltAbility[${tSpell}].ID}) {
|Alt
                     /varset tSpellID ${Me.AltAbility[${tSpell}].Spell.ID}
                     /if (!${Me.AltAbilityReady[${tSpell}]}) /goto :CantCast
                     /varset tGem 30
                     /varcalc tCT ${Me.AltAbility[${tSpell}].Spell.CastTime}/100+20.99
                  } else /if (${sGem.Equal[script]}) {
|Script
                     /call MBScript "${tSpell}"
                     /return
                  } else /if (${sGem.Equal[disc]} && ${Me.CombatAbility[${tSpell}]}) {
|Disc
                     /varset tSpellID ${Spell[${tSpell}].ID}
                     /if (${Me.CurrentEndurance} < ${Spell[${tSpell}].EnduranceCost}) || !${Me.CombatAbilityReady[${tSpell}]}) /goto :CantCast
                     /varset tGem 50
                     /varcalc tCT ${Spell[${tSpellID}].MyCastTime}/100+20.99
                  }
                  /if (!${tGem}) /goto :CantCast
                  /varset tSpellRange ${Spell[${tSpellID}].Range}
                  /varset tTargetID 0
                  /if (${CCMsgTxt.Arg[3].Length}) {
                     /if (${CCMsgTxt.Arg[3].Find[%t]} && ${NetBots[${CCMsgFrom}].TargetID}) {
                        /varset tTargetID ${NetBots[${CCMsgFrom}].TargetID}
                     } else /if (${CCMsgTxt.Arg[3].Find[self]}) {
                        /varset tTargetID ${Me.ID}
                     } else /if (${sNBClnt.Find[${CCMsgTxt.Arg[3]}]}) {
                        /varset tTargetID ${Spawn[pc ${CCMsgTxt.Arg[3]}].ID}
                     } else /if (${CCMsgTxt.Arg[4].Equal[warder]} && ${Spawn[${CCMsgTxt.Arg[3].Left[-2]}].Pet.Name.Find[Warder]}) {
                        /varset tTargetID ${Spawn[pc ${CCMsgTxt.Arg[3].Left[-2]}].Pet.ID}
                     } else /if (${Spawn[${CCMsgTxt.Arg[3]}].ID} && ${CCMsgTxt.Arg[3].Equal[${Spawn[${CCMsgTxt.Arg[3]}].Name}]}) {
                        /varset tTargetID ${Spawn[${CCMsgTxt.Arg[3]}].ID}
                     } else /if (${Spawn[${CCMsgTxt.Arg[3]}].ID} && ${CCMsgTxt.Arg[3].Equal[${Spawn[${CCMsgTxt.Arg[3]}].ID}]}) {
                        /varset tTargetID ${Spawn[${CCMsgTxt.Arg[3]}].ID}
                     } else /if (${CCMsgTxt.Arg[3].Find[pet]} && ${Spawn[pc ${CCMsgFrom}].Pet.ID}) {
                        /varset tTargetID ${Spawn[pc ${CCMsgFrom}].Pet.ID}
                     }
                  }
                  /if (!${tTargetID}) {
                     /varset tTargetID ${If[${Spell[${tSpellID}].SpellType.Equal[detrimental]},${NetBots[${CCMsgFrom}].TargetID},${If[${NetBots[${CCMsgFrom}].ID},${NetBots[${CCMsgFrom}].ID},${Spawn[${CCMsgFrom}].ID}]}]}
                     /if (!${tTargetID}) {
                        /if (!${NetBots[${CCMsgFrom}].ID} && (${Spell[${tSpellID}].SpellType.Equal[detrimental]} || ${CCMsgTxt.Arg[3].Find[%t]})) {
                           /target clear
                           /assist ${CCMsgFrom}
                           /delay 2s ${Target.ID}
                           /varset tTargetID ${Target.ID}
						}
				     }
                  }
:tPrepBuff
                  /if (${CCMsgTxt.Arg[3].Equal[grp]}) {
                     /varcalc tGrpMbr ${tGrpMbr}+1
                     /if (${Group.Member[${tGrpMbr}].ID}) {
                        /varset tTargetID ${Group.Member[${tGrpMbr}].ID}
                        /if (!${Spawn[${tTargetID}].ID} || ${Spawn[${tTargetID}].Distance}>${Spell[${tSpellID}].MyRange}) {
                           /bc Msg: Cast canceled for ${Spawn[${tTargetID}].Name}.  Not in range!
                           /goto :tPrepBuff
                        }
                     } else {
                        /varset tTargetID ${Me.ID}
                     }
                  }
                  /if (${Select[${Spell[${tSpellID}].Name},Gate,Mass Group Buff]} || (${Spell[${tSpellID}].TargetType.Equal[self]} && !${Me.AltAbility[Project Illusion]})) {
                     /varset tTargetID ${Me.ID}
                  } else /if (${Spell[${tSpellID}].TargetType.Equal[self]} && ${Me.AltAbility[Project Illusion]} && (${Spawn[${tTargetID}].ID} && ${tTargetID}!=${Me.ID} || ${CCMsgTxt.Arg[3].Find[%t]} || ${Spawn[${CCMsgTxt.Arg[3]}].ID})) {
                     /varset ProjectIllu TRUE
                     /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Projecting
                  }
                  /if (${Spell[${tSpellID}].TargetType.Equal[pet]}) /varset tTargetID ${Me.Pet.ID}
                  /if (!${tTargetID}) /goto :CantCast
                  /if (${Spell[${tSpellID}].TargetType.Find[single]}) {
                     /if (${Spawn[${tTargetID}].Distance3D}>${tSpellRange}) {
                        /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Out of Range
                        /goto :CantCast
                     }
                  }
                  /if (${tGem}==50) {
                     /varset CCtTargetID ${tTargetID}
                     /if (${Target.ID}!=${CCtTargetID} && ${CCtTargetID} && (${tTargetID}!=${Me.ID} && ${Spell[${tSpellID}].TargetType.NotEqual[self]})) {
                        /squelch /tar id ${CCtTargetID}
                        /delay 1s ${Target.ID}==${CCtTargetID}
                     }
                     /disc ${tSpellID}
                     /goto :CastDone
                  }
                  /varset CCtTargetID 0
                  /varset tCastTime ${tCT}
:tCasting
                  /if (${CCtTargetID}) {
                     /doevents ImHit
                     /if (${APAggro}) /goto :tContCast
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
                           /if (${Select[${CastResult},SUCCESS,IMMUNE,RESIST]}) /goto :tContCast
                           /if (${CastResult.Equal[STUNNED]}) {
                              /varset CCtTargetID 0
                              /goto :tCasting
                           }
                        }
                        /delay 2
                        /if (${Me.Casting.ID}!=${Spell[${Me.Gem[${tGem}]}].ID}) {
                           /varset CastResult SUCCESS
                           /delay 1s
                        }
                        /goto :tCasting
                     }
                     /if (${Cast.Timing} && !${Cast.Status.Equal[i]}) /goto :tCasting
                     /if (${Cast.Status.Equal[i]}) {
                        /if (${Select[${Cast.Result},cast_success,cast_resist]}) /goto :tContCast
                        /if (${Select[${Cast.Result},cast_fizzle,cast_recover]}) {
                           /varset tCastTime ${tCT}
                           /varset CCtTargetID 0
                           /goto :tCasting
                        }
                        /if (${Select[${Cast.Result},cast_immune,cast_takehold]}) {
                           /bc Msg: Target Immune or wont Take Hold
                           /goto :tContCast
                        }
| Everything else - CAST_OUTOFRANGE,CAST_CANNOTSEE,CAST_DISTRACTED,CAST_NOTARGET,CAST_INTERRUPTED,CAST_CANCELLED,CAST_NOTREADY,CAST_OUTOFMANA (and anything else we missed)
                        /goto :CantCast
                     }
                  } else /if (${Me.Class.ShortName.Equal[BRD]} || (${Cast.Ready[${tSpell}|${sGem}]} || (${Cast.Status.Equal[i]} && ${tGem}<19 && !${Me.GemTimer[${tGem}]})) && !${Spell[tSpellID].TargetType.Equal[self]}) {
                     /varset CCtTargetID ${tTargetID}
                     /if (${Target.ID}!=${CCtTargetID} && ${CCtTargetID} && (${tTargetID}!=${Me.ID} || ${Spell[${tSpellID}].TargetType.NotEqual[self]})) {
                        /varset tPrevTarget ${Target.ID}
|                       /if (${Me.Class.ShortName.Equal[BRD]}) /squelch /stoptwist
                        /if (${Melee.Combat}) /attack off
                        /squelch /tar id ${CCtTargetID}
                        /delay 1s ${Target.ID}==${CCtTargetID}
                     } else {
                        /varset tPrevTarget 0
                     }
                     /if (${Me.Class.ShortName.Equal[BRD]}) {
                        /if (${Me.State.Equal[sit]}) /stand
                        /if (${tGem}==30) {
                           /alt act ${Me.AltAbility[${tSpell}].ID}
                           /delay 2
                           /if (${tPrevTarget}) /target id ${tPrevTarget}
                           /goto :tContCast
                        } else /if (${tGem}==20) {
                           /stopsong
                           /nomodkey /useitem "${tSpell}"
                           /delay 2
                           /if (${tPrevTarget}) /target id ${tPrevTarget}
                           /delay ${Math.Calc[${tCT}-50]}
                           /goto :tContCast
                        } else {
                           /call ResetSpellResists
                           /varset CastResult PENDING
                           /squelch /twist once ${tGem}
                           /delay 5
                           /if (${tPrevTarget}) /target id ${tPrevTarget}
                           /goto :tCasting
                        }
                     } else {
                        /if (${ProjectIllu}) {
                           /casting "Project Illusion|alt" -targetid|${tTargetID}
                           /delay 1s
                        }
                        /varset tCastTime ${tCT}
                        /varset MBCurrentSpell CC|1|${tTargetID}
                        /if (${CCMsgTxt.Arg[4].Equal[CastCheck]}) {
                           /if (${tGem}>19) {
                              /call MQ2Cast "${tSpell}" ${sGem} ${tCastTime} CastCheck -targetid|${tTargetID} ${If[${tFoci.Length},-bandolier|${tFoci},]}
                           } else {
                              /call MQ2Cast "${Spell[${tSpellID}]}" gem${tGem} ${tCastTime} CastCheck -targetid|${tTargetID} ${If[${tFoci.Length},-bandolier|${tFoci},]}
                           }
                           /if (${interruptFlag}) /goto :CantCast
                        } else {  
                           /if (${tGem}>19) {
                              /call MQ2Cast "${tSpell}" ${sGem} ${tCastTime} -targetid|${tTargetID} ${If[${tFoci.Length},-bandolier|${tFoci},]}
                           } else {
                              /call MQ2Cast "${Spell[${tSpellID}]}" gem${tGem} ${tCastTime} -targetid|${tTargetID} ${If[${tFoci.Length},-bandolier|${tFoci},]}
                           }
                        }
                        /if (${Macro.Return.Equal[cast_success]}) /goto :tContCast
                     }
                  }
                  /if (${tCastTime} || ${Cast.Status.NotEqual[i]}) /goto :tCasting
                  /if (${MBDebug}) {
                     /echo ${If[${TimeStampOnEcho},[${Time}] ,]}CAST_not_i - ${Time} - ${tSpell} - ${Spell[${tSpellID}]} - ${MBCurrentSpell} - ${interruptFlag} - ${Cast.Status} - ${Cast.Result} - ${Cast.Stored} - ${NetBots[${TankName}].TargetID} - ${tTargetID} - ${tSpellID} - ${tCT} - ${tCastTime}
                  }
                  /goto :CantCast
:tContCast
                  /varset sReturn ${Cast.Result} ${sReturn} ${tSpellID} ${CCtTargetID}
                  /if (${Me.Class.ShortName.Equal[BRD]}) {
                     /if (${tTwistGems.Length}) {
                        /if (${Twist.List.NotEqual[${tTwistGems}]}) {
                           /squelch /twist ${tTwistGems}
                        }
                     } else /if (${Twist.Twisting}) {
                        /squelch /twist off
                     }
                  } 
                  /if (!${Select[${Cast.Result},cast_immune,cast_takehold]} && !${Select[${tCCMsgArg1},qcast,scast]}) {
				     /if (${tSpell.Find[from list]}) {
					    /bc Cast: -[ Bard ]- ${tSpell}
				     } else {
					    /bc Cast: -[ ${Spawn[${CCtTargetID}].CleanName} ]- with ${tSpell}
				     }
                  }
                  /if (${Cursor.ID}) /call ClearCursor
                  /doevents eqbc
                  /if (${CCMsgTxt.Arg[3].Equal[grp]} && ${Group.Member[${tGrpMbr}].ID}) {
                     /varcalc tCT ${tCT}+40
                     /goto :tPrepBuff
                  }
                  /varcalc CastCount ${CastCount}+1
                  /if (${CCMsgTxt.Arg[1].Equal[coresist]}) /goto :CastDone
               }
            /next c
         /next b
      }
   /next a
   
   /if (!${CastCount}) {
      /if (${NetBots[${CCMsgFrom}].ID}) {
	     /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Couldn't find Alias [${CCMsgTxt.Arg[2, ]}]
	  }
      /return NOALIASFND
   }   
   
   /if (${CCMsgTxt.Arg[3].Equal[on]} || ${CCMsgTxt.Arg[3].Equal[off]}) /return SUCCESS

   /if (!${tSCast}) {
      /varset d ${If[${CCMsgTxt.Arg[3].Equal[%t]} || ${CCMsgTxt.Arg[3].Equal[grp]} || ${sNBClnt.Find[${CCMsgTxt.Arg[3]}]} || ${CCMsgTxt.Arg[3].Equal[self]} || ${CCMsgTxt.Arg[3].Equal[${Spawn[${CCMsgTxt.Arg[3]}].ID}]},4,3]}
      /varset tCCMsgTxt
      /while (${CCMsgTxt.Arg[${d}].Length}) {
         /if (${CCMsgTxt.Arg[${Math.Calc[${d}+1]}].Equal[warder]}) /varcalc d ${d} + 2
         /varset tCCMsgTxt ${tCCMsgTxt} ${CCMsgTxt.Arg[${d}]}
         /varcalc d ${d}+1
		 /delay 1
	  }
      /if (${tCCMsgTxt.Length}) {
         /varset CCMsgTxt ${tCCMsgArg1} ${tCCMsgTxt}
         /goto :AliasLoop
      }
   }
:CastDone
   /if (!${Me.ID}) {
      /call Event_DelayOnZone
   }
   /varset MBCurrentSpell
   /if (${Me.Class.ShortName.Equal[BRD]} && !${MBPause}) {
      /if (${Twist.List.NotEqual[${tTwistGems}]} && ${tTwistGems.Length}) /squelch /twist ${tTwistGems}
   }
   /return ${sReturn}

:CantCast
   /if (${Me.Class.ShortName.Equal[BRD]} && !${MBPause}) {
      /if (${Twist.List.NotEqual[${tTwistGems}]} && ${tTwistGems.Length}) /squelch /twist ${tTwistGems}
   }
   /if (${Spell[${tSpellID}].SpellType.Equal[detrimental]}) {
      /if (${CCtTargetID}) {
         /if (${Spawn[${CCtTargetID}].Type.Equal[corpse]} || !${Spawn[${CCtTargetID}].ID}) /return FAIL ${sReturn}
      } else /if (${tTargetID}) {
         /if (${Spawn[${tTargetID}].Type.Equal[corpse]} || !${Spawn[${tTargetID}].ID}) /return FAIL ${sReturn}
      }
      /if (${Cast.Result.Equal[cast_notarget]} && ${Spell[${tSpellID}].TargetType.Equal[target of target]}) /return FAIL ${sReturn}
   }
   /if (!${interruptFlag}) /bc Msg: There was a problem and I can't cast ${tSpell} right now
/return FAIL ${sReturn}


Sub ProcessExcludeList(P0)
   /declare a int local
   /declare sID string local
   /if ((${P0.Length}) || !${Zone.Name.Equal[${ZoneName}]}) {
       /if (${P0.Equal[save]}) {
          /ini "${MBCommon}" "ExcludeList" "${Zone.ShortName}" "${ExcludeList}"
          /return
       } else /if (${P0.Equal[zone]} || !${Zone.Name.Equal[${ZoneName}]}) {
          /varset ExcludeList ${If[${Select[${Ini["${MBCommon}","ExcludeList","${Zone.ShortName}",NOTFOUND]},NOTFOUND,NULL]},|,${Ini["${MBCommon}","ExcludeList","${Zone.ShortName}",NOTFOUND]}]}
          /varset ExcludeListID |
          /for a 1 to ${NetBots.Counts}
             /if (${NetBots.Client.Arg[${a}].NotEqual[${Me.CleanName}]} && ${NetBots[${NetBots.Client.Arg[${a}]}].InZone}) {
                /if (${NetBots[${NetBots.Client.Arg[${a}]}].PetID} && !${Spawn[${NetBots[${NetBots.Client.Arg[${a}]}].PetID}].IsSummoned}) /varset ExcludeListID ${ExcludeListID}${NetBots[${NetBots.Client.Arg[${a}]}].PetID}|
             }
          }
       }
   }
   /if (!${ExcludeList.Left[1].Equal[|]}) /varset ExcludeList |${ExcludeList}
   /if (!${ExcludeListID.Left[1].Equal[|]}) /varset ExcludeListID |${ExcludeListID}
|  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${ExcludeList} 
   /squelch /alert clear ${AlertList}
   /squelch /alert add ${AlertList} NPC "eye of "
   /if (${ExcludeList.Length}>1) {
      /for a 1 to ${Math.Calc[${ExcludeList.Count[|]}-1]}
         /if (${ExcludeList.Arg[${a},|].Find[#]}==1) {
            /squelch /alert add ${AlertList} NPC "${ExcludeList.Arg[${a},|].Right[-1]}"
         } else {
            /squelch /alert add ${AlertList} NPC "${ExcludeList.Arg[${a},|]}"
         }
      /next a
   }
   /if (${ExcludeListID.Length}>1) {
      /for a 1 to ${Math.Calc[${ExcludeListID.Count[|]}-1]}
         /squelch /alert add ${AlertList} id "${ExcludeListID.Arg[${a},|]}"
      /next a
   }
/return


Sub Login(string name,..)
|  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${name.Arg[2, ]}
|  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${MacToRun}
   /if (!${name.Length}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Syntax error
      /return
   }
   /declare toon int local
   /if (${Defined[MacToRun]}) /deletevar MacToRun
   /if (${name.Find[|]}) {
      /declare MacToRun string global ${name.Arg[2,|]}
   }
   /bc Msg: Logging into ${name.Arg[1,|]}
   /camp
   /delay 10m ${MacroQuest.GameState.Equal[CHARSELECT]}
   /varset toon ${Window[CharacterListWnd].Child[CLW_LeftScreen].Child[CLW_CharactersScreen].Child[CLW_Character_List].List[${name.Arg[1,|]},3]}
   /notify CharacterListWnd CLW_Character_List listselect ${toon}
   /notify CharacterListWnd CLW_Character_List leftmouse ${toon}
   /notify CharacterListWnd CLW_Play_Button leftmouseup
   /delay 10m ${MacroQuest.GameState.Equal[INGAME]}
|  /delay 3m ${Me.ID}
   /if (${Zone.ShortName.Equal[Bazaar]} && ${Me.Name.Equal[${TraderName.Arg[1,|]}]} && !${Defined[MacToRun]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Trader logged in.
      /if (!${Window[BazaarWnd]}) /timed 10 /trader
      /timed 30 /if (${Window[BazaarWnd].Child[BZW_Start_Button]}) /nomodkey /notify BazaarWnd BZW_Start_Button leftmouseup
      /endmacro
   } else /if (${Zone.ShortName.Equal[Bazaar]} && ${Me.Name.Equal[${TraderName.Arg[1,|]}]} && ${Defined[MacToRun]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Trader logged in.  Starting ${MacToRun} macro.
      /timed 100 /mac ${MacToRun}
      /timed 150 /deletevar MacToRun
      /endmacro
   } else {
      /mqp
      /timed 100 /macro modbot
   }
/return


Sub ItemSort
   /if (!${Cursor.ID}) /return
   /declare DoubleCheck int local ${Cursor.ID}

   /if (!${Ini["${ForageIni}",ForageList,${Cursor.Name}].Length}) {
      /ini "${ForageIni}" "ForageList" "${Cursor.Name}" "-1"
   } else {
      /if (${Ini["${ForageIni}",ForageList,${Cursor.Name}]}) {
         /if (${Ini["${ForageIni}",ForageList,${Cursor.Name}].Equal[Ignore]}) /return
         /if (${FindItemCount["=${Cursor.Name}"]}>=${Ini["${ForageIni}",ForageList,${Cursor.Name}]} && ${Ini["${ForageIni}",ForageList,${Cursor.Name}]}!=-1 && ${Ini["${ForageIni}",ForageList,${Cursor.Name}].NotEqual[Ignore]}) /goto :Destroy
      } else {
:Destroy
         /if (${Cursor.ID} == ${DoubleCheck}) { 
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Destroying ${Cursor.Name} 
            /destroy 
         } 
         /return
      }
   }
:LootIt
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Msg: Keeping - ${Cursor.Name}
   /autoinventory
   /delay 2
/return


Sub Event_ResetMelee
   /melee reset
/return


Sub Event_WornOff(Line,SpellName,Toon)
   /if (!${Spell[${ADCharmSpell}].ID} && !${ABHasRecast}) {
      /doevents flush WornOff
      /return
   }
   /if (${SpellName.Equal[${ADCharmSpell}]}) {
      /call CharmBroke
      /return
   }
   /if (${NetBots[${Toon}].ID}) /return
   /if (!${ABHasRecast}) /return
   /declare i int local
   /declare spwnType string local
   /if ((${Toon.Find['s warder]} || ${Toon.Find['s Pet]}) && ${Spawn[Pet ${Toon}].ID}) {
      /varset spwnType Pet
   } else {
      /varset spwnType ${Spawn[${Toon}].Type}
   }
   /if (${ABCount} && ${Select[${Spawn[${Toon}].Type},PC,Pet]}) {
      /for i 1 to ${ABCount}
          /if (${SpellName.Equal[${Spell[${ABSpell[${i}]}].Name}]} && ${ABRecast[${i}]}) /call AddToQueue ${Spawn[${spwnType} ${Toon}].ID} ${i}
      /next i
   }
/return


Sub DoBuffEvents
   /if (${MBDebug}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}BE - ${Macro.RunTime} - ${MBCurrentSpell} - ${interruptFlag} - ${Cast.Status} - ${Cast.Result} - ${Cast.Stored} - ${NetBots[${TankName}].TargetID} - ${Target.ID} - ${Me.Casting.ID} - ${QueueCount}
   }
   /if (${MBPause}) /return
   /declare a int local
   /declare TempVar int local ${Math.Calc[${ABCount}+1]}
   /declare TempGuy int local 0
   /for a 1 to 25
      /if (${DoBuffQueue[${a},2]}<${TempVar} && ${Spawn[${DoBuffQueue[${a},1]}].ID} && ${Spawn[${DoBuffQueue[${a},1]}].Type.NotEqual[corpse]} && (${Spawn[${DoBuffQueue[${a},1]}].Distance}<${Me.Book[${Me.Book[${ABSpell[${DoBuffQueue[${a},2]}]}]}].MyRange} || ${Spawn[${DoBuffQueue[${a},1]}].Distance}<${Me.Book[${Me.Book[${ABSpell[${DoBuffQueue[${a},2]}]}]}].AERange})) {
         /varset TempGuy ${DoBuffQueue[${a},1]}
         /varset TempVar ${DoBuffQueue[${a},2]}
      }
|     /if (!${Spawn[${DoBuffQueue[${a},1]}].ID}) /call RemoveFromQueue ${DoBuffQueue[${a},1]} ${DoBuffQueue[${a},2]}
   /next a
   /if (!${ACState} && ${DoMelee} && ${ADMobFound}) /return
   /if (!${SpawnCount[npc radius ${Radius} zradius ${ZRadius} noalert ${AlertList}]} || ${ABTarType[${TempVar}].Find[cbt]} || ${Select[${ABGem[${TempVar}]},item,alt]} || ${Me.Gem[${ABSpell[${TempVar}]}]}) {
      /if (${TempGuy}) /if (((${Me.SpellReady[${ABSpell[${TempVar}]}]} || ${Spell[${ABSpell[${TempVar}]}].RecastTime.TotalSeconds}<30) && ${Me.CurrentMana}>${Spell[${ABSpell[${TempVar}]}].Mana} && ${Me.PctMana}>=${ABSpellMinMana[${TempVar}]}) || (${ABGem[${TempVar}].Equal[item]} && ${FindItem[${ABSpell[${TempVar}]}].TimerReady}<30) || (${ABGem[${TempVar}].Equal[alt]} && ${Me.AltAbilityReady[${ABSpell[${TempVar}]}]}<30)) {
         /call MQ2Cast "${ABSpell[${TempVar}]}" ${If[!${Select[${ABGem[${TempVar}]},item,alt]},gem,]}${ABGem[${TempVar}]} 10s CastCheck -targetid|${TempGuy}
         /if (${castReturn.Equal[CAST_TAKEHOLD]} || ${castReturn.Equal[CAST_SUCCESS]} || ${Cast.Return.Equal[CAST_OUTDOORS]}) {
            /call RemoveFromQueue ${TempGuy} ${TempVar}
         }
      }
   }
/return


Sub AddToQueue(int Buffee,int BuffNumber)
   /if (${MBDebug}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}AQ - ${Macro.RunTime} - ${MBCurrentSpell} - ${interruptFlag} - ${Cast.Status} - ${Cast.Result} - ${Cast.Stored} - ${NetBots[${TankName}].TargetID} - ${Target.ID} - ${Me.Casting.ID} - ${Buffee} - ${BuffNumber}
   }
   /declare a int local
|  /if (!${RestrictedList.Find[|${DoBuffID[${BuffNumber}]}|]} && ${QueueCount}<25 && ${Spawn[${Buffee}].Type.NotEqual[corpse]} && ${Spawn[${Buffee}].ID}) {
   /if (${QueueCount}<25 && ${Spawn[${Buffee}].Type.NotEqual[corpse]} && ${Spawn[${Buffee}].ID}) {
      /for a 1 to 25
         /if (!${DoBuffQueue[${a},1]}) {
            /varset DoBuffQueue[${a},1] ${Buffee}
            /varset DoBuffQueue[${a},2] ${BuffNumber}
            /varcalc QueueCount ${QueueCount}+1
            /return
         }
      /next a
   }
/return


Sub RemoveFromQueue(int Buffee,int BuffNumber)
   /if (${MBDebug}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}RQ - ${Macro.RunTime} - ${MBCurrentSpell} - ${interruptFlag} - ${Cast.Status} - ${Cast.Result} - ${Cast.Stored} - ${NetBots[${TankName}].TargetID} - ${Target.ID} - ${Me.Casting.ID} - ${Buffee} - ${BuffNumber}
   }
   /doevents IRC
   /doevents Chat
   /declare a int local
   /declare tABMob int local
   /for a 1 to 25
      /if (((${Spawn[${DoBuffQueue[${a},1]}].State.Equal["DEAD"]} || !${Spawn[${DoBuffQueue[${a},1]}].ID}) || ((${DoBuffQueue[${a},1]}==${Buffee} || (!${Buffee} && (${Group.Member[${Spawn[${DoBuffQueue[${a},1]}].CleanName}].Index} || ${Group.Member[${Spawn[${DoBuffQueue[${a},1]}].Master.CleanName}].Index}))) && ${DoBuffQueue[${a},2]}==${BuffNumber})) && ${DoBuffQueue[${a},1]}) {
         /varset DoBuffQueue[${a},1] 0
         /varset DoBuffQueue[${a},2] 0
         /varcalc QueueCount ${QueueCount}-1
      }
   /next a
/return


Sub TellEvent(string From, string MsgText)
   /if (!${DoTells}) /return
   /bc Msg: ${Time} [+g+]Tell Received from[+x+] ${From} : ${MsgText}
   /docommand /${BeepCommand}
/return


Sub Event_Camping
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Campout detected. Ending macro.
   /if (${Cursor.ID}) /call ClearCursor force
   /call MainExit
   /endmacro
/return


Sub CastCheck(timer giveUpTimer)
| Let's do the looping here, instead of repeatedly calling it
   /while (${Cast.Status.Find[C]}) {   
      /if (!${giveUpTimer}) /return
      /if (${DoHeals} && ${AHCount} && !${MBCurrentSpell.Arg[4,|].Equal[NHI]}) /call AHHealCheck
      /if (${Cast.Status.Equal[I]} || ${interruptFlag}) /return
      /if (${Window[RespawnWnd].Open} || ${NetBots[${Me.CleanName}].Charmed}) {
         /varset interruptFlag TRUE
         /return
      }
      /if ((${Target.Type.Equal[corpse]} || !${Target.ID}) && ${Me.Casting.ID} && ${Spell[${Me.Casting.ID}].TargetType.Find[Single]}) {
         /call interrupt
         /return
      }
      /if (${MBCurrentSpell.Arg[1,|].Equal[AD]}) {
         /if (${Me.Casting.ID} && !${Spell[${Me.Casting.ID}].TargetType.Find[ ae]} && !${Spell[${Me.Casting.ID}].SpellType.Find[Beneficial]}) {
            /if (${Select[${ADTarType[${MBCurrentSpell.Arg[2,|]}]},1,11,3,13]}) {
               /if (${NetBots[${TankName}].TargetID}!=${MBCurrentSpell.Arg[3,|]}) {
                  /call interrupt
                  /return
               }
            } else /if (${Select[${ADTarType[${MBCurrentSpell.Arg[2,|]}]},2,12]}) {
               /if (${NetBots[${TankName}].TargetID}==${MBCurrentSpell.Arg[3,|]}) {
                  /call interrupt
                  /return
               }
            }
         }
      } else /if (${Select[${MBCurrentSpell.Arg[1,|]},AB,AE]} && ${Me.Casting.ID}) {
	     /if (${${MBCurrentSpell.Arg[1,|]}TarType[${MBCurrentSpell.Arg[2,|]}].Find[cbt]}) {
            /if (!${${MBCurrentSpell.Arg[1,|]}TarType[${MBCurrentSpell.Arg[2,|]}].Find[idle]}) {
			   /call ADSpawnCheck Find
			   /if (!${ADMobFound}) {
                  /call interrupt
                  /return
			   }
            }
         } else /if (${Cast.Timing}>1000 || ${Cast.Status.Find[M]}) {
            /call ADSpawnCheck Find
            /if (${ADMobFound}) {
               /call interrupt
               /return
            }
         }
      }
   }
/return


Sub ParseAnnounce(string sMsg, string sText, string sTarget, string sSpell)
   /if (${sText.Length}>10) {
      /if (${sText.Find[%t]}) {
         /varset sText ${sText.Replace[%t,${sTarget}]}
	  }
      /if (${sText.Find[%s]}) {
         /varset sText ${sText.Replace[%s,${sSpell}]}
	  }
   } else {
      /varset sText ${sText} ${sMsg} -[ ${sTarget} ]- with ${sSpell}
   }
   /docommand ${sText}
/return


Sub IniCommand(string IniText)
| /echo ${If[${TimeStampOnEcho},[${Time}] ,]}IniCommand: Text = ${IniText}
  /if (${IniText.Length}<3) /return
  /declare b int local 1
  /declare sVarC string local
  /declare sVarL string local
  /declare sVarM string local
  /for b 1 to ${IniText.Count[{]}
    /varset sVarL ${IniText.Arg[${b},{]}
    /varset sVarC ${sVarC}${sVarL}${
      }
  /next b
  /varset sVarC ${sVarC}${IniText.Arg[${b},{]}
  /if (!${IniText.Count[{]}) /varset sVarC ${IniText}
  /varset sVarM
  /for b 1 to ${sVarC.Count[']}
    /if (${sVarC.Find['']}) {
      /varset sVarL ${sVarC.Mid[1,${Math.Calc[${sVarC.Find['']}-1]}]}"
| "
      /varset sVarC ${sVarC.Mid[${Math.Calc[${sVarC.Find['']}+2]},${sVarC.Length}]}
      /varset sVarM ${sVarM}${sVarL}
    }
  /next b
  /varset sVarM ${sVarM} ${sVarC}
  /if (${sVarM.Length}) /varset sVarC ${sVarM}
| /echo ${If[${TimeStampOnEcho},[${Time}] ,]}"${sVarC}"
  /docommand ${sVarC}
/return FALSE


Sub Event_GoM(string eLine,string eSkip,string eMax)
| /echo ${If[${TimeStampOnEcho},[${Time}] ,]}GOM: ${eLine} - ${eSkip} - ${eMax}
   /if (${MBPause} || !${GoMNuke.Length} || (!${Me.Song[Gift of Mana (${eMax})].ID} && !${Me.Song[Gracious Gift of Mana (${eMax})].ID})) /return
   /declare x int local 0
   /declare IniSpell string local
   /for x 1 to ${Math.Calc[${GoMNuke.Count[|]}+1]}
      /call FindAlias ${GoMNuke.Arg[${x},|]}
      /if (!${Macro.Return.Length} || ${Macro.Return.Equal[NULL]}) /return
      /varset IniSpell ${Macro.Return}
      /if ((${Spell[${${IniSpell.Arg[1,|]}Spell[${IniSpell.Arg[2,|]}]}].SpellType.Equal[detrimental]} && (!${Spawn[${ACMATarget}].ID} || ${Spawn[${ACMATarget}].Type.Equal[corpse]})) || ${Spell[${${IniSpell.Arg[1,|]}Spell[${IniSpell.Arg[2,|]}]}].Level}>${Int[${eMax}]}) {
         /if (${GoMNuke.Find[|]} && ${Math.Calc[${x}-1]}<${GoMNuke.Count[|]}) /continue
         /return
      }
      /delay 2s ${Cast.Ready[${${IniSpell.Arg[1,|]}Spell[${IniSpell.Arg[2,|]}]}|gem${${IniSpell.Arg[1,|]}Gem[${IniSpell.Arg[2,|]}]}]}
      /if (!${Cast.Ready[${${IniSpell.Arg[1,|]}Spell[${IniSpell.Arg[2,|]}]}|gem${${IniSpell.Arg[1,|]}Gem[${IniSpell.Arg[2,|]}]}]} && ${GoMNuke.Find[|]} && ${Math.Calc[${x}-1]}<${GoMNuke.Count[|]}) /continue
      /call MQ2Cast "${${IniSpell.Arg[1,|]}Spell[${IniSpell.Arg[2,|]}]}" gem${${IniSpell.Arg[1,|]}Gem[${IniSpell.Arg[2,|]}]} 10s CastCheck ${If[${Spell[${${IniSpell.Arg[1,|]}Spell[${IniSpell.Arg[2,|]}]}].SpellType.Equal[detrimental]},-targetid|${ACMATarget},-targetid|${Spawn[${TankName}].ID}]}
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}GoMNuke : ${${IniSpell.Arg[1,|]}Spell[${IniSpell.Arg[2,|]}]}
      /return
   /next x
/return


Sub FindAlias(SpellAlias)
   /declare tCastClass string local AH AD AB AE AQ
   /declare d int local
   /declare x int local
   /declare y int local
   /declare tSpell string local
   /for d 1 to ${tCastClass.Count[ ]}
      /if (${${tCastClass.Arg[${d}]}Count}) {
          /for x 1 to ${${tCastClass.Arg[${d}]}Count}
             /for y 1 to ${${tCastClass.Arg[${d}]}SpellAlias[${x}].Count[|]}
                /if (${${tCastClass.Arg[${d}]}SpellAlias[${x}].Arg[${y},|].Equal[${SpellAlias}]}) {
                   /varset tSpell ${tSpell}${tCastClass.Arg[${d}]}|${x}|
                }
             /next y
          /next x
      }
   /next d
|  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}tSpell - ${tSpell}
|  AHSpell5 would return as AH|5|
/return ${tSpell}


Sub Event_OutOfStuff
   /if (${DoCombines}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Out of materials. Setting DoCombines FALSE
      /varset DoCombines FALSE
   }
/return


Sub IsNumber(Index)
   /if (!${Index.Length}) /return
   /declare x int local
   /for x 1 to ${Index.Length}
      /if (!${Select[${Index.Mid[${x},1]},0,1,2,3,4,5,6,7,8,9,.]}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}IsNumber - FALSE - Index was ${Index}
         /return FALSE
      }
   /next x
/return TRUE


Sub SearchVendor
|  /declare CurMerchant int outer 0 - Need to set this to the ID of merchant window.  Grab name from window.
   /declare ICount int local
   /declare mItem int local 0
   /declare mItemName string local
   /if (!${Window[MerchantWnd].Open}) {
      /varset CurMerchant 0
      /return
   }
   /declare VendorName string local ${Window[MerchantWnd].Child[MW_MerchantName].Text}
:MerchPopulate
   /varset ICount ${Merchant.Items}
   /delay 1s
   /if (${ICount}!=${Merchant.Items}) /goto :MerchPopulate
   /varset CurMerchant ${Spawn[merchant ${Window[MerchantWnd].Child[MW_MerchantName].Text}].ID}
   /for mItem 1 to ${Merchant.Items}
      /varset mItemName ${Merchant.Item[${mItem}].Name}
      /if (${Ini["${LootIni}","${mItemName.Left[1]}","${mItemName}"].Find[Buy]}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${mItemName} found on ${VendorName}
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${Ini["${LootIni}","${mItemName.Left[1]}","${mItemName}"]}
      }
   /next mItem
/return


| AAPurchasing Revamped - 11/01/2017
| Codebase from AAPurchase.inc - customized, restructured, and integrated into ModBot.

Sub AAInit(IniSection) 
   /declare ListNum string outer 
   /declare a int local 
   /if (!${IniSection.Length}) /varset IniSection AAtoBuy 
   /if (!${Ini["${AAIniFile}","${IniSection}"].Length} ) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Creating AA_${Me.Name}.ini file
      /call AADump
   }
   /call LoadINIVar ${IniSection} AACount 0 AACount ${AAIniFile} int 
   /if (${AACount}) { 
      /declare AA[${AACount}] string outer 
      /for a 1 to ${AACount} 
         /call LoadINIVar ${IniSection} AA${a} " " AA[${a}] ${AAIniFile} 
      /next a 
      /if (${DoAA}) /call AAPicker force
   } 
/return 


Sub AAPicker(string aaParm)
   /if (${MBPause} || !${AACount} || ${Me.AAPoints}<=0) /return
| If not Fast-AA purchase, and Confirmation box is open from something else, skip AA checking for the moment so as not to accidentally click it
   /if (!${Window[OptionsWindow].Child[OptionsGeneralPage].Child[OGP_AANoConfirmCheckbox].Checked} && ${Window[ConfirmationDialogBox].Open}) /return
   /varset AACheckTimer 5m

   /declare x int local 
   /declare Cost int local 
   /declare AALevelStr string local 
   /declare curAALevel int local 
   /declare maxAALevel int local 
   /declare maxPurchaseLevel int local 
   /declare nameAA string local 
   /declare AACategory string local 
   /declare maxCounter int local 0 
   /declare myMax int local ${Math.Calc[${Me.Level}*2]}
   /declare AAtoSpend int local
   /declare wantSomething bool local FALSE
   /declare wantExpendable bool local FALSE
   /declare doAnnounce string local 
   /declare doVerbose bool local
   /declare newAAstr string local
   /declare disAAName string local   

   /if (${aaParm.Equal[force]}) {
      /if (${AAAnnounce.Length}) {
         /varset doAnnounce ${AAAnnounce} Msg:
      } else {
         /varset doAnnounce /bc Msg:
      }
      /varset doVerbose TRUE
   } else {
      /if (${AAAnnounce.Length}) {
         /varset doAnnounce ${AAAnnounce} Msg:
      }
      /varset doVerbose ${AAVerbose}
   } 

   /varcalc AAtoSpend ${Me.AAPoints}-${AABank}
   /if (${AAtoSpend}<=0) {
      /if (${doAnnounce.Length} && ${doVerbose}) {
	     /docommand ${doAnnounce} Unable to purchase AA's at this time. You have no spendable AA.
	  }
      /return
   }

   /for x 1 to ${AACount} 
      /if (!${AA[${x}].Arg[1,"|"].Length} || !${AA[${x}].Arg[2,"|"].Length} || ${AA[${x}].Arg[2,"|"].Equal[X]}) /continue
      /varset nameAA ${AA[${x}].Arg[1,"|"]} 
| Sel ListNum to the page the AA is on. 
      /varset ListNum List${AltAbility[${nameAA}].Type} 
:BuyAdditional
| Clear the Can Purchase button and Filters
      /nomodkey /notify AAWindow ResetFilter leftmouseup
| Check if the skill is available to you. 
      /if (${AltAbility[${nameAA}].ID}) { 
         /varset AALevelStr ${Window[AAWindow].Child[${ListNum}].List[${Window[AAWindow].Child[${ListNum}].List[=${nameAA}]}, 2]} 
         /varset curAALevel ${AALevelStr.Arg[1,"/"]} 
         /varset maxAALevel ${AALevelStr.Arg[2,"/"]}
         /if (${Select[${AA[${x}].Arg[2,"|"]},M,L]}) {
            /varset maxPurchaseLevel ${maxAALevel}
            /if (${curAALevel}>=${maxPurchaseLevel}) {
               /varset newAAstr ${AA[${x}].Arg[1,"|"]}|X
               /if (${AA[${x}].Arg[3,"|"].Length}) {
			      /varset newAAstr ${newAAstr}|${AA[${x}].Arg[3,"|"]}
			   }
               /ini "${AAIniFile}" AAtoBuy AA${x} "${newAAstr}" 
               /continue
            }
         } else { 
            /varset maxPurchaseLevel ${AA[${x}].Arg[2,"|"]} 
            /if (${maxPurchaseLevel}<=0) /continue
            /if (${maxPurchaseLevel}>${maxAALevel}) {
			   /varset maxPurchaseLevel ${maxAALevel}
			}
         }
         /varset AACategory ${Window[AAWindow].Child[${ListNum}].List[${Window[AAWindow].Child[${ListNum}].List[=${nameAA}]}, 5]}
         /if (${AACategory.Equal[Expendable]}) {
            /varset wantExpendable TRUE  
            /varset AACheckTimer 1m
         }
         /if (${curAALevel}<${maxPurchaseLevel}) {
            /varset wantSomething TRUE 
            /varset Cost ${Window[AAWindow].Child[${ListNum}].List[${Window[AAWindow].Child[${ListNum}].List[=${nameAA}]}, 3]} 
| Click the Can Purchase button. 
            /if (!${Window[AAWindow].Child[CanPurchaseFilter].Checked}) { 
			   /nomodkey /notify AAWindow CanPurchaseFilter leftmouseup 
			}
| If the AA still shows in the list, we should be able to purchase it.
            /if (!${Window[AAWindow].Child[${ListNum}].List[=${nameAA}]}==NULL) {
| Check vs. spendable AA (considering Banked AA).
               /if (${Cost}<=${AAtoSpend}) {
                  /call AASelect "${nameAA}"   
                  /call AAPurchase 
                  /varcalc curAALevel ${curAALevel}+1 
                  /if (${doAnnounce.Length}) {
			         /docommand ${doAnnounce} ${Time} Purchased level ${curAALevel}/${maxAALevel} of ${nameAA} for ${Cost} points.
			      }
                  /delay 1s 
| If I purchased an "Enabled" AA, it is probably now Disabled. If it is now Disabled and zero-cost, re-enable it.
                  /if (${nameAA.Find[Enabled]} && ${curAALevel}<${maxPurchaseLevel}) {
				     /varset disAAName ${nameAA.Replace[Enabled,Disabled]}
					 /if (!${Window[AAWindow].Child[${ListNum}].List[=${disAAName}]}==NULL) {
                        /varset Cost ${Window[AAWindow].Child[${ListNum}].List[${Window[AAWindow].Child[${ListNum}].List[=${disAAName}]}, 3]}
                        /if (${Cost}==0) {					 
                           /call AASelect "${disAAName}"   
                           /call AAPurchase
				           /varcalc curAALevel ${curAALevel}+1 
                           /if (${doAnnounce.Length}) {
			                  /docommand ${doAnnounce} ${Time} Purchased level ${curAALevel}/${maxAALevel} of ${disAAName} for ${Cost} points (to re-enable it).
			               }
                           /delay 1s
						}
				     }
                  }					 
| If I still have points after purchasing try to purchase again. 
                  /if (${Me.AAPoints}>0 && ${Me.AAPoints}>${AABank}) {
                     /varcalc AAtoSpend ${Me.AAPoints}-${AABank}
                     /if (${AAtoSpend}>0) {
  					    /goto :BuyAdditional
					 }
                  }
                  /return
			   } else {
			      /if (${doAnnounce.Length} && ${doVerbose}) {
				     /docommand ${doAnnounce} Unable to purchase ${nameAA} at this time. You have ${AAtoSpend} spendable AA (keeping ${AABank} banked) and need ${Cost} AA.
                  }
               }
| Else, check why we can't purchase it
            } else {
| If we are maxing to level, check level first.
               /if (${AA[${x}].Arg[2,"|"].Equal[L]}) {
	   		      /call AALevelCheck "${nameAA}"
			      /if (${Macro.Return.NotEqual[TRUE]}) {
			         /if (${doAnnounce.Length} && ${doVerbose}) {				  
				        /docommand ${doAnnounce} Unable to purchase ${nameAA} at this time. You are Level ${Me.Level} and it requires Level ${Macro.Return}.
                        /docommand ${doAnnounce} Checking if other AA are available.
				     }
                     /continue
			      }
               }			
| If announcing, check for other reasons for the inability to purchase, but only if we are announcing the reasons		
			   /if (${doAnnounce.Length} && ${doVerbose}) {
| Check if Cost is greater than spendable AA. 
                  /if (${Cost}>${AAtoSpend}) { 
                     /docommand ${doAnnounce} Unable to purchase ${nameAA} at this time. You have ${AAtoSpend} spendable AA and need ${Cost} AA.
                  } else {
| Check Level
   		             /call AALevelCheck "${nameAA}"
		             /if (${Macro.Return.NotEqual[TRUE]}) {
			            /docommand ${doAnnounce} Unable to purchase ${nameAA} at this time. You are Level ${Me.Level} and it requires Level ${Macro.Return}.
                     } else {				  
| Click off the Can Purchase button. 
                        /if (${Window[AAWindow].Child[CanPurchaseFilter].Checked}) /nomodkey /notify AAWindow CanPurchaseFilter leftmouseup
| You must not have the expansion or other pre-requisites
                        /if (${Window[AAWindow].Child[AAW_ExpansionFilter].List[AACategory]}) {  
                           /docommand ${doAnnounce} You have ${AAtoSpend} AA to spend and ${nameAA} costs ${Cost} AA. You must not have ${AACategory} or you have not met the prerequisites.
                        } else {
                           /docommand ${doAnnounce} You have ${AAtoSpend} AA to spend and ${nameAA} costs ${Cost} AA. You must not have met the prerequisites.
			            }
				     }
                  }
			   }
			}
| Check if we are skipping to the next AA, or stopping
            /if (${AA[${x}].Arg[3,"|"].Equal[S]}) {
	  		   /if (${doAnnounce.Length} && ${doVerbose}) {			
                  /docommand ${doAnnounce} Checking if other AA are available.
			   }
               /continue
            }
            /return
| Else, the skill is maxed. 
         } else { 
            /if (${doAnnounce.Length} && ${doVerbose}) {
			   /if (${curAALevel}==${maxAALevel} && ${maxPurchaseLevel}==${maxAALevel}) {
   			      /docommand ${doAnnounce} ${nameAA} is Maxed
               }
            }			   
         } 
| Else, that skill is not available. 
      } else { 
         /if (${doAnnounce.Length}) {
		    /docommand ${doAnnounce} ${nameAA} is not a valid ${Me.Class} Ability.
		 }
      } 
   /next x 
| If all skills are maxed
   /if (!${wantSomething} && !${wantExpendable}) { 
      /if (${DoAA}) {
         /if (${doAnnounce.Length}) {
		    /docommand ${doAnnounce} No defined skills or all defined skills maxed. Setting DoAA FALSE.
		 }
         /varset DoAA FALSE
      } else {
         /if (${doAnnounce.Length}) {
		    /docommand ${doAnnounce} No defined skills or all defined skills maxed.
		 }
      }
| Else, if not, wait for next round. 
   } else { 
      /if (${doAnnounce.Length} && ${doVerbose}) {
	     /docommand ${doAnnounce} Cycled through all AA and nothing to purchase this round.
	  }
   } 
| If banked AA is greater than or equal to warning level and less than max level, issue a warning.
   /if (${Me.AAPoints}>=${AAWarning} && ${Me.AAPoints}<${myMax}) {
      /if (${doAnnounce.Length}) {
	     /docommand ${doAnnounce} Warning! You are at ${Me.AAPoints} banked AA.
	  }
   }
| If banked AA equals max, turn off alt exp (as requested). 
   /if (${Me.AAPoints}>=${myMax}) { 
      /if (${doAnnounce.Length} && ${doVerbose}) {
	     /docommand ${doAnnounce} You have maxed your AAs and are unable to purchase any of your defined AAs.
	  }
      /if (${AAtoNormal}) {
         /if (${doAnnounce.Length}) {
		    /docommand ${doAnnounce} Changing you to full normal XP.
		 }
         /alt off
      } 
   } 
/return 


Sub AASelect(string AAName)
   /nomodkey /notify AAWindow ResetFilter leftmouseup
   /nomodkey /notify AAWindow AAW_Subwindows tabselect ${AltAbility[${AAName}].Type}
   /nomodkey /notify AAWindow ${ListNum} listselect ${Window[AAWindow].Child[${ListNum}].List[=${AAName}]}
   /nomodkey /notify AAWindow ${ListNum} leftmouse ${Window[AAWindow].Child[${ListNum}].List[=${AAName}]} 
/return 


Sub AALevelCheck(string AAName)
   /declare lvlStart int local
   /declare lvlEnd int local
   /declare requiredLevel int local 
   
   /call AASelect "${AAName}"  
   /varset lvlStart ${Math.Calc[${Window[AAWindow].Child[AAW_DescriptionNext].Text.Find[Level:]} + 7]}
   /if (${lvlStart}) {
      /varset lvlEnd ${Window[AAWindow].Child[AAW_DescriptionNext].Text.Right[${Math.Calc[-${lvlStart}+1]}].Find[,]}
      /if (${lvlEnd}>1) {
         /varset requiredLevel ${Window[AAWindow].Child[AAW_DescriptionNext].Text.Mid[${lvlStart},${Math.Calc[${lvlEnd}-1]}]}
         /if (${requiredLevel}>${Me.Level}) {
		    /return ${requiredLevel} 
		 }
      }
   }
/return TRUE

| NextIndex doesn't always work, so we will stick with clicking the button
|Sub AAPurchase(string AAName)
|   /if (${AltAbility[${AAName}].NextIndex}>0) {
|      /alt buy ${AltAbility[${AAName}].NextIndex}
|   } else {
|      /alt buy ${AltAbility[${AAName}].ID}
|   }
|   /delay 2
|/return
 
 
Sub AAPurchase 
   /nomodkey /notify AAWindow TrainButton leftmouseup 
| If fast AA purchase is not on, accept the purchase. 
   /if (!${Window[OptionsWindow].Child[OptionsGeneralPage].Child[OGP_AANoConfirmCheckbox].Checked}) { 
      /delay 10s ${Window[ConfirmationDialogBox].Open}
      /if (!${Window[ConfirmationDialogBox].Open}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}AA confirmation box not showing. Your AA purchase may not have been made.
         /return
      }
      /delay 2 
      /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup 
   }
   /delay 2 
/return 


Sub Event_AAEarned
   /varset AACheckTimer
/return 


Sub Event_AACapped
   /if (${AAtoNormal}) {
      /bc Msg: You have reached the AA cap. Changing you to full normal XP.
      /alt off
   } else {
      /bc Msg: You have reached the AA cap.
   } 
/return 


Sub AADump
   /declare a int local 
   /declare b int local
   /declare c int local
   /declare AAcnt int local 1 
   /ini ${AAIniFile} AAtoBuy AACount 0 
   /declare AALevelStr string local
   /declare AACostStr string local
   /declare AACategory string local
   /declare AATabs string local 35214 
|  /declare AATabs string local 3214 

   /nomodkey /notify AAWindow ResetFilter leftmouseup
   /delay 2

   /for c 1 to 5
      /varset b ${AATabs.Mid[${c},1]}
      /if (!${Window[AAWindow].Child[List${b}]}) /continue
      /varset a 1
	  /while (1) {
         /if (!${Window[AAWindow].Child[List${b}].List[${a}].Length}) /break 
         /varset AALevelStr ${Window[AAWindow].Child[List${b}].List[${a}, 2]} 
         /varset AACostStr ${Window[AAWindow].Child[List${b}].List[${a}, 3]} 
         /varset AACategory ${Window[AAWindow].Child[List${b}].List[${a}, 5]} 
         /if (${AACategory.Equal[Expendable]} || (${AALevelStr.Arg[1,"/"]}<${AALevelStr.Arg[2,"/"]} && ${AACostStr.Length} && ${AACostStr.NotEqual[0]})) { 
            /ini "${AAIniFile}" AAtoBuy AA${AAcnt} "${Window[AAWindow].Child[List${b}].List[${a}]}|" 
            /varcalc AAcnt ${AAcnt}+1 
         } 
         /varcalc a ${a}+1 
      } 
   /next c
   /ini "${AAIniFile}" AAtoBuy AACount ${Int[${Math.Calc[${AAcnt} -1]}]} 
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Finished dump to file: ${AAIniFile} 
/return


Sub PetToys(int petID)
   /if (!${Defined[DoPetToys]}) /return
   /if (!${DoPetToys} || !${PTCount}) /return
   /if (!${petID}) /return
   /if (${Me.Invis}) {
      /bc Msg: Cannot give toys while Invis
      /return
   }
   /declare i int local
   /declare j int local
   /declare tPack int local 0
   /declare tItem int local 0
   /declare summonedID int local

   /if (!${Spawn[pet id ${petID}].ID}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}PetToys - Pet not found
      /return FALSE
   }
   /if (${Spawn[id ${petID}].Distance3D}>50) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}PetToys - Pet ${Spawn[id ${petID}].Name} too far away.
      /return FALSE
   }
   /if (${Spawn[id ${petID}].Distance3D}>12) {
      /call MBMoveTo ${Spawn[id ${petID}].Y} ${Spawn[id ${petID}].X}
      /if (!${Macro.Return.Equal[TRUE]}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}PetToys - Problem getting to ${Spawn[id ${petID}].Name}
         /return FALSE
      }
   }
   /call ClearCursor force
   /for i 1 to ${Me.NumBagSlots}
      /if (${Me.Inventory[pack${i}].Container}) /continue
      /if (${Me.FreeInventory}>1) {
         /varset tPack ${i}
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Using pack ${tPack} for toys
         /break
      }
   /next i
   /if (${tPack}==0) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}No inventory slot for pet toys.
      /return FALSE
   }

   /for i 1 to ${PTCount}
      /if (!${DoPetToys}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}PetToysOff - aborting
         /return FALSE
      }
      /if (${Me.Pet.ID}==${petID}) {
         /if (${Spell[${PTSpell[${i}].Arg[1,|]}].Level}>=76 && (${PTSpell[${i}].Find[muzzle]} || ${PTSpell[${i}].Find[visor]} || ${PTSpell[${i}].Find[belt]})) /continue
      } else {
         /bc Msg: Giving toys to ${Spawn[id ${petID}].CleanName} (${i} of ${PTCount})
      }
      /if (${Me.Book[${PTSpell[${i}].Arg[1,|]}]}) {
         /call MQ2Cast "${PTSpell[${i}].Arg[1,|]}" ${PTSpell[${i}].Arg[2,|]} 10s -targetID|${Me.ID} -maxtries|3
         /delay 15s ${Cursor.ID}
         /while (1) {
            /if (!${Cursor.ID}) {
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with summoning pettoys - aborting
               /return FALSE
            }
            /if (${Cursor.NoRent}) /break
            /if (${Cursor.ID}) /autoinv
            /delay 5s ${Cursor.ID}
         }
         /varset summonedID ${Cursor.ID}
         /nomodkey /itemnotify pack${tPack} leftmouseup
         /delay 5s ${Cursor.ID}!=${summonedID}
         /if (${Cursor.ID}) /call ClearCursor force
         /delay 5
         /if (${Me.Inventory[pack${tPack}].ID}!=${summonedID}) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with pettoys - aborting
            /return FALSE
         }
         /if (${Me.Inventory[pack${tPack}].Container} || ${Me.Inventory[pack${tPack}].Name.Find[Folded]}) {
            /if (${Me.Inventory[pack${tPack}].Name.Find[folded]}) {
               /nomodkey /itemnotify pack${tPack} rightmouseup
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Opening ${Me.Inventory[pack${tPack}].Name}
               /delay 15s ${Cursor.ID}
               /if (!${Cursor.ID}) /return FALSE
               /delay 1s
               /nomodkey /itemnotify pack${tPack} leftmouseup
               /delay 5
               /delay 5s ${Me.Inventory[pack${tPack}].ID}
               /if (!${Me.Inventory[pack${tPack}].Name.Find[Phantom Satchel]} && !${Me.Inventory[pack${tPack}].Name.Find[Pouch of Quellious]}) {
                  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with pettoys pack - aborting
                  /return FALSE
               }
               /if (${Cursor.ID}) /call ClearCursor force
               /delay 5
            }
            /if (${Me.Inventory[pack${tPack}].Container} && (${Me.Inventory[pack${tPack}].Name.Find[Phantom Satchel]} || ${Me.Inventory[pack${tPack}].Name.Find[Pouch of Quellious]})) {
               /nomodkey /itemnotify pack${tPack} rightmouseup
               /delay 10
               /if (${PTItem1[${i}].Length} && ${PTItem1[${i}].NotEqual[NULL]}) {
                  /if (${FindItemCount[=${PTItem1[${i}]}]}) {
                     /call GiveCheck "${petID}" "${PTItem1[${i}]}" PetToys ${tPack}
                     /if (${Macro.Return.Equal[FALSE]}) {
                        /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with pettoys item1 - aborting
                        /return FALSE
                     }
                  }
                  /if (${PTItem2[${i}].Length} && ${PTItem2[${i}].NotEqual[NULL]} && ${FindItemCount[=${PTItem2[${i}]}]}) {
                     /call GiveCheck "${petID}" "${PTItem2[${i}]}" PetToys ${tPack}
                     /if (${Macro.Return.Equal[FALSE]}) {
                        /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with pettoys item2 - aborting
                        /return FALSE
                     }
                  }
               } else {
                  /for tItem 1 to ${Me.Inventory[pack${tPack}].Container}
                     /if (!${DoPetToys}) {
                        /echo ${If[${TimeStampOnEcho},[${Time}] ,]}PetToysOff - aborting
                        /return FALSE
                     }
                     /if (${Me.Inventory[pack${tPack}].Item[${tItem}].ID} && ${Me.Inventory[pack${tPack}].Item[${tItem}].Name.Length}) {
                        /call GiveCheck "${petID}" "${Me.Inventory[pack${tPack}].Item[${tItem}].Name}" PetToys ${tPack}
                        /if (${Macro.Return.Equal[FALSE]}) {
                           /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with pettoys item2 - aborting
                           /return FALSE
                        }
                     }
                  /next tItem
               }
            }
         }
         /if (!${DoPetToys}) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}PetToysOff - aborting
            /return FALSE
         }
         /if (${Me.Inventory[pack${tPack}].Name.Find[Summoned:]}) {
            /call GiveCheck "${petID}" "${Me.Inventory[pack${tPack}].Name}" PetToys ${tPack}
            /if (${Macro.Return.Equal[FALSE]}) {
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with pettoys item - aborting
               /return FALSE
            }
         }
         /if (${Me.Inventory[pack${tPack}].Name.Find[Phantom Satchel]} || ${Me.Inventory[pack${tPack}].Name.Find[Pouch of Quellious]}) {
            /if (${Me.Inventory[pack${tPack}].Items}) {
               /for tItem 1 to ${Me.Inventory[pack${tPack}].Container}
                  /if (!${Me.Inventory[pack${tPack}].Item[${tItem}].NoRent} && ${Me.Inventory[pack${tPack}].Item[${tItem}].Name.Length}) {
                     /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Bag has non summoned item(s) in it. Aborting delete. Setting DoPetToys FALSE
                     /varset DoPetToys FALSE
                     /return FALSE
                  }
               /next tItem
            }
            /nomodkey /itemnotify pack${tPack} leftmouseup
            /delay 5s ${Cursor.ID}
            /if (${Cursor.Name.Find[Phantom Satchel]} || ${Cursor.Name.Find[Pouch of Quellious]}) /destroy
            /delay 1s !${Cursor.ID}
         }
      }
   /next i
   /bc Msg: Finished giving toys to ${Spawn[id ${petID}].Name}
/return TRUE


| Check for Stuck Gems
Sub StuckGemsCheck
   /if (!${DoStuckGems}) /return
   /if (!${Me.Casting.ID} || !${GemStuckID} || ${Me.Casting.ID}!=${GemStuckID}) {
      /varset GemStuckTimer 25s
      /varset GemStuckID ${Me.Casting.ID}
   } else /if (!${GemStuckTimer}) {
      /if (${MBDebug}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]} GSb - ${Time} - ${Me.Casting.ID} - ${GemStuckID}
      }
      /call StuckGems
      /if (${MBDebug}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]} GSc - ${Time} - ${Me.Casting.ID} - ${GemStuckID}
      }
   }
/return


| Stuck Gems
Sub StuckGems
   /if (!${Me.Casting.ID}) /return
   /if (!${DoStuckGems}) {
      /if (!${GemStuckRetry}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}My Spell Gems may be Stuck, but DoStuckGems is FALSE.
         /varset GemStuckRetry 3s
      }
      /return
   }
   /if (!${GemStuckRetry}) {
      /docommand /${BeepCommand}
      /bc ${If[${TimeStampOnEcho},[${Time}] ,]}My Spell Gems may be Stuck
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}My Spell Gems may be Stuck
      /delay 1s
      /if (${StuckGemsAA.Length}>2) {
         /if (${Me.AltAbilityReady[${StuckGemsAA}]}) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Attempting to unstick with ${StuckGemsAA}.
            /tar id ${Me.ID}
            /alt act ${Me.AltAbility[${StuckGemsAA}].ID}
            /delay 5s !${Me.Casting.ID}
         } else {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Waiting for ${StuckGemsAA} to be ready so we can unstick the gems.
         }
      } else {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You have not set StuckGemsAA in your ini, so we can't unstick you.
      }
      /varset GemStuckRetry 2s
   }
/return


Sub EditTwist(string tTwistList,string tGem,string tAction)
   /declare a int local
   /declare b int local
   /declare tList string local
   /declare sReturn string local

   /if (!${Select[${tGem},${ValidSpellGems}]} || !${Select[${tAction},add,delete]}) {
      /varset sReturn ${tTwistList}
   } else /if (${tAction.Equal[add]}) {
      /if (!${tTwistList.Find[${tGem}]}) {
         /varset sReturn ${tTwistList} ${tGem}
      } else /if (!${Select[${tGem},1,2,3]}) {
         /varset sReturn ${tTwistList}
      } else {
         /varset b ${tTwistList.Count[ ]}+1
         /for a 1 to ${b}
            /if (${tTwistList.Arg[${a}].Equal[${tGem}]}) /break
         /next a
         /if (${a}>${b}) {
            /varset sReturn ${tTwistList} ${tGem}
         } else {
            /varset sReturn ${tTwistList}
         }
      }
   } else {
      /if (!${tTwistList.Find[${tGem}]}) {
         /varset sReturn ${tTwistList}
      } else {
         /varset tList
         /for a 1 to ${tTwistList.Count[ ]}+1
            /if (!${tTwistList.Arg[${a}].Equal[${tGem}]}) /varset tList ${tList} ${tTwistList.Arg[${a}]} 
         /next a
         /varset sReturn ${tList}
      }
   }
/return ${sReturn}


|Get Bard Slot for item casting with Twist
Sub GetBardSlot(string itemName,string tCast)
   /declare tCastClass string local
   /declare tCastGem string local
   /declare a int local

   /if (${Me.Class.ShortName.NotEqual[BRD]}) /return
   /varset tCastClass ${tCast.Arg[1,|]}
   /if (!${Select[${tCastClass},AD,AH,AB,AQ,AE]}) /return
   /varset tCastGem ${tCast.Arg[2,|]}
   /if (!${${tCastClass}Gem[${tCastGem}].Length}) /return
   /if (${FindItem[=${itemName}].ID}) {
      /if (${NextBardSlot}>20) {
         /varcalc a ${FindItem[=${itemName}].CastTime}/100+5
         /squelch /twist set ${NextBardSlot} ${a} 5 "${itemName}" DISABLED
         /varset ${tCastClass}BardSlot[${tCastGem}] ${NextBardSlot}
         /varcalc NextBardSlot ${NextBardSlot}-1
      }
   }
/return 


Sub MemGem(int tGem,string sSpell)
   /if (${Me.Gem[${tGem}].Name.Equal[${sSpell}]}) {
	  /return TRUE
   }   
   /if (${Cursor.ID}) {
      /call ClearCursor force
   }			   
   /if (${Me.Class.ShortName.NotEqual[BRD]}) {
      /delay 2s ${Cast.Status.Equal[i]}
   }   
   /memspell ${tGem} "${sSpell}"
   /delay 5
   /delay 5s ${Me.Gem[${tGem}].Name.Equal[${sSpell}]} || ${Me.Standing}
   /if (${Window[SpellBookWnd].Open}) /book
   /if (${Me.Gem[${tGem}].Name.Equal[${sSpell}]}) {
	  /return TRUE
   } else /if (${Me.Standing}) {
      /return INTERRUPTED
   }
   /bc Msg: FAILED MEMORIZE - Gem ${tGem} - ${sSpell}
/return FALSE


Sub SetPuller
   /if (${ManualPuller} && ${AllPullerName.Length}<3) /return
   /declare PrevPullerName string local ${PullerName}
   /declare s string local
   /declare i int local
   /if (!${ManualPuller} && ${Group.Puller.Name.Length} && !${Group.Puller.Type.Equal[corpse]} && !${Group.Puller.Offline}) { 
      /varset s ${Group.Puller.Name}
   } else /if (${AllPullerName.Length}>2) {
      /if (!${ManualPuller} || !${PullerName.Length} || ${Spawn[${PullerName}].Type.Equal[corpse]}) { 
         /for i 1 to ${Math.Calc[${AllPullerName.Count[,]}+1]}
            /if (${Spawn[${AllPullerName.Arg[${i},,]}].ID} && !${Spawn[${AllPullerName.Arg[${i},,]}].Type.Equal[corpse]}) {
               /varset s ${AllPullerName.Arg[${i},,]}
			   /break
            }
         /next i
	  }
   }   
   /if (${s.Length}) {
      /varset PullerName ${s.Left[1].Upper}${s.Right[-1]}
      /if (!${PullerName.Equal[${PrevPullerName}]}) {
	     /bc Msg: ${PullerName} is now PULLER
	  }
   }		 
/return


Sub SetTank
   /if (${ManualTank} && ${AllTankName.Length}<3) /return
   /declare PrevTankName string local ${TankName}
   /declare s string local
   /declare i int local
   /if (!${ManualTank} && ${Group.MainAssist.Name.Length} && !${Group.MainAssist.Type.Equal[corpse]} && !${Group.MainAssist.Offline}) { 
      /varset s ${Group.MainAssist.Name}
   } else /if (!${ManualTank} && ${Group.MainTank.Name.Length} && !${Group.MainTank.Type.Equal[corpse]} && !${Group.MainTank.Offline}) { 
      /varset s ${Group.MainTank.Name}
   } else /if (${AllTankName.Length}>2) {
      /if (!${ManualTank} || !${TankName.Length} || ${Spawn[${TankName}].Type.Equal[corpse]}) { 
         /for i 1 to ${Math.Calc[${AllTankName.Count[,]}+1]}
            /if (${Spawn[${AllTankName.Arg[${i},,]}].ID} && !${Spawn[${AllTankName.Arg[${i},,]}].Type.Equal[corpse]}) {
			   /varset s ${AllTankName.Arg[${i},,]}
			   /break
			}
         /next i
      }
   }
   /if (${s.Length}) {
      /varset TankName ${s.Left[1].Upper}${s.Right[-1]}
      /if (!${TankName.Equal[${PrevTankName}]}) {
         /bc Msg: ${TankName} is now TANK
	  }
   }
/return


| MQ2Cast Spell Routines - Highly customized for ModBot
|
| These are much like the code from Spell_Routines.inc by Rusty. This uses the MQ2Cast Plugin by s0rcier.
Sub MQ2Cast(string spellName,string spellType,timer giveUpTimer,string mySub,string MQ2Feature1,string MQ2Feature2,string MQ2Feature3,string MQ2Feature4)

   /declare i int local
   /declare bandSet string local
   /declare oldItem string local

   /varset interruptFlag FALSE

   /if (${mySub.Left[1].Equal[-]}) {
      /varset MQ2Feature4 ${MQ2Feature3}
      /varset MQ2Feature3 ${MQ2Feature2}
      /varset MQ2Feature2 ${MQ2Feature1}
      /varset MQ2Feature1 ${mySub}
      /varset mySub
   }
   /if (${Corpse.Open}) {
      /nomodkey /keypress esc
   }
   /if (${Me.Feigning}) {
      /if (!${noFeigning}) /stand
   }
   /if (${Window[SpellBookWnd].Open}) /book
   /if (${spellType.Equal[item]}) {
      /if (${FindItem[=${spellName}].EffectType.Equal[Click Worn]}) {
         /call SREquipItem "${spellName}"
         /varset oldItem ${Macro.Return}
      }
   }
   /if (${Stick.Status.Equal[on]}) {
      /squelch /stick pause
      /delay 5 !${Me.Moving}
   }
   /if (${AdvPath.Following}) {
      /if (!${AdvPath.Paused} && ${FollowFlag}==1) {
         /varset FollowFlag 3
         /squelch /afollow pause
      }
   }
   /for i 1 to 4
      /if (!${MQ2Feature${i}.Length}) /break
      /if (${MQ2Feature${i}.Find[-bandolier|]}) {
         /varset bandSet ${MQ2Feature${i}.Arg[2,|]}
         /if (${bandSet.Length}) /call SRBandIn ${bandSet}
         /break
      }
   /next i
| Since there are other things that may be casting/activating (like MQ2Melee things), it is possible to be here and find MQ2Cast is no longer ready. Wait for it.
   /if (${Me.Class.ShortName.NotEqual[BRD]} && ${Cast.Status.NotEqual[i]}) {
      /while (${Cast.Status.NotEqual[i]}) {
|        /if (${MBDebug}) {
|           /echo ${If[${TimeStampOnEcho},[${Time}] ,]}SRa - ${Time} - ${Macro.RunTime} - ${MBCurrentSpell} - ${interruptFlag} - ${Cast.Status} - ${Cast.Result} - ${Cast.Stored} - ${NetBots[${TankName}].TargetID} - ${Me.Casting.ID}
|        }
         /if (!${giveUpTimer}) {
            /call StuckGems
            /if (${Me.Casting.ID}) /return CAST_NOTREADY
         }
         /delay 1
      }
      /varset giveUpTimer ${giveUpTimer.OriginalValue}
   }
   /if (${noInvis}) {
      /squelch /casting "${spellName}|${spellType}"  "-invis" "${MQ2Feature1}" "${MQ2Feature2}" "${MQ2Feature3}" "${MQ2Feature4}"
   } else {
      /squelch /casting "${spellName}|${spellType}"  "${MQ2Feature1}" "${MQ2Feature2}" "${MQ2Feature3}" "${MQ2Feature4}"
   }

   /varset SpellWasCast TRUE
   /while (${Cast.Status.Find[C]}) {
      /if (!${giveUpTimer}) {
         /if (${MBDebug}) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}no SRtimer - ${Time} - ${Macro.RunTime} - ${MBCurrentSpell} - ${interruptFlag} - ${Cast.Status} - ${Cast.Result} - ${Cast.Stored} - ${NetBots[${TankName}].TargetID} - ${Me.Casting.ID}
         }
         /call StuckGems
         /delay 1
      }
      /if (${mySub.Length}) /call ${mySub} ${giveUpTimer} 
      /if (${interruptFlag} || !${MBCurrentSpell.Length}) /break
   }

   /if (${MBDebug}) {
      /if (${Cast.Status.NotEqual[i]} && !${interruptFlag}) /echo ${If[${TimeStampOnEcho},[${Time}] ,]}SR1 - ${Time} - ${MBCurrentSpell} - ${interruptFlag} - ${Cast.Status} - ${Cast.Result} - ${Cast.Stored} - ${NetBots[${TankName}].TargetID}
   }

   /if (${Stick.Status.Equal[paused]}) /squelch /stick unpause

   /if (${AdvPath.Paused} && ${FollowFlag}>2) {
      /varset FollowFlag 1
      /squelch /afollow unpause
   }
   /if (${bandSet.Length}) {
      /call SRBandOut
   }
   /if (${oldItem.Length}) {
      /call SREquipItem ${oldItem}
   }

   /varset castReturn ${Cast.Result}

/return ${Cast.Result}


Sub Interrupt
   /stopcast
   /varset interruptFlag TRUE
/return


Sub SRBandIn(setName)
   /if (!${Defined[bandIni]}) /declare bandIni string local ..\MQ2Bandolier_${Me.Name}.ini
   /declare equipSlots string local
   /declare itemSlot string local
   /declare itemNum string local
   /declare i int local
        
   /varset equipSlots ${Ini["${bandIni}",${setName}]}

   /if (${equipSlots.Length} && ${equipSlots.Count[|]} > 1) {
      /for i 1 to ${numSlots}
         /if (${Me.Inventory[${Math.Calc[${i}-1]}].ID}) {
            /varset equippedArray[${i}] ${Me.Inventory[${Math.Calc[${i}-1]}].Name}
         } else {
            /varset equippedArray[${i}]
         }
      /next i 
      /for i 1 to ${equipSlots.Count[|]} - 1
         /varset itemSlot ${equipSlots.Arg[${i},|]}
         /if (${itemSlot.Length}) {
            /varset itemNum ${Ini["${bandIni}",${setName},${itemSlot}]}
            /if (${itemNum.Length}) {
               /if (${Me.Inventory[${itemSlot}].ID} != ${itemNum}) /call SRSwapItemNum ${itemNum} ${itemSlot}
            }
         }
      /next i
   }
/return


Sub SRBandOut
   /declare i int local
   /for i 1 to ${numSlots}
      /if (${equippedArray[${i}].Length}) {
	     /if (!${equippedArray[${i}].Equal[${Me.Inventory[${Math.Calc[${i}-1]}].Name}]}) {
		    /call SRSwapItem "${equippedArray[${i}]}" ${slotNames.Arg[${i},,]}
		 }
	  }
   /next i 
/return


Sub SRClearCursor
   /declare i int local
:check_cursor
   /if (${Cursor.ID}) {
      /if (!${Cursor.Container} || !${Cursor.Items}) /timed 5 /autoinventory
      /if (${Cursor.Container}) {
         /for i 1 to ${Me.NumBagSlots}    
            /if (!${Me.Inventory[pack${i}].Container}) /nomodkey /itemnotify pack${i} leftmouseup
         /next i
      } 
      /goto :check_cursor
   }
/return


Sub SREquipItem(WhatWhere)
   /declare destName string local
   /declare itemName string local ${WhatWhere.Arg[1,|]}
   /declare slotName string local ${WhatWhere.Arg[2,|]}
    
   /if (${slotName.Equal[NULL]}) /varset slotName ${FindItem[=${itemName}].WornSlot[1].Name}

   /if (${FindItem[=${itemName}].itemSlot}<${numSlots} || !${FindItem[=${itemName}].WornSlot[${slotName}]}) /return

   /if (${Me.Inventory[${slotName}].ID}) /varset destName "${Me.Inventory[${slotName}].Name}|${slotName}"

   /call SRSwapItem "${itemName}" "${slotName}"
/return ${destName}


Sub SRSwapItem(itemName,slotName)
   /if (${Cursor.ID}) /call SRClearCursor
   /exchange "${itemName}" ${slotName}
   /delay 5s ${Me.Inventory[${slotName}].Name.Equal[${itemName}]}
   /delay 5s !${Cursor.ID}
   /if (${Cursor.ID}) /call SRClearCursor
/return


Sub SRSwapItemNum(itemNum,slotNum)
   /if (${Cursor.ID}) /call SRClearCursor
   /exchange ${itemNum} ${slotNum}
   /delay 5s ${Me.Inventory[${slotNum}].ID}==${itemNum}
   /delay 5s !${Cursor.ID}
   /if (${Cursor.ID}) /call SRClearCursor
/return

| End of - MQ2Cast Spell Routines


Sub TFToggle(string sVar,string sSetting)
   /varset ${sVar} ${If[${Select[${sSetting},on,true]},TRUE,${If[${Select[${sSetting},off,false]},FALSE,${If[${${sVar}},FALSE,TRUE]}]}]}
   /bc Setting - ${sVar} ${${sVar}}
/return


Sub Command_ABORT(P0)
   /if (${ACState}) {
      /varcalc ACState ${ACState}+2
      /call AdvCombat
   }
   /if (${Me.Casting.ID}) {
      /if (${Me.Class.ShortName.Equal[BRD]}) /squelch /stoptwist
      /stopcast
      /if (${Me.Class.ShortName.Equal[BRD]}) /squelch /stoptwist
   }
   /if (${Me.Mount.ID}) /dismount
   /varset DoMelee FALSE
   /varset DoDebuffs FALSE
   /varset DoHeals FALSE
   /if (!${Me.Class.ShortName.Equal[brd]}) /varset DoBuffs FALSE
   /bc Setting - DoHeals, DoBuffs, DoDebuffs & DoMelee FALSE and Following ${SenderName}
   /call Command_FOLLOW "follow close"
/return


Sub Command_ADDBUFF(string MsgText)
   /if (${MsgText.Arg[2].Equal[help]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Format: /mb addbuff "<Spell Name>|<gem,alt,item,ect>" <Alias> <TarType> <TarCnt>
   } else /if (${MsgText.Arg[2].Arg[1,|].Length} && ${MsgText.Arg[2].Arg[2,|].Length}) {
      /varcalc ABCount ${ABCount}+1
      /ini "${IniFile}" AdvBuff ABCount ${ABCount}
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${MsgText.Right[-8]} : ABCount = ${ABCount}
      /if (${ABNewFormat}) {
         /ini "${IniFile}" AB${ABCount} Gem ${MsgText.Arg[2].Arg[2,|]}
         /ini "${IniFile}" AB${ABCount} Spell "${MsgText.Arg[2].Arg[1,|]}"
         /if (${MsgText.Arg[3].Length}) /ini "${IniFile}" AB${ABCount} SpellAlias "${MsgText.Arg[3]}"
         /if (${MsgText.Arg[4].Length}) /ini "${IniFile}" AB${ABCount} TarType "${MsgText.Arg[4]}"
         /if (${MsgText.Arg[5].Length}) /ini "${IniFile}" AB${ABCount} TarCnt "${MsgText.Arg[5]}"
      } else {
         /ini "${IniFile}" AdvBuff ABGem${ABCount} ${MsgText.Arg[2].Arg[2,|]}
         /ini "${IniFile}" AdvBuff ABSpell${ABCount} "${MsgText.Arg[2].Arg[1,|]}"
         /if (${MsgText.Arg[3].Length}) /ini "${IniFile}" AdvBuff ABSpellAlias${ABCount} "${MsgText.Arg[3]}"
         /if (${MsgText.Arg[4].Length}) /ini "${IniFile}" AdvBuff ABTarType${ABCount} "${MsgText.Arg[4]}"
         /if (${MsgText.Arg[5].Length}) /ini "${IniFile}" AdvBuff ABTarCnt${ABCount} "${MsgText.Arg[5]}"
      }
	  /echo Restart to add to memory.
   } else {
      /echo Invalid AddBuff String. Buff Not Added.
   }
/return


Sub Command_ADDDEBUFF(string MsgText)
   /if (${MsgText.Arg[2].Equal[help]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Format: /mb adddebuff "<Spell Name>|<gem,alt,item,ect>" <Alias> <TarType> <TarCnt>
   } else /if (${MsgText.Arg[2].Arg[1,|].Length} && ${MsgText.Arg[2].Arg[2,|].Length}) {
      /varcalc ADCount ${ADCount}+1
      /ini "${IniFile}" AdvDebuff ADCount ${ADCount}
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${MsgText.Right[-10]} : ADCount = ${ADCount}
      /if (${ADNewFormat}) {
         /ini "${IniFile}" AD${ADCount} Gem ${MsgText.Arg[2].Arg[2,|]}
         /ini "${IniFile}" AD${ADCount} Spell "${MsgText.Arg[2].Arg[1,|]}"
         /if (${MsgText.Arg[3].Length}) /ini "${IniFile}" AD${ADCount} SpellAlias "${MsgText.Arg[3]}"
         /if (${MsgText.Arg[4].Length}) /ini "${IniFile}" AD${ADCount} TarType "${MsgText.Arg[4]}"
         /if (${MsgText.Arg[5].Length}) /ini "${IniFile}" AD${ADCount} TarCnt "${MsgText.Arg[5]}"
      } else {
         /ini "${IniFile}" AdvDebuff ADGem${ADCount} ${MsgText.Arg[2].Arg[2,|]}
         /ini "${IniFile}" Advdebuff ADSpell${ADCount} "${MsgText.Arg[2].Arg[1,|]}"
         /if (${MsgText.Arg[3].Length}) /ini "${IniFile}" Advdebuff ADSpellAlias${ADCount} "${MsgText.Arg[3]}"
         /if (${MsgText.Arg[4].Length}) /ini "${IniFile}" Advdebuff ADTarType${ADCount} "${MsgText.Arg[4]}"
         /if (${MsgText.Arg[5].Length}) /ini "${IniFile}" Advdebuff ADTarCnt${ADCount} "${MsgText.Arg[5]}"
      }
      /echo Restart to add to memory.
   } else {
      /echo Invalid AddDebuff String. Debuff Not Added.
   }	  
/return


Sub Command_ATTACK(string MsgText)
   /bc Setting - DoMelee TRUE & Attacking
   /varset ACState 100
   /varset DoMelee TRUE
   /if (!${MsgText.Arg[2].Length}) /varset MsgText ${MsgText} ${Me.CleanName}
   /call AdvCombat ${If[${MsgText.Arg[2].Length},${MsgText.Arg[2]},]} ${If[${MsgText.Arg[2].Length} && ${MsgText.Arg[3].Length},${MsgText.Arg[3]},]} ${If[${MsgText.Arg[2].Length} && ${MsgText.Arg[3].Length} && ${MsgText.Arg[4].Length},${MsgText.Arg[4]},]}
/return


Sub Command_AUTOBANK(string MsgText)
   /declare BankerName string local
   /if (${Target.Class.Name.Equal[Banker]}) {
      /varset BankerName ${Target.CleanName}
   } else /if (${Spawn[Banker].Distance} < 20) {
      /varset BankerName ${Spawn[Banker].Name}
   }
   /if (${BankerName.Length}) {
      /call AutoBank ${BankerName} ${MsgText.Right[-9]}
   } else {	  
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}No Banker Targeted or close by
   }
/return


Sub Command_BACKOFF(string MsgText)
      /if (${ACState}) {
         /varcalc ACState ${ACState}+2
         /call AdvCombat
      }
      /squelch /melee reset
      /varset ACMATarget 0
      /if (${Me.Pet.ID}) /pet back off
      /if (${MsgText.Equal[backoff pause]}) {
         /bc Setting - MBPause TRUE
         /varset MBPause TRUE
      }
      /bc Msg: Backing off
/return


Sub Command_BUFFUP(P0)
   /bc Setting - DoBuffs & DoEvents & DoHeals TRUE
   /varset DoEvents TRUE
   /varset DoHeals TRUE
   /varset DoBuffs TRUE
/return


Sub Command_CAMPOUT(string MsgText)
   /if (${AdvPath.Following}) /squelch /afollow off
   /if (${Stick.Status.Equal[on]}) /stick off
   /if (${Me.Casting.ID}) {
      /delay 5s !${Me.Casting.ID}
   }
   /if (${Me.Mount.ID}) {
      /dismount
      /delay 1s
   }
   /if (${Me.AltAbilityReady[Companion's Suspension]} && ${Me.Pet.ID} && ${MsgText.Find[sm]}) {
      /casting "Companion's Suspension|alt" -maxtries|5
      /delay 5s !${Me.Pet.ID}
      /bc Msg: ${If[${Me.Pet.ID},Pet didn't cache,Cached the pet]}
   }
   /if (${MsgText.Find[char]}) {
      /bc Msg: Camping to character select
      /docommand /camp
      /delay 3s
      /docommand /endmacro
   }
   /if (${MsgText.Right[-7].Length}>3) {
      /if (${MsgText.Right[-7].Find[trader]}) {
         /bc Msg: Camping and selecting this account's trader to login
         /call Login "${TraderName}"
      } else {
         /bc Msg: Camping and selecting ${MsgText.Right[-7]} to login
         /call Login ${MsgText.Right[-7]}
      }
      /bc Msg: Problem detected. Ending mac
      /delay 3s
      /docommand /endmacro
   }
   /bc Msg: Camping to desktop
   /docommand /camp desktop
   /delay 3s
   /docommand /endmacro
/return


Sub Command_DOAA(string MsgText)
   /if (${MsgText.Right[-5].Equal[force]}) {
      /bc Msg: Forcing an AA purchase Check
      /call AAPicker force
   } else {
      /call TFToggle DoAA ${MsgText.Arg[2]}
   }
/return


Sub Command_DOCOMMAND(string MsgText)
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Processing command ${MsgText.Right[-10]}
   /docommand ${MsgText.Right[-10]}
/return


Sub Command_DOLIST(P0)
   /bc Msg: DoBuffs ${DoBuffs}
   /bc Msg: DoDebuffs ${DoDebuffs}
   /bc Msg: DoHeals ${DoHeals}
   /bc Msg: DoEvents ${DoEvents}
   /bc Msg: DoMelee ${DoMelee}
   /bc Msg: DoAfk ${DoAfk}
   /bc Msg: DoSit ${DoSit}
   /bc Msg: DoLoot ${DoLoot}
   /bc Msg: DoForage ${DoForage}
   /bc Msg: DoFW ${DoFW}
/return
      

Sub Command_DOPULL(string MsgText)
   /call TFToggle DoPull ${MsgText.Arg[2]}
   /if (${DoPull}) {
      /if (${MsgText.Arg[2].Length} && !${Select[${MsgText.Arg[2]},on,off]}) {
         /varset APPath ${MsgText.Arg[2]}
         /bc Msg: APPath set to [ ${MsgText.Arg[2]} ]
      }
      /if (${TankName.Equal[${Me.Name}]}) /varset ACAssistPct 110
      /if (!${CampStatus}) /mb makecamp
   }
/return


Sub Command_DOQUEST(string MsgText)
   /if (${Select[${MsgText.Arg[2]},on,off,true,false]} || !${MsgText.Arg[2].Length}) {
      /call TFToggle DoQuest ${MsgText.Arg[2]}
      /bc Setting - DoQuest ${DoQuest}
   } else /if (${Cursor.ID} && !${Target.ID} && ${MsgText.Arg[3].Length}) {
      /declare qName string local
      /declare i int local
      /for i 3 to ${Math.Calc[${MsgText.Count[ ]}+1]}
         /varset qName ${qName} ${MsgText.Arg[${i}]}
      /next i
      /ini "${LootIni}" ${Cursor.Name.Left[1]} "${Cursor.Name}" "Quest|${MsgText.Arg[2]}|${qName}|"
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Added - "Quest|${MsgText.Arg[2]}|${qName}|" - to loot.ini for ${Cursor.Name}
   }
/return


Sub Command_DROPPET(P0)
   /if (${ADCharmID}) {
      /bc Msg: Dropping my pet: ${ADCharmID} - ${Spawn[${ADCharmID}].CleanName}
      /bc IncludeID ${ADCharmID}
      /varset ADDropID ${ADCharmID}
   }
/return


Sub Command_DUCK(string MsgText)
   /declare pDuck bool local
   /varset pDuck ${If[(${MsgText.Find[on]} || ${MsgText.Find[true]}),TRUE,${If[(${MsgText.Find[off]} || ${MsgText.Find[false]}),FALSE,${If[${Me.Ducking},FALSE,TRUE]}]}]}
   /if (${pDuck}) {
      /if (!${Me.Ducking}) /nomodkey /keypress duck
      /bc Msg: Ducked
   } else {
      /if (${Me.Ducking}) /nomodkey /keypress duck
      /bc Msg: Un-Ducked
   }
/return


Sub Command_EXCLUDE(string MsgText)
   /if (${MsgText.Right[-8].Equal[save]}) {
      /bc Msg: Saving Exclude List
      /call ProcessExcludeList save
   } else {
      /bc Msg: Excluding ${MsgText.Right[-8]}
      /if (!${ExcludeList.Find[|${MsgText.Right[-8]}|]}) /varset ExcludeList ${ExcludeList}${MsgText.Right[-8]}|
      /call ProcessExcludeList
   }
/return


Sub Command_EXCLUDEID(string MsgText)
   /declare sID string local ${MsgText.Right[-10]}
   /if (${Spawn[${sID}].ID} && ${Spawn[${sID}].ID}==${sID}) {
      /if (!${ExcludeListID.Find[|${sID}|]}) /varset ExcludeListID ${ExcludeListID}${sID}|
      /squelch /alert add ${AlertList} id ${sID}
   }
/return


Sub Command_FOLLOW(string MsgText)
   /squelch /makecamp off
   /varset CampStatus 0
   /varset ACStartHeading 0
   /varset FollowID ${Spawn[pc ${SenderName}].ID}
   /if (!${FollowID}) /return
   /if (${MoveTo.Moving}) /moveto off
   /if (${MsgText.Equal[follow me]} || ${MsgText.Equal[follow]} || ${MsgText.Find[follow close]}) {
      /if (${AdvPath.Active} && !${MsgText.Find[follow close]}) {
         /if (${Stick.Status.Equal[ON]}) /squelch /stick off
         /call AdvPlugCall "${SenderName}"
      }
   } else {
      /varset FollowID ${Spawn[${MsgText.Right[-7]}].ID}
      /if (${AdvPath.Active}) {
	     /call AdvPlugCall ${Spawn[${MsgText.Right[-7]}].CleanName}
	  }
   }   
   /varset FollowName ${Spawn[${FollowID}].Name}
   /varset FollowFlag ${If[!${FollowID} || ${Spawn[${FollowID}].Type.Equal[corpse]},0,1]}
   /call AdvPathCall
   /if (${MsgText.Equal[follow close]} && ${FollowID}) {
      /if (${AdvPath.Active}) /afollow off
      /if (${Target.ID}!=${FollowID}) {
         /tar id ${FollowID}
         /delay 5 ${Target.ID}==${FollowID}
      }
      /squelch /stick 5 uw
   }
   /if (${Stick.Status.Equal[on]} || ${AdvPath.Following}) /bc Msg: AFing ${Spawn[${FollowID}].CleanName}
/return


Sub Command_GETBANK(string MsgText)
   /if (${Cursor.ID}) {
      /call GetBankItem "${Cursor.Name}" ${MsgText.Right[-7]}
   } else /if (${MsgText.Right[-7].Length}) {
      /call GetBankItem ${MsgText.Right[-7]}
   } else {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Usage - Hold Item to get on Cursor or getbank <ItemName> <InStacks> <Quantity>
   }
/return


Sub Command_GIVE(string MsgText)
   /if (${MsgText.Right[-5].Length}) {
      /bc Msg: Giving items to ${MsgText.Arg[2]}
      /call GiveCheck ${MsgText.Right[-5]}
   }
/return


Sub Command_GOTO(string MsgText)
   /if (${MsgText.Arg[2].Length} && ${MsgText.Arg[3].Length}) {
      /if (!${Me.Standing}) /stand
      /moveto loc ${MsgText.Arg[2]} ${MsgText.Arg[3]}
   }
/return


Sub Command_HANDIN(string MsgText)
   /declare tTarget string local ${Target.CleanName}
   /declare cItem string local ${Cursor.Name}
   /if (${MsgText.Arg[2].Length}) /varset tTarget ${MsgText.Arg[2]}
   /if (${MsgText.Arg[3].Length}) /varset cItem ${MsgText.Arg[3]}
   /if (!${tTarget.Length} || !${cItem.Length}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Incorrect usage.  /mb handin "TargetName" "ItemName" or hold item and target.
   } else {  
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Handing in all ${Cursor.Name}'s to ${Target.CleanName}
      /call GiveCheck "${tTarget}" "${cItem}" all
   }
/return


Sub Command_HELP(string MsgText)
   /if (!${MsgText.Right[-4].Length}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Modbot command list.  Use "/mb help command" for specific help on each.
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Simple Toggle Commands: ${OnOffCommands}
	  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Other ModBot Commands : ${ActionCommands}
   } else {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}This feature is currently under construction.
   }
/return


Sub Command_HAILDELAY(P0)
   /declare delaytarget int local ${NetBots[${SenderName}].TargetID}
   /if (!${delaytarget}) {
      /bc Msg: No Target for HailDelay
   } else /if (${Spawn[${delaytarget}].Type.Equal[NPC]} && ${Spawn[${delaytarget}].Distance3D}<25) {
      /declare haildelay int local ${Math.Calc[${Select[${Me.Name},${NetBots.Client.Replace[ ,,]}]}*15+${Math.Rand[2,5]}]}
      /delay ${haildelay}
      /target id ${delaytarget}
      /delay 1s ${Target.ID}==${delaytarget}
	  /if (${Target.ID}==${delaytarget}) {
	     /hail
	     /bc Msg: HailDelay - Hailed
	  } else {
		 /bc Msg: Targetting Failure for HailDelay
	  }
   } else {
	  /bc Msg: Invalid Target for HailDelay - ${Spawn[${delaytarget}].Name} (${Spawn[${delaytarget}].Type} / ${Spawn[${delaytarget}].Distance3D})
   }
/return	  


Sub Command_HOLDUP(P0)
   /bc Setting - DoBuffs,DoDebuffs,DoEvents,DoMelee,DoCures,DoLoot,DoPull FALSE
   /varset DoDebuffs FALSE
   /varset DoEvents FALSE
   /varset DoHeals TRUE
   /if (!${Me.Class.ShortName.Equal[brd]}) /varset DoBuffs FALSE
   /varset DoMelee FALSE
   /varset DoCures FALSE
   /varset DoPull FALSE
   /varset DoLoot FALSE
/return


Sub Command_INCLUDE(string MsgText)
   /bc Msg: Including ${MsgText.Right[-8]}
   /if (${ExcludeList.Find[${MsgText.Right[-8]}]}) {
      /declare sVarM string local ${MsgText.Right[-8]}|
      /if (${ExcludeList.Find[#${MsgText.Right[-8]}]}) /varset sVarM #${sVarM}
      /declare sVarL string local ${ExcludeList.Mid[1,${Math.Calc[${ExcludeList.Find[${sVarM}]}-1]}]}
      /declare sVarR string local ${ExcludeList.Mid[${Math.Calc[${sVarL.Length}+${sVarM.Length}+1]},${ExcludeList.Length}]}
      /varset ExcludeList ${sVarL}${sVarR}
   }
   /call ProcessExcludeList
/return


Sub Command_INCLUDEID(string MsgText)
   /declare sID string local ${MsgText.Right[-10]}
   /if (${ExcludeListID.Find[|${sID}|]}) {
      /declare sVarM string local ${sID}|
      /declare sVarL string local ${ExcludeListID.Mid[1,${Math.Calc[${ExcludeListID.Find[${sVarM}]}-1]}]}
      /declare sVarR string local ${ExcludeListID.Mid[${Math.Calc[${sVarL.Length}+${sVarM.Length}+1]},${ExcludeListID.Length}]}
      /varset ExcludeListID ${sVarL}${sVarR}
   }
   /squelch /alert remove ${AlertList} id ${sID}
/return


Sub Command_INVITEALL(string MsgText)
   /declare i int local
   /if (${MsgText.Find[ ]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Starting modbot on channel - ${MsgText.Arg[2]}
      /bct ${MsgText.Arg[2]} //mac modbot
   } else {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Starting modbot and inviting all clients.
      /noparse /bca //if (!${Zone.Name.Equal[The Bazaar]}) /mac modbot
   }
   /delay 15s
   /for i 1 to ${Math.Calc[${NetBots.Counts}]}
      /if (${NetBots.Client.Arg[${i}].NotEqual[${Me.Name}]} && !${Group.Member[${NetBots.Client.Arg[${i}]}].Index}) { 
         /invite ${NetBots.Client.Arg[${i}]}
         /delay ${Math.Rand[20]}
      }
      /if (${i}==6) /break
   /next i
/return


Sub Command_LETSROLL(string MsgText)
   /bc Setting - DoCures,DoBuffs,DoHeals,DoMelee,DoEvents,DoDebuffs TRUE
   /varset DoDebuffs TRUE
   /varset DoHeals TRUE
   /varset DoMelee TRUE
   /varset DoEvents TRUE
   /varset DoBuffs TRUE
   /varset DoCures TRUE
   /if (${MsgText.Equal[letsroll unpause]}) {
      /bc Setting - MBPause FALSE
      /varset MBPause FALSE
   }
/return


Sub Command_MAKECAMP(P0)
   /squelch /stick off
   /if (${AdvPath.Following}) /afollow off
   /varset FollowFlag 0
   /makecamp on
   /varset MakeCampX ${Me.X}
   /varset MakeCampY ${Me.Y}
   /varset MakeCampD ${Me.Heading.DegreesCCW}
   /varset CampStatus 1
   /bc Msg: Hanging out
/return


Sub Command_MANA(P0)
   /bc Msg: ${Me.PctMana}%
/return


Sub Command_MBPAUSE(string MsgText)
   /if (${Select[${MsgText.Arg[2]},on,true]} && ${ACState}) {
      /varcalc ACState ${ACState}+2
      /call AdvCombat
   }
   /call TFToggle MBPause ${MsgText.Arg[2]}
   /if (${MBPause}) {
      /if (${CampStatus}) /makecamp off
      /if (${Me.Class.ShortName.Equal[brd]}) {
         /if (${Twist}) /twist off
	  }
   }
/return


Sub Command_MBWAYPLAY(string MsgText)
   /call MBWayPlay ${MsgText.Right[-10]}
   /if (${Macro.Return.Equal[false]}) {
      /bc Msg: Problem with playback
   }
/return


Sub Command_MBWAYREC(string MsgText)
   /call MBWayRecord ${MsgText.Right[-9]}
/return


Sub Command_PETTOYS(P0)
   /if (${Defined[DoPetToys]}) {
      /if (!${DoPetToys}) /return
      /if (${Spawn[pc ${SenderName}].Pet.ID}) {
         /declare x int local ${Me.X.Int}
         /declare y int local ${Me.Y.Int}
         /call PetToys "${Spawn[pc ${SenderName}].Pet.ID}"
         /if (${Macro.Return.NotEqual[TRUE]}) {
		    /bc Msg: There seems to be a PetToy problem
		 }
         /call GiveCleanup
         /if (${Me.X.Int}!=${x} || ${Me.Y.Int}!=${y}) {
		    /call MBMoveTo ${y} ${x}
		 }
      } else {
         /bc Msg: No Pet found
      }
   }
/return


Sub Command_RECAST(string MsgText)
   /declare b int local 0
   /declare c int local 0
   /for b 1 to ${ABCount}
      /for c 1 to ${Math.Calc[${ABSpellAlias[${b}].Count[|]}+1]}
         /if (${MsgText.Arg[2].Equal[${ABSpellAlias[${b}].Arg[${c},|]}]}) {
            /if (!${ABRecast[${b}]}) {
               /varset ABRecast[${b}] TRUE
               /bc Msg: ${ABSpell[${b}]} will now recast when worn off.
            }
         }
      /next c
   /next b
/return


Sub Command_SAYDELAY(string MsgText)
   /declare delaytarget int local ${NetBots[${SenderName}].TargetID}
   /if (!${delaytarget}) {
      /bc Msg: No Target for SayDelay
   } else /if (!${MsgText.Right[-9].Length}) {
      /bc Msg: No Text for SayDelay
   } else /if (${Spawn[${delaytarget}].Type.Equal[NPC]} && ${Spawn[${delaytarget}].Distance3D}<25) {
      /declare saydelay int local ${Math.Calc[${Select[${Me.Name},${NetBots.Client.Replace[ ,,]}]}*15+${Math.Rand[2,5]}]}
      /delay ${saydelay}
      /target id ${delaytarget}
      /delay 1s ${Target.ID}==${delaytarget}
	  /if (${Target.ID}==${delaytarget}) {
	     /say ${MsgText.Right[-9]}
	     /bc Msg: SayDelay - ${MsgText.Right[-9]}
	  } else {
		 /bc Msg: Targetting Failure for SayDelay
	  }
   } else {
	  /bc Msg: Invalid Target for SayDelay - ${Spawn[${delaytarget}].Name} (${Spawn[${delaytarget}].Type} / ${Spawn[${delaytarget}].Distance3D})
   }
/return	  


Sub Command_SAYTARGET(string MsgText)
   /if (${NetBots[${SenderName}].TargetID}) {
      /declare saytarget int local ${NetBots[${SenderName}].TargetID}
      /target id ${saytarget}
      /delay 1s ${Target.ID}==${saytarget}
      /if (${Target.Distance} > 10) /stick 5
      /delay 1s ${Target.Distance} < 10
      /stick off
      /say ${MsgText.Right[-10]}
   }
/return


Sub Command_SCRIPT(string MsgText)
  /call MBScript "${MsgText.Right[-7]}" ${SenderName}
/return


Sub Command_SEARCH(string MsgText)
   /declare SearchText string local ${MsgText.Right[-7]}
   /declare ReplyText string local ${FindItemCount[${SearchText}]} on me and ${FindItemBankCount[${SearchText}]} in my bank. 
   /declare Count1 int local ${FindItemCount[${SearchText}]}
   /declare BCount1 int local ${FindItemBankCount[${SearchText}]}
   /if (${Count1}>0 && ${FindItemCount[=${SearchText}]}!=${Count1} || ${BCount1}>0 && ${FindItemBankCount[=${SearchText}]}!=${BCount1}) {
      /varset ReplyText ${ReplyText} - Probably different items (generic search matches).
   }
   /bc ${ReplyText}
/return


Sub Command_SELL(string MsgText)
   /if (!${MsgText.Right[-5].Length} && ${Target.Class.Name.Equal[Merchant]}) {
      /declare MerchName string local ${Target.CleanName}
   } else /if (${MsgText.Right[-5].Length}) {
      /declare MerchName string local ${MsgText.Right[-5]}
   } else /if (${Spawn[Merchant].Distance} < 20) {
      /declare MerchName string local ${Spawn[Merchant].Name}
   }
   /bc Msg: Selling items to ${MerchName}
   /call SellCheck "${MerchName}"
/return


Sub Command_SETINIVAR(string MsgText)
   /if (!${MsgText.Arg[2].Length} || !${MsgText.Arg[3].Length}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Usage: /mb setinivar <Varname> <Setting>  (use quotes around multiple word settings)
      /return
   }
   /declare IniSection string local
   /declare a int local
   /declare b int local
   /declare IniVar string local ${MsgText.Arg[2]}
   /declare IniSetting string local ${MsgText.Arg[3]}
   /declare VarNumber int local
   /declare tSection string local Settings Melee AdvHeal AdvDebuff AdvBuff AdvEvent AdvCure AdvPull AdvCure
   /declare tClass string local
   /declare tVar string local
   /declare tNumCnt int local
   /if (${IniVar.Right[1].Equal[]]} && ${IniVar.Find[[]}) {
      /varset VarNumber ${MsgText.Arg[2].Arg[2,[].Left[-1]}
      /varset IniVar ${MsgText.Arg[2].Arg[1,[]}${VarNumber}
   }
   /if (${MsgText.Arg[4].Length}) {
      /for b 4 to ${Math.Calc[${MsgText.Count[ ]}+1]}
         /varset IniSetting ${IniSetting} ${MsgText.Arg[${b}]}
      /next b
   }
   /for a 1 to 9
      /varset IniSection ${tSection.Arg[${a}]}
      /if (!${IniSection.Length}) /return
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${IniFile},${IniSection}
      /for b 1 to ${Ini["${IniFile}",${IniSection}].Count[|]}+1
         /if (${Ini["${IniFile}",${IniSection}].Arg[${b},|].Equal[${IniVar}]}) {
            /ini "${IniFile}" "${IniSection}" "${IniVar}" "${IniSetting}"
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You're setting for ${MsgText.Arg[2]} is now ${Ini["${IniFile}",${IniSection},${IniVar}]} in your .ini file.
            /if (${Defined[${MsgText.Arg[2]}]}) {
               /varset ${MsgText.Arg[2]} ${IniSetting}
            } else {
               /varset IniVar ${MsgText.Arg[2].Arg[1,[]}
               /if (!${Defined[${IniVar}]}) {
                  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Cannot set variable in memory.  Defined variable does not match. -Restart required to set it.
                  /return
               }
               /varset ${MsgText.Arg[2]} ${IniSetting}
            }
            /return
         }
      /next b
   /next a
   /varset tClass ${IniVar.Left[2]}
   /if (${Select[${tClass},AH,AD,AB,AE,AQ]} && ${${tClass}NewFormat} && ${Select[${IniVar.Right[1]},1,2,3,4,5,6,7,8,9,0]}) {
      /varset tNumCnt 1
:IniNextNum
      /varcalc a ${IniVar.Length}-${tNumCnt}
      /if (${Select[${IniVar.Mid[${a},1]},1,2,3,4,5,6,7,8,9,0]}) {
         /varcalc tNumCnt ${tNumCnt}+1
         /goto :IniNextNum
      }
      /varset tVar ${IniVar.Left[-${tNumCnt}]}
      /varset tVar ${tVar.Right[-2]}
      /varset IniSection ${tClass}${IniVar.Right[${tNumCnt}]}
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${IniFile},${IniSection}
      /for b 1 to ${Ini["${IniFile}",${IniSection}].Count[|]}+1
         /if (${Ini["${IniFile}",${IniSection}].Arg[${b},|].Equal[${tVar}]}) {
            /ini "${IniFile}" "${IniSection}" "${tVar}" "${IniSetting}"
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You're setting for ${MsgText.Arg[2]} is now ${Ini["${IniFile}",${IniSection},${tVar}]} in your .ini file.
            /if (${Defined[${MsgText.Arg[2]}]}) {
               /varset ${MsgText.Arg[2]} ${IniSetting}
            } else {
               /varset IniVar ${MsgText.Arg[2].Arg[1,[]}
               /if (!${Defined[${IniVar}]}) {
                  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Cannot set variable in memory.  Defined variable does not match. -Restart required to set it.
                  /return
               }
               /varset ${MsgText.Arg[2]} ${IniSetting}
            }
            /return
         }
      /next b
   }   
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Variable not found
/return


Sub Command_SETPULLER(string MsgText)
   /declare s string local ${MsgText.Right[-10]}
   /if (${s.Length}) {
      /varset PullerName ${s.Left[1].Upper}${s.Right[-1]}
      /if (${ManualPuller}) {
	     /bc Msg: ${PullerName} is now PULLER
	  } else {
         /varset ManualPuller TRUE
		 /bc Msg: ${PullerName} is now PULLER (Ignoring Group Puller Role)
	  }
   } else {
      /bc Msg: Clearing/Resetting Puller (Group Roles will be considered)
	  /varset ManualPuller FALSE
	  /varset PullerName
	  /call SetPuller
	  /if (!${PullerName.Length}) {
	     /bc Msg: NO Puller Assigned
	  }
   }
/return


Sub Command_SETTANK(string MsgText)
   /declare s string local ${MsgText.Right[-8]}
   /if (${s.Length}) {
      /varset TankName ${s.Left[1].Upper}${s.Right[-1]}
      /if (${ManualTank}) {
	     /bc Msg: ${TankName} is now TANK
	  } else {
         /varset ManualTank TRUE
		 /bc Msg: ${TankName} is now TANK (Ignoring Group MA/MT Roles)
	  }
   } else {
      /bc Msg: Clearing/Resetting Tank (Group Roles will be considered)
	  /varset ManualTank FALSE
	  /varset TankName
	  /call SetTank
	  /if (!${TankName.Length}) {
	     /bc Msg: NO Tank Assigned
	  }
   }
/return


Sub Command_STOP(P0)
   /if (${Select[${ACState},2,4]}) {
      /varset ACState 1
   }
   /if (${AdvPath.Following} || ${AdvPath.Paused}) {
      /call AdvPlugCall off
   } else {
      /squelch /stick off
      /if (${CampStatus}==1) {
         /varset MakeCampX ${Me.X}
         /varset MakeCampY ${Me.Y}
         /varset MakeCampD ${Me.Heading.DegreesCCW}
         /makecamp on
      } else {
         /makecamp off
      }
   }
   /varset FollowFlag 0
   /varset FollowID 0
   /varset FollowName
   /varset ACStartY ${Me.Y}
   /varset ACStartX ${Me.X}
   /varset ACStartHeading ${Me.Heading.DegreesCCW}
   /if (!${AdvPath.Following} || ${Stick.Status.Equal[off]}) {
      /bc Msg: No longer on AF
   }
/return


Sub Command_SWITCH(string MsgText)
   /varset ACSwitchID 0
   /if (!${MsgText.Arg[2].Length}) {
      /if (${NetBots[${SenderName}].TargetID}) {
         /varset ACSwitchID ${NetBots[${SenderName}].TargetID} 
      } else /if (${NetBots[${PullerName}].TargetID}) {
         /varset ACSwitchID ${NetBots[${PullerName}].TargetID}
      } else {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}No Switch Target or Sender/Puller Target
         /return
      }
   } else {
      /if (${Spawn[npc radius 100 id ${MsgText.Arg[2]}].ID}) {
         /varset ACSwitchID ${Spawn[${MsgText.Arg[2]}].ID}
      } else {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Switch Spawn not found
         /return
      }
   }
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Switching to ${Spawn[${ACSwitchID}]} (#${ACSwitchID})
|  /varset ACState 100
   /varset DoMelee TRUE
   /varset ACMATarget ${ACSwitchID}
   /squelch /melee reset
   /if (${Me.Pet.ID}) /pet back off
   /target id ${ACMATarget}
   /delay 2s ${Target.ID}==${ACMATarget}
   /if (${Spawn[${ACMATarget}].Distance3D} > ${ACLeash}) /return
   /if (${ACManaPct}!=101) {
      /stick id ${ACMATarget}
      /attack on
   }
   /if (${ACMATarget}) {
      /if (${Me.Pet.ID} && ${PetAssist}) {
         /pet attack
         /varset PetAssist ${ACMATarget}
      }
      /if (${SPetAssist}) {
         /pet swarm
         /varset SPetAssist ${ACMATarget}
      }
   }
/return


Sub Command_TANKANDHEALS
   /if (${TankName.Equal[${Me.Name}]}) {
      /varset DoMelee TRUE
      /varset DoDebuffs TRUE
      /varset DoEvents TRUE
   } else {
      /varset DoMelee FALSE
      /if (${ACState}) {
         /varcalc ACState ${ACState}+2
         /call AdvCombat
      }
      /squelch /melee reset
      /varset ACMATarget 0
      /if (${Me.Pet.ID}) /pet back off
      /varset DoDebuffs FALSE
      /varset DoEvents FALSE
   }
   /varset DoHeals TRUE
   /varset DoCures TRUE
   /varset DoBuffs TRUE
   /bc Msg: Setting Tank-only Melee/Debuffs. Heals/Cures/Buffs from others.
   /if (${MsgText.Equal[tankandheals unpause]}) {
      /bc Setting - MBPause FALSE
      /varset MBPause FALSE
   }
/return
