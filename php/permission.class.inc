<?php

namespace LORIS\biobank;

enum Permission: string
{
    case GET = 'GET';
    case PUT = 'PUT';
    case DELETE = 'DELETE';
    case POST = 'POST';
}
