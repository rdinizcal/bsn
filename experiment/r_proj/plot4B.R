# Experiments for 4B
library(tidyverse)
library(ggplot2)
library(cowplot) 

r <- 0.90
rmin <- r*0.98
rmax <- r*1.02

nt_m_data %>% 
  mutate(color = if_else(RELIABILITY>=rmin & RELIABILITY<=rmax, "ok", "not ok")) %>%
  ggplot() + 
  geom_point(aes(TIME..ms./1000,RELIABILITY*100, color=color), size=0.5) +
  geom_point(data=nt_c_data,aes(TIME..ms./1000,RELIABILITY*100, color="model"), size=0.5) +
  #stat_smooth(aes(TIME..ms./1000,RELIABILITY*100, color='system'), method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  #stat_smooth(data=nt_c_data, aes(TIME..ms./1000,RELIABILITY*100, color='model'), method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_hline(yintercept = rmin*100, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = r*100, color = 'black') +
  geom_hline(yintercept = rmax*100, color = 'black', linetype=2, alpha=0.3) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*25), alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./25, name="active sensors")) +
  scale_color_manual(values = c("ok" = 'blue', "not ok" = 'red', "system" = 'black', "model" = 'orange'), name = "") +
  labs(title="overall reliability for static estimated reliability and cost", x="time (s)", y="reliability (%)") +
  coord_cartesian(xlim=c(0, 300), ylim=c(0, 100))


## Reliability analysis

r <- 0.90
rmin <- r*0.98
rmax <- r*1.02

r_rel <- r_data %>% 
  mutate(color = if_else(RELIABILITY>=rmin & RELIABILITY<=rmax, "ok", "not ok")) %>%
  ggplot() + 
  geom_point(aes(TIME..ms./1000,RELIABILITY*100, color=color)) +
  stat_smooth(aes(TIME..ms./1000,RELIABILITY*100), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_hline(yintercept = rmin*100, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = r*100, color = 'black') +
  geom_hline(yintercept = rmax*100, color = 'black', linetype=2, alpha=0.3) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*25), alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./25, name="active sensors")) +
  scale_color_manual(values = c("ok" = 'blue', "not ok" = 'red'), name = "") +
  labs(title="overall reliability for dynamic randomized reliability and cost",x="time (s)", y="reliability (%)") +
  coord_cartesian(xlim=c(0, 300), ylim=c(0, 100))

nc_rel <- nc_data %>% 
  mutate(color = if_else(RELIABILITY>=rmin & RELIABILITY<=rmax, "ok", "not ok")) %>%
  ggplot() + 
  geom_point(aes(TIME..ms./1000,RELIABILITY*100, color=color)) +
  stat_smooth(aes(TIME..ms./1000,RELIABILITY*100), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_hline(yintercept = rmin*100, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = r*100, color = 'black') +
  geom_hline(yintercept = rmax*100, color = 'black', linetype=2, alpha=0.3) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*25), alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./25, name="active sensors")) +
  scale_color_manual(values = c("ok" = 'blue', "not ok" = 'red'), name = "") +
  labs(title="overall reliability for static estimated reliability and cost",x="time (s)", y="reliability (%)") +
  coord_cartesian(xlim=c(0, 300), ylim=c(0, 100))

c_rel <- c_data %>% 
  mutate(color = if_else(RELIABILITY>=rmin & RELIABILITY<=rmax, "ok", "not ok")) %>%
  ggplot() + 
  geom_point(aes(TIME..ms./1000,RELIABILITY*100, color=color)) +
  stat_smooth(aes(TIME..ms./1000,RELIABILITY*100), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_hline(yintercept = rmin*100, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = r*100, color = 'black') +
  geom_hline(yintercept = rmax*100, color = 'black', linetype=2, alpha=0.3) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*25), alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./25, name="active sensors")) +
  scale_color_manual(values = c("ok" = 'blue', "not ok" = 'red'), name = "") +
  labs(title="overall reliability for static known reliability and cost", x="time (s)", y="reliability (%)") +
  coord_cartesian(xlim=c(0, 300), ylim=c(0, 100))

rel <- plot_grid(r_rel, nc_rel, c_rel, nrow=1)

## Cost analysis

c <- 0.47
cmin <- c*0.98
cmax <- c*1.02

r_cost <- r_data %>% 
  mutate(color = if_else(cmin<=COST & COST<=cmax,"ok", "not ok")) %>%
  ggplot() + 
  geom_point(aes(TIME..ms./1000,COST, color=color)) +
  stat_smooth(aes(TIME..ms./1000,COST), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_hline(yintercept = cmin, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = c, color = 'black') +
  geom_hline(yintercept = cmax, color = 'black', linetype=2, alpha=0.3) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*1.5), alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./1.5, name="active sensors")) +
  scale_color_manual(values = c("ok" = 'blue', "not ok" = 'red'), name = "") +
  labs(title="overall cost for dynamic randomized reliability and cost",x="time (s)", y="cost") +
  coord_cartesian(xlim=c(0, 300), ylim=c(0, 6))

nc_cost <- nc_data %>% 
  mutate(color = if_else(cmin<=COST & COST<=cmax,"ok", "not ok")) %>%
  ggplot() + 
  geom_point(aes(TIME..ms./1000,COST, color=color)) +
  stat_smooth(aes(TIME..ms./1000,COST), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_hline(yintercept = cmin, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = c, color = 'black') +
  geom_hline(yintercept = cmax, color = 'black', linetype=2, alpha=0.3) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*0.15), alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./0.15, name="active sensors")) +
  scale_color_manual(values = c("ok" = 'blue', "not ok" = 'red'), name = "") +
  labs(title="overall cost for static estimated reliability and cost",x="time (s)", y="cost") +
  coord_cartesian(xlim=c(0, 300), ylim=c(0, 0.6))

c_cost <- c_data %>% 
  mutate(color = if_else(cmin<=COST & COST<=cmax,"ok", "not ok")) %>%
  ggplot() + 
  geom_point(aes(TIME..ms./1000, COST, color=color)) +
  stat_smooth(aes(TIME..ms./1000, COST), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_hline(yintercept = cmin, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = c, color = 'black') +
  geom_hline(yintercept = cmax, color = 'black', linetype=2, alpha=0.3) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*0.15), alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./0.15, name="active sensors")) +
  scale_color_manual(values = c("ok" = 'blue', "not ok" = 'red'), name = "") +
  labs(title="overall reliability for static known reliability and cost",x="time (s)", y="cost") + 
  coord_cartesian(xlim=c(0, 300), ylim=c(0, 0.6))

cost <- plot_grid(r_cost, nc_cost, c_cost, nrow=1)

# plot all
plot_grid(rel, cost, ncol = 1, align = 'v')