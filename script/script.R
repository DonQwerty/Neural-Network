library(ggplot2)
library(reshape2)
#datos1<-read.csv("stats_1.txt",sep=" ")
datos2<-read.csv("stats_0.5.txt",sep=" ")
datos3<-read.csv("stats_0.1.txt",sep=" ")
datos4<-read.csv("stats_0.02.txt",sep=" ")
datos<-datos2
datos$Epoca<-datos2$Epoca
datos$Pesos<-NULL
datos$Error<-NULL
datos$Err0.02<-datos4$Error
datos$Err0.1<-datos3$Error
datos$Err0.5<-datos2$Error
#datos$Err1<-datos1$Error
datos<-melt(datos,c("Epoca"))
names(datos)<-c("Epoca","Constante_aprendizaje","Error")
ggplot(datos, aes(x = Epoca, y = Error, colour=Constante_aprendizaje)) +
  geom_line()


