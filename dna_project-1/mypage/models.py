from django.db import models
from schedule.models import Location_weight

# Create your models here.
class User_info(models.Model):
    identifier = models.CharField(max_length=10, null=False, primary_key = True, default = '')
    password = models.CharField(max_length=10, null=False, default = '')
    name = models.CharField(max_length=5, default = '')
    Loc_per = models.BooleanField(null=False, default = False)

    def __str__(self):
        return self.identifier

class Travel_info(models.Model):
    identifier = models.ForeignKey(
        'User_info',
        on_delete=models.CASCADE,
        default = '',
    )
    start_date = models.DateTimeField(null=False)
    end_date = models.DateTimeField(null=False)
    start_loc = models.ForeignKey(
        'schedule.Location_weight',
        related_name = "start_location",
        default = '',
    )
    end_loc = models.ForeignKey(
        'schedule.Location_weight',
        related_name = "end_location",
        default = '',
    )
    Travel_list = models.CharField(max_length=100, default = '')
    is_done = models.BooleanField(default = False)

    def __str__(self):
        return self.Travel_list
