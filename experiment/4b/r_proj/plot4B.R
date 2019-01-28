# Experiments for 4B
library(tidyverse)
library(ggplot2)
library(cowplot) 


## Adaptation goals
r <- 0.90
rmin <- r*0.98
rmax <- r*1.02

c <- 0.47
cmin <- c*0.98
cmax <- c*1.02

###########################################################
##          Plot constant vs. tamed scenario             ##
###########################################################
#Our transformation function
scaleFUN <- function(x) sprintf("%.2f", x)

nt_as_c_mc_reli <- ggplot() +
  #geom_line(data=nt_fr_c_m_data, aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*25, color = "available sensors"), size=0.5) +
  geom_hline(yintercept = rmin*100, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(aes(yintercept = r*100, color = "set point")) +
  geom_hline(yintercept = rmax*100, color = 'black', linetype=2, alpha=0.3) +
  #geom_line(data=nt_fr_c_m_data, aes(TIME..ms./1000, RELIABILITY*100, color="untamed"), size=1) +
  stat_smooth(data=nt_fr_c_m_data, aes(TIME..ms./1000, RELIABILITY*100, color="untamed"), method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  #geom_line(data=t_fr_c_m_data, aes(TIME..ms./1000, RELIABILITY*100, color="tamed"), size=1) +
  stat_smooth(data=t_fr_c_m_data, aes(TIME..ms./1000, RELIABILITY*100, color="tamed"), method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  #geom_text(aes(1, rmin*100, label = 'setpoint', vjust = -1), size=3)+
  scale_color_manual(values=c("tamed"='blue',"untamed"='red',"set point"='black'), name = "") +
  scale_y_continuous(breaks=sort(c(seq(0,100,length.out=5), r*100))) +
  labs(x="", y="reliability (%)") +  
  theme(legend.position='bottom', axis.text=element_text(size=10)) +
  coord_cartesian(xlim=c(0, 300), ylim=c(0, 100))

nt_as_c_mc_cost <- ggplot() +
  #geom_line(data=nt_fr_c_m_data, aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*0.175, color="available sensors"), size=0.5) +
  geom_hline(yintercept = cmin, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(aes(yintercept = c, color = "set point")) +
  geom_hline(yintercept = cmax, color = 'black', linetype=2, alpha=0.3) +
  #geom_line(data=nt_fr_c_m_data, aes(TIME..ms./1000, COST, color="untamed"), size=1) +
  stat_smooth(data=nt_fr_c_m_data, aes(TIME..ms./1000, COST, color="untamed"), method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  #geom_line(data=t_fr_c_m_data, aes(TIME..ms./1000, COST, color="tamed"), size=1) +
  stat_smooth(data=t_fr_c_m_data, aes(TIME..ms./1000, COST, color="tamed"), method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  #geom_text(aes(1, cmin, label = 'setpoint', vjust = -1), size=3)+
  scale_color_manual(values=c("tamed"='blue',"untamed"='red',"set point"='black'), name = "") +
  scale_y_continuous(labels=scaleFUN, breaks=sort(c(seq(0,0.7,length.out=5), c))) +
  labs(x="time (s)", y="cost (W)") +  
  theme(legend.position='none', axis.text=element_text(size=10)) +
  coord_cartesian(xlim=c(0, 300), ylim=c(0, 0.70))

legend <- get_legend(nt_as_c_mc_reli)
nt_as_c_mc_pg <- plot_grid(nt_as_c_mc_reli+theme(legend.position='none'),nt_as_c_mc_cost, ncol=1)
#plot_grid(nt_as_c_mc_pg, legend, ncol=1, rel_heights=c(1,0.05))
###########################################################
##          Plot constant vs. tamed scenario             ##
###########################################################
