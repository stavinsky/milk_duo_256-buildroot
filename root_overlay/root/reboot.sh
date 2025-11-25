CTRL_BASE=0x05025000                                                                     
RTC_BASE=0x5026000    # CTRL_BASE + 0x1000                                               
                                                                                         
# 1) select 32k domain                                                                   
devmem $((RTC_BASE + 0x3C)) 32 0x1                                                       
                                                                                         
# 2) unlock CTRL0                                                                        
devmem $((CTRL_BASE + 0x4)) 32 0xAB18                                                    
                                                                                         
# 3) enable warm reset request                                                           
devmem $((RTC_BASE + 0xCC)) 32 0x1                                                       
                                                                                         
# 4) optional: poll FSM_STATE (RTC_BASE + 0xD4) == 3 (ST_ON) with devmem                 
                                                                                         
# 5) trigger warm reset                                                                  
devmem $((CTRL_BASE + 0x8)) 32 $((0xFFFF0800 | (1 << 4)))   
