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
##             Plot uncontrolled scenario                ##
###########################################################
uc_m_reli <- uc_m_data %>%
  mutate(color = if_else(RELIABILITY>=rmin & RELIABILITY<=rmax, "ok", "not ok")) %>%
  ggplot() + 
  geom_point(aes(TIME..ms./1000,RELIABILITY*100, color=color), size=1) +
  stat_smooth(aes(TIME..ms./1000,RELIABILITY*100), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_hline(yintercept = rmin*100, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = r*100, color = 'black') +
  geom_hline(yintercept = rmax*100, color = 'black', linetype=2, alpha=0.3) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*25), alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./25, name="active sensors")) +
  scale_color_manual(values = c("ok" = 'blue', "not ok" = 'red'), name = "") +
  labs(title="uncontrolled bsn behavior", x="time (s)", y="reliability (%)") +
  coord_cartesian(xlim=c(0, 300), ylim=c(0, 100))

uc_m_cost <- uc_m_data %>%
  mutate(color = if_else(cmin<=COST & COST<=cmax,"ok", "not ok")) %>%
  ggplot() + 
  geom_point(aes(TIME..ms./1000, COST, color=color), size=1) +
  stat_smooth(aes(TIME..ms./1000, COST), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_hline(yintercept = cmin, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = c, color = 'black') +
  geom_hline(yintercept = cmax, color = 'black', linetype=2, alpha=0.3) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*0.15), alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./0.15, name="active sensors")) +
  scale_color_manual(values = c("ok" = 'blue', "not ok" = 'red'), name = "") +
  labs(x="time (s)", y="cost") + 
  theme(legend.position='none') +
  coord_cartesian(xlim=c(0, 300), ylim=c(0, 0.65))

legend <- get_legend(uc_m_reli)
uc_m_pg <- plot_grid(uc_m_reli+theme(legend.position='none'),uc_m_cost, ncol=1)
plot_grid(uc_m_pg, legend, nrow=1, align='h', rel_widths=c(1,0.2))

###########################################################
##             Plot uncontrolled scenario                ##
###########################################################

###########################################################
##         Plot taming uncertainties scenario            ##
###########################################################
t_a_m_reli <- t_a_m_data %>%
  mutate(color = if_else(RELIABILITY>=rmin & RELIABILITY<=rmax, "ok", "not ok")) %>%
  ggplot() +
  geom_point(aes(TIME..ms./1000,RELIABILITY*100, color=color), size=1) +
  stat_smooth(aes(TIME..ms./1000,RELIABILITY*100), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_hline(yintercept = rmin*100, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = r*100, color = 'black') +
  geom_hline(yintercept = rmax*100, color = 'black', linetype=2, alpha=0.3) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*25), alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./25), name="active sensors") +
  scale_color_manual(values = c("ok" = 'blue', "not ok" = 'red'), name = "") +
  labs(title="bsn behavior for all uncertainties tamed", x="time (s)", y="reliability (%)") +
  coord_cartesian(xlim=c(0, 300), ylim=c(0, 100))

t_a_m_cost <- t_a_m_data %>%
  mutate(color = if_else(cmin<=COST & COST<=cmax,"ok", "not ok")) %>%
  ggplot() +
  geom_point(aes(TIME..ms./1000, COST, color=color), size=1) +
  stat_smooth(aes(TIME..ms./1000, COST), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_hline(yintercept = cmin, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = c, color = 'black') +
  geom_hline(yintercept = cmax, color = 'black', linetype=2, alpha=0.3) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*0.15), alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./0.15), name="active sensors") +
  scale_color_manual(values = c("ok" = 'blue', "not ok" = 'red'), name = "") +
  labs(x="time (s)", y="cost") + 
  theme(legend.position='none') +
  coord_cartesian(xlim=c(0, 300), ylim=c(0, 0.65))

legend <- get_legend(t_a_m_reli)
t_a_m_pg <- plot_grid(t_a_m_reli+theme(legend.position='none'),t_a_m_cost, ncol=1)
plot_grid(t_a_m_pg, legend, nrow=1, align='h', rel_widths=c(1,0.2))
###########################################################
##         Plot taming uncertainties scenario            ##
###########################################################

