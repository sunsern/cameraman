public class SkinScorer {


public static double getScore(double[] fv) { 
double s = 0.0;
s += 1.281326744507232;
if (fv[1] < 0.25204117089247335) {
  s += -0.6176763365395955;
  if (fv[0] < 0.5958646933861949) {
    s += 0.854956140187068;
    if (fv[0] < 0.5459051922820081) {
      s += 0.18344210489560356;
      if (fv[2] < 0.5029411911964417) {
        s += -0.4529694834381747;
      }
      else {
        s += 0.39627639870364334;
        if (fv[0] < 0.33730159723569464) {
          s += -0.4970183423148676;
          if (fv[2] < 0.7558823674917221) {
            s += 0.2416174831444067;
            if (fv[2] < 0.6892157047986984) {
              s += -0.3392420716052645;
            }
            else {
              s += 2.550705212503494;
            }
          }
          else {
            s += -3.3612124501804908;
          }
        }
        else {
          s += 2.051260923982186;
        }
      }
    }
    else {
      s += -1.5476246641168048;
    }
  }
  else {
    s += -1.742501330315489;
    if (fv[0] < 0.6691176669270384) {
      s += 0.23363279463411346;
      if (fv[2] < 0.6098039448261261) {
        s += 0.33766952455828586;
        if (fv[1] < 0.21919816021338806) {
          s += -0.24380298016733987;
        }
        else {
          s += 3.1811107037421085;
        }
        if (fv[0] < 0.6012820711502663) {
          s += -2.6674101907469154;
        }
        else {
          s += 0.7098852247764659;
        }
      }
      else {
        s += -3.1395389600232204;
      }
    }
    else {
      s += -3.6084432300809937;
    }
  }
  if (fv[2] < 0.28921569138765335) {
    s += 0.783348673531274;
    if (fv[1] < 0.09887218674746001) {
      s += -0.24558694348459076;
    }
    else {
      s += 1.4745032318091713;
    }
  }
  else {
    s += -0.2316930313110475;
    if (fv[0] < 0.16369047619047616) {
      s += -3.296829063394856;
    }
    else {
      s += 0.011862496823213043;
    }
  }
}
else {
  s += 0.9819607766255887;
  if (fv[2] < 0.6392157003283501) {
    s += 1.8281016977189752;
    if (fv[2] < 0.27941176854074) {
      s += -0.4936277368603447;
    }
    else {
      s += 3.6600507802392577;
    }
  }
  else {
    s += -0.8639440581274987;
    if (fv[0] < 0.5101525657350464) {
      s += 2.648184758231779;
    }
    else {
      s += -0.512715698216374;
    }
    if (fv[1] < 0.37389158763204566) {
      s += -1.327321514488052;
      if (fv[1] < 0.30816714150047486) {
        s += 0.40120264832864827;
      }
      else {
        s += -1.0194663375822093;
      }
    }
    else {
      s += 0.3635174309805443;
      if (fv[2] < 0.8617647141218185) {
        s += 0.686419193134437;
        if (fv[0] < 0.5978665475940483) {
          s += 3.358556632450719;
        }
        else {
          s += -0.5992592833833869;
        }
        if (fv[1] < 0.8333333373929446) {
          s += -0.4698286586649905;
          if (fv[0] < 0.6504955153637104) {
            s += -1.2856990547933302;
          }
          else {
            s += 1.8531528642175632;
          }
        }
        else {
          s += 3.367630292229243;
        }
      }
      else {
        s += -0.604151147225539;
      }
    }
  }
}
if (fv[0] < 0.1771786587771378) {
  s += -2.1105510769773677;
  if (fv[2] < 0.10784313827753067) {
    s += 2.766561506021685;
  }
  else {
    s += -1.480076625614253;
    if (fv[0] < 0.12220026709610066) {
      s += -0.9023939460508025;
      if (fv[0] < 0.032926827761334154) {
        s += -0.6895809363937758;
      }
      else {
        s += -3.4272455222689735;
      }
    }
    else {
      s += 0.9298583360991042;
      if (fv[1] < 0.034960340415620135) {
        s += 3.1730835910810042;
      }
      else {
        s += -0.27601511254586863;
        if (fv[1] < 0.1279220862583651) {
          s += -2.9321073035494822;
        }
        else {
          s += 1.301550713840488;
        }
      }
    }
  }
}
else {
  s += 0.20724068778344412;
  if (fv[1] < 0.08559218093937426) {
    s += -1.124569633816872;
    if (fv[1] < 0.05263157527930372) {
      s += -0.7298226031047382;
      if (fv[2] < 0.6686274707317352) {
        s += -0.7485205503904137;
      }
      else {
        s += 3.1045440424397586;
      }
    }
    else {
      s += 0.351026561252149;
    }
  }
  else {
    s += 0.47072945667703947;
    if (fv[2] < 0.7078431472182274) {
      s += 0.41866008359717216;
      if (fv[1] < 0.1490295027956809) {
        s += -0.6058308212862175;
        if (fv[0] < 0.582070726270149) {
          s += 0.0245848588026913;
        }
        else {
          s += -1.513410960637163;
          if (fv[2] < 0.353921577334404) {
            s += -0.12085315780688184;
          }
          else {
            s += -3.2638007987605957;
          }
        }
      }
      else {
        s += 0.7991873790475558;
        if (fv[0] < 0.5921052807236017) {
          s += 3.539440569298824;
        }
        else {
          s += -0.4246203171960445;
        }
      }
      if (fv[2] < 0.5882353112101555) {
        s += 0.6111714964020918;
        if (fv[0] < 0.6628788076243337) {
          s += 0.4515650472435656;
        }
        else {
          s += -2.466495710377499;
        }
      }
      else {
        s += -0.48957139883358414;
        if (fv[2] < 0.6401960924267769) {
          s += -1.5013111258056309;
        }
        else {
          s += 0.016682795485732916;
          if (fv[2] < 0.6823529526591301) {
            s += 0.7420167845519627;
            if (fv[0] < 0.5888390928537109) {
              s += 3.0572297141489497;
            }
            else {
              s += -0.5978833778489168;
            }
          }
          else {
            s += -1.4578307878265495;
          }
        }
      }
    }
    else {
      s += -0.7895661423569831;
      if (fv[2] < 0.7480392307043076) {
        s += 0.74905206169836;
      }
      else {
        s += -0.17459280201169847;
      }
    }
    if (fv[1] < 0.10240062418829776) {
      s += 2.1833450016656735;
    }
    else {
      s += -0.04427723996467672;
    }
  }
  if (fv[0] < 0.57536966544334) {
    s += 0.43372394315902146;
    if (fv[0] < 0.26867335057465463) {
      s += -0.9574466291335925;
      if (fv[2] < 0.4019607901573181) {
        s += -1.2721327029944371;
      }
      else {
        s += 0.25787993940621723;
        if (fv[2] < 0.5137255042791367) {
          s += 3.2106746213906465;
        }
        else {
          s += -0.22461466248993553;
        }
      }
      if (fv[2] < 0.6205882504582405) {
        s += -0.7964932434047847;
      }
      else {
        s += 0.5736983428867428;
      }
    }
    else {
      s += 0.2797307491897998;
      if (fv[2] < 0.875490203499794) {
        s += 0.23993877909955008;
        if (fv[1] < 0.03821810912925448) {
          s += -2.4564691492746866;
        }
        else {
          s += 0.2827568483310265;
        }
        if (fv[0] < 0.5330808195050354) {
          s += 0.47297388826273523;
          if (fv[1] < 0.07905264369300885) {
            s += -0.6249098267378047;
            if (fv[2] < 0.4676470682024956) {
              s += -1.123121217696596;
            }
            else {
              s += 0.4965130753079471;
              if (fv[1] < 0.07846475714218093) {
                s += 1.2559546080834079;
              }
              else {
                s += -1.5607932389675603;
              }
            }
          }
          else {
            s += 1.5767163576717471;
            if (fv[0] < 0.2858751708092684) {
              s += -1.4598277847895438;
            }
            else {
              s += 3.0423560402785452;
            }
          }
        }
        else {
          s += -0.6905169684728664;
          if (fv[1] < 0.08230104268781749) {
            s += -3.208518518194832;
          }
          else {
            s += 0.2560550367918436;
            if (fv[2] < 0.8696078509092331) {
              s += 0.6671252247086494;
              if (fv[0] < 0.5664215758296829) {
                s += 0.6032989554449394;
              }
              else {
                s += -1.3796364161515802;
              }
            }
            else {
              s += -0.49328242342526674;
            }
          }
        }
        if (fv[2] < 0.33627451956272125) {
          s += 3.1219197237416503;
        }
        else {
          s += -0.08829950442020305;
        }
      }
      else {
        s += -1.6416896793956282;
        if (fv[0] < 0.5534705606268913) {
          s += 0.6688741540816973;
          if (fv[2] < 0.9009803980588913) {
            s += 3.2010199616020887;
          }
          else {
            s += -0.3307532312560152;
            if (fv[0] < 0.5069960000784809) {
              s += 3.0531761523372944;
            }
            else {
              s += -3.053948654107697;
            }
          }
        }
        else {
          s += -2.9835764577647352;
        }
      }
    }
  }
  else {
    s += -0.5526718010126586;
    if (fv[2] < 0.3607843182981014) {
      s += 1.7164434031074842;
    }
    else {
      s += -0.5193421106064777;
      if (fv[2] < 0.946078434586525) {
        s += -0.1298828676923702;
        if (fv[1] < 0.100525445109043) {
          s += -3.1412915878839285;
        }
        else {
          s += -0.038780312222852796;
          if (fv[0] < 0.7236111309793258) {
            s += 0.07058386614854577;
            if (fv[0] < 0.635690809341899) {
              s += -0.22124842105614723;
              if (fv[2] < 0.9127451032400131) {
                s += 0.1054013428695628;
                if (fv[2] < 0.9107843190431595) {
                  s += -0.08777546548258712;
                  if (fv[2] < 0.7852941304445267) {
                    s += 0.1572863109529067;
                    if (fv[1] < 0.44198287203139985) {
                      s += -0.32615523039451966;
                      if (fv[0] < 0.6212121450057448) {
                        s += 0.24420591196673047;
                      }
                      else {
                        s += -3.3463421908461055;
                      }
                    }
                    else {
                      s += 1.288664016181761;
                      if (fv[0] < 0.6301370061695848) {
                        s += 3.149843006944442;
                      }
                      else {
                        s += -3.0473732644558194;
                      }
                    }
                  }
                  else {
                    s += -1.6365709653063696;
                  }
                }
                else {
                  s += 3.2435519548735727;
                }
              }
              else {
                s += -3.2153266280396093;
              }
            }
            else {
              s += 0.8831676104677542;
            }
          }
          else {
            s += -2.9769515270649913;
          }
        }
      }
      else {
        s += 3.1464733785648336;
      }
    }
  }
}
if (fv[2] < 0.2372549083083868) {
  s += 1.444191289908656;
}
else {
  s += -0.02983852190231842;
}
return s;
}

}