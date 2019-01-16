# plot_4A

num_id <- factor(mean_time$id,levels = c("1", "2", "3", "4", "5", "6", "7", "8", "9", "10"))

ggplot(data=mean_time) +
  geom_point(aes(num_id,average/1000)) + 
  geom_errorbar(aes(num_id,ymin=(average-stddev)/1000,ymax=(average+stddev)/1000)) + 
  labs(x="experiment", y="formulae evaluation\nmean time (ms)") + 
  coord_cartesian(ylim=c(0, 160))