###########################################################
##       Plot randomized actives sensors scenario        ##
###########################################################
nt_as_gs_m_reli <- nt_as_gs_m_data %>%
  mutate(color = if_else(RELIABILITY>=rmin & RELIABILITY<=rmax, "ok", "not ok")) %>%
  ggplot() +
  geom_point(aes(TIME..ms./1000,RELIABILITY*100, color=color), size=1) +
  stat_smooth(aes(TIME..ms./1000,RELIABILITY*100), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*25), alpha=0.3) +
  geom_hline(yintercept = rmin*100, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = r*100, color = 'black') +
  geom_hline(yintercept = rmax*100, color = 'black', linetype=2, alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./25), name="active sensors") +
  scale_color_manual(values = c("ok" = 'blue', "not ok" = 'red', "model" = 'orange'), name = "") +
  labs(title="bsn behavior for randomized active sensors", x="time (s)", y="reliability (%)") +
  coord_cartesian(xlim=c(0, 300), ylim=c(0, 100))

nt_as_gs_m_cost <- nt_as_gs_m_data %>%
  mutate(color = if_else(cmin<=COST & COST<=cmax,"ok", "not ok")) %>%
  ggplot() +
  geom_point(aes(TIME..ms./1000, COST, color=color), size=1) +
  stat_smooth(aes(TIME..ms./1000, COST), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_hline(yintercept = cmin, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = c, color = 'black') +
  geom_hline(yintercept = cmax, color = 'black', linetype=2, alpha=0.3) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*0.15), alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./0.15), name="active sensors") +
  scale_color_manual(values = c("ok" = 'blue', "not ok" = 'red'), name = "") +
  labs(x="time (s)", y="cost") + 
  theme(legend.position='none') +
  coord_cartesian(xlim=c(0, 300), ylim=c(0, 0.65))

legend <- get_legend(nt_as_gs_m_reli)
nt_as_gs_m_pg <- plot_grid(nt_as_gs_m_reli+theme(legend.position='none'),nt_as_gs_m_cost, ncol=1)
plot_grid(nt_as_gs_m_pg, legend, nrow=1, align='h', rel_widths=c(1,0.2))

nt_as_gs_c_reli <- nt_as_gs_c_data %>%
  mutate(color = if_else(RELIABILITY>=rmin & RELIABILITY<=rmax, "ok", "not ok")) %>%
  ggplot() +
  geom_point(aes(TIME..ms./1000,RELIABILITY*100, color=color), size=1) +
  stat_smooth(aes(TIME..ms./1000,RELIABILITY*100), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*25), alpha=0.3) +
  geom_hline(yintercept = rmin*100, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = r*100, color = 'black') +
  geom_hline(yintercept = rmax*100, color = 'black', linetype=2, alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./25), name="active sensors") +
  scale_color_manual(values = c("ok" = 'blue', "not ok" = 'red', "model" = 'orange'), name = "") +
  labs(title="model behavior for randomized active sensors", x="time (s)", y="reliability (%)") +
  coord_cartesian(xlim=c(0, 300), ylim=c(0, 100))

nt_as_gs_c_cost <- nt_as_gs_c_data %>%
  mutate(color = if_else(cmin<=COST & COST<=cmax,"ok", "not ok")) %>%
  ggplot() +
  geom_point(aes(TIME..ms./1000, COST, color=color), size=1) +
  stat_smooth(aes(TIME..ms./1000, COST), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_hline(yintercept = cmin, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = c, color = 'black') +
  geom_hline(yintercept = cmax, color = 'black', linetype=2, alpha=0.3) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*0.15), alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./0.15), name="active sensors") +
  scale_color_manual(values = c("ok" = 'blue', "not ok" = 'red'), name = "") +
  labs(x="time (s)", y="cost") + 
  theme(legend.position='none') +
  coord_cartesian(xlim=c(0, 300), ylim=c(0, 0.65))

legend <- get_legend(nt_as_gs_c_reli)
nt_as_gs_c_pg <- plot_grid(nt_as_gs_c_reli+theme(legend.position='none'),nt_as_gs_c_cost, ncol=1)
plot_grid(nt_as_gs_c_pg, legend, nrow=1, align='h', rel_widths=c(1,0.2))

plot_grid(nt_as_gs_c_pg, nt_as_gs_m_pg, legend, nrow=1, align='h', rel_widths=c(1,1,0.2))
###########################################################
##       Plot randomized actives sensors scenario        ##
###########################################################

