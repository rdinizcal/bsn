# Experiments for 3C

## Reliability analysis

nc_rel <- nc_data %>% 
  mutate(color = if_else(RELIABILITY>0.88 & RELIABILITY<.92, "88%<x<92%", "x>88% or x>92%")) %>%
  ggplot() + 
  geom_point(aes(TIME..ms./1000,RELIABILITY*100, color=color)) +
  stat_smooth(aes(TIME..ms./1000,RELIABILITY*100), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_hline(yintercept = 88, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = 90, color = 'black') +
  geom_hline(yintercept = 92, color = 'black', linetype=2, alpha=0.3) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*25), alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./25, name="active sensors")) +
  scale_color_manual(values = c("88%<x<92%" = 'blue', "x>88% or x>92%" = 'red'), name = "") +
  labs(title="BSN overall reliability over time for non-controlled system",x="time (s)", y="reliability (%)") 

c_rel <- c_data %>% 
  mutate(color = if_else(RELIABILITY>0.88 & RELIABILITY<.92, "88%<x<92%", "x>88% or x>92%")) %>%
  ggplot() + 
  geom_point(aes(TIME..ms./1000,RELIABILITY*100, color=color)) +
  stat_smooth(aes(TIME..ms./1000,RELIABILITY*100), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_hline(yintercept = 88, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = 90, color = 'black') +
  geom_hline(yintercept = 92, color = 'black', linetype=2, alpha=0.3) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*25), alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./25, name="active sensors")) +
  scale_color_manual(values = c("88%<x<92%" = 'blue', "x>88% or x>92%" = 'red'), name = "") +
  labs(title="BSN overall reliability over time for controlled system",x="time (s)", y="reliability (%)")

rel <- plot_grid(nc_rel, c_rel, ncol = 1, align = 'v')

## Cost analysis

nc_cost <- nc_data %>% 
  mutate(color = if_else(0.0050<COST & COST<0.0056, "0.0050<x<0.0056", "x<0.0050 or x>0.0056")) %>%
  ggplot() + 
  geom_point(aes(TIME..ms./1000, COST, color=color)) +
  stat_smooth(aes(TIME..ms./1000,COST), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_hline(yintercept = 0.0056, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = 0.0053, color = 'black') +
  geom_hline(yintercept = 0.0050, color = 'black', linetype=2, alpha=0.3) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*0.0015), alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./0.0015, name="active sensors")) +
  scale_color_manual(values = c("0.0050<x<0.0056" = 'blue', "x<0.0050 or x>0.0056" = 'red'), name = "") +
  labs(title="BSN overall cost over time for non-controlled system",x="time (s)", y="cost") 

c_cost <- c_data %>% 
  mutate(color = if_else(0.0050<COST & COST<0.0056, "0.0050<x<0.0056", "x<0.0050 or x>0.0056")) %>%
  ggplot() + 
  geom_point(aes(TIME..ms./1000, COST, color=color)) +
  stat_smooth(aes(TIME..ms./1000,COST), color='black', method = lm, formula = y ~ poly(x, 25), se = FALSE) +
  geom_hline(yintercept = 0.0056, color = 'black', linetype=2, alpha=0.3) +
  geom_hline(yintercept = 0.0053, color = 'black') +
  geom_hline(yintercept = 0.0050, color = 'black', linetype=2, alpha=0.3) +
  geom_line(aes(TIME..ms./1000,(OXIM+ECG+TEMP+ABP)*0.0015), alpha=0.3) +
  scale_y_continuous(sec.axis = sec_axis(~ ./0.0015, name="active sensors")) +
  scale_color_manual(values = c("0.0050<x<0.0056" = 'blue', "x<0.0050 or x>0.0056" = 'red'), name = "") +
  labs(title="BSN overall cost over time for controlled system",x="time (s)", y="cost")


cost <- plot_grid(nc_cost, c_cost, ncol = 1, align = 'v')

plot_grid(rel, cost)