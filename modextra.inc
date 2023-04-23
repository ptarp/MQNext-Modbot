
|-------------------------------------------------------
| Some extra things - Not necessary for ModBot.
|-------------------------------------------------------

| Bind xbots - The /xbots command will fill the XTargets (bottom-up) with the other characters on NetBots (not in my group), auto-hate the rest.
#bind XBots /xbots
Sub Bind_XBots
   /declare b int local
   /declare x int local
   /declare sNBClnt string local
   /if (!${NetBots.Counts} || !${Me.XTargetSlots}) /return
   /varset x ${Me.XTargetSlots}
   /for b 1 to ${NetBots.Counts}
      /if (${x}<1) /return
      /varset sNBClnt ${NetBots.Client.Arg[${b}]}
      /if (!${sNBClnt.Equal[${Me.CleanName}]} && !${NetBots[${sNBClnt}].InGroup}) {
         /docommand /xtarget set ${x} ${sNBClnt}
         /varcalc x ${x}-1
      }
   /next b
   /if (${x}<1) /return
   /for b ${x} downto 1
      /docommand /xtarget set ${b} AH
   /next b
/return


| Bind - autoskills - The /autoskills command will list the combat skills you currently have on /autoskill.
#bind AutoSkills /autoskills
Sub Bind_AutoSkills
   /declare used int local
   /declare x int local
| Currently, only two concurrent skills allowed
   /for x 1 to 2
      /if (${Me.AutoSkill[${x}].Name.NotEqual[NULL]}) {
         /varcalc used ${used}+1
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}AutoSkill ${used} -> ${Me.AutoSkill[${x}]}
      }
   /next x
   /if (${used}<1) /echo ${If[${TimeStampOnEcho},[${Time}] ,]}No AutoSkills in use
/return


| Bind - lteach - The /lteach command will teach a language
#bind LTeach /lteach
Sub Bind_LTeach
   /declare i int local
   /for i 1 to 100
      /g Teaching Language ${i}
      /delay 2
   /next i
/return


| Bind - checkinv - The /checkinv command will /bc if you have an item on yourself or in the bank
#bind CheckInv /checkinv
Sub Bind_CheckInv(x)
   /bc Msg: ${x} - ${FindItemCount[=${x}]} on me. ${FindItemBankCount[=${x}]} in bank.
/return


| Bind - droplev - The /droplev command will attempt to strip Levitation buffs from you and your pet.
#bind DropLev /droplev
Sub Bind_DropLev
   /declare i int local
   /for i 1 to ${Me.MaxBuffSlots}
      /if (${Me.Buff[${i}].HasSPA[57]}) {
         /removebuff ${Me.Buff[${i}]}
         /delay 5
      }
   /next i
   /for i 1 to 30
      /if (${Me.Song[${i}].HasSPA[57]}) {
         /removebuff ${Me.Song[${i}]}
         /delay 5
      }
   /next i
   /if (${Me.Pet.ID}) {
      /for i 1 to 97
         /if (${Me.PetBuff[${i}].HasSPA[57]}) {
            /removepetbuff ${Me.PetBuff[${i}]}
            /delay 5
         }
      /next i
   }
/return