###########################################################
##       Plot constant actives sensors scenario          ##
###########################################################
nt_as_c_m_reli <- nt_as_c_m_data %>%
  mutate(color = if_else(RELIABILITY>=rmin & RELIABILITY<=rmax, "ok", "not ok")) %>%
  ggplot() +
  geom_point(aes(TIME..ms./1000,RELIABILITY*100, color=color), size=1) +
  stat_smooth(aes(TIME..ms./1000,RELIABILITY*100), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*25), alpha=0.3) +
  geom_hline(yintercept = rmin*100, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = r*100, color = 'black') +
  geom_hline(yintercept = rmax*100, color = 'black', linetype=2, alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./25), name="active sensors") +
  scale_color_manual(values = c("ok" = 'blue', "not ok" = 'red', "model" = 'orange'), name = "") +
  labs(title="bsn behavior for randomized active sensors", x="time (s)", y="reliability (%)") +
  coord_cartesian(xlim=c(0, 300), ylim=c(0, 100))

nt_as_c_m_cost <- nt_as_c_m_data %>%
  mutate(color = if_else(cmin<=COST & COST<=cmax,"ok", "not ok")) %>%
  ggplot() +
  geom_point(aes(TIME..ms./1000, COST, color=color), size=1) +
  stat_smooth(aes(TIME..ms./1000, COST), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_hline(yintercept = cmin, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = c, color = 'black') +
  geom_hline(yintercept = cmax, color = 'black', linetype=2, alpha=0.3) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*0.15), alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./0.15), name="active sensors") +
  scale_color_manual(values = c("ok" = 'blue', "not ok" = 'red'), name = "") +
  labs(x="time (s)", y="cost") + 
  theme(legend.position='none') +
  coord_cartesian(xlim=c(0, 300), ylim=c(0, 0.65))

legend <- get_legend(nt_as_c_m_reli)
nt_as_c_m_pg <- plot_grid(nt_as_c_m_reli+theme(legend.position='none'),nt_as_c_m_cost, ncol=1)
plot_grid(nt_as_c_m_pg, legend, nrow=1, align='h', rel_widths=c(1,0.2))

nt_as_c_c_reli <- nt_as_c_c_data %>%
  mutate(color = if_else(RELIABILITY>=rmin & RELIABILITY<=rmax, "ok", "not ok")) %>%
  ggplot() +
  geom_point(aes(TIME..ms./1000,RELIABILITY*100, color=color), size=1) +
  stat_smooth(aes(TIME..ms./1000,RELIABILITY*100), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*25), alpha=0.3) +
  geom_hline(yintercept = rmin*100, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = r*100, color = 'black') +
  geom_hline(yintercept = rmax*100, color = 'black', linetype=2, alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./25), name="active sensors") +
  scale_color_manual(values = c("ok" = 'blue', "not ok" = 'red', "model" = 'orange'), name = "") +
  labs(title="model behavior for constant active sensors", x="time (s)", y="reliability (%)") +
  coord_cartesian(xlim=c(0, 300), ylim=c(0, 100))

nt_as_c_c_cost <- nt_as_c_c_data %>%
  mutate(color = if_else(cmin<=COST & COST<=cmax,"ok", "not ok")) %>%
  ggplot() +
  geom_point(aes(TIME..ms./1000, COST, color=color), size=1) +
  stat_smooth(aes(TIME..ms./1000, COST), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_hline(yintercept = cmin, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = c, color = 'black') +
  geom_hline(yintercept = cmax, color = 'black', linetype=2, alpha=0.3) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*0.15), alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./0.15), name="active sensors") +
  scale_color_manual(values = c("ok" = 'blue', "not ok" = 'red'), name = "") +
  labs(x="time (s)", y="cost") + 
  theme(legend.position='none') +
  coord_cartesian(xlim=c(0, 300), ylim=c(0, 0.65))

legend <- get_legend(nt_as_c_c_reli)
nt_as_c_c_pg <- plot_grid(nt_as_c_c_reli+theme(legend.position='none'),nt_as_c_c_cost, ncol=1)
plot_grid(nt_as_c_c_pg, legend, nrow=1, align='h', rel_widths=c(1,0.2))

plot_grid(nt_as_c_c_pg, nt_as_c_m_pg, legend, nrow=1, align='h', rel_widths=c(1,1,0.2))

###########################################################
##       Plot constant actives sensors scenario          ##
###########################################################