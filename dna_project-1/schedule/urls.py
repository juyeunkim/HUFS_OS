from django.conf.urls import url
from . import views

urlpatterns = [
    url(r'^', views.schedule, name='schedule'),
    url(r'^state_name',views.state_name, name="state_name"),
]
