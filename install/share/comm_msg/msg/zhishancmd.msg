#1:save init position 2:save goal position
#3:navigation goals 4:commision release
#5:save call position 6:one key call
#7：set init position 8：voice
#9：clear navigation goals in yaml
#10: turn voice 
#11:touch screen
#12:miaozhun
#13:shutdown
#14:reboot
#15:inner turn left right and back
#16:start run after speaking finished,receive from other node,jixuqianjin
#17:start speak,send to other node
#18:patrol is finished
#19:yuan di deng dai
#20：dance
uint8 zhishanCommand
#1:auto charge 2:charge finish 3:send command to serial to prepare charge
uint8 autoCharge
#1: rise 2:fall 3:head turn left 4:head turn right 5:arm up 6:arm down 7:dance 8：stop dance
uint8 rodControl
# reserve to use 
# voice8 1：interact 2：play mode 3:play voice 4：interupt 5:patrol and play
# 6:stop play image after voice is finished 7:patrol and speak
# turn voice 10: 1:up 2:down
# touch screen11：1:enable 2:disable
# turn15: 1 left 2 right 3 back 
uint8 zhishanPara
# voice
# zhishanCommand 8：voice 5:patrol and play zhishanVoice = number
uint8 zhishanVoice
