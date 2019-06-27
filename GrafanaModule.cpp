/**
 * A module which queries a remote Grafana instance
 * to collect metrics to be printed afterwards.
 *
 */


/**
 * Sample query:
 * https://metrics.spironet.fr/api/datasources/proxy/1/query?db=collectd
 * &q=SELECT%20mean(%22value%22)%20FROM%20%22exec_value%22%20WHERE%20(%22type%22%20%3D%20%27temperature%27)
 *    %20AND%20time%20%3E%3D%201561637455000ms%20and%20time%20%3C%3D%201561637465000ms%20GROUP%20BY%20time(1m)%20fill(linear)&epoch=ms
 *
 * urldecoded query:
 * SELECT mean("value") FROM "exec_value" WHERE ("type" = 'temperature') AND time >= 1561637455000ms and time <= 1561637465000ms GROUP BY time(1m) fill(linear)
 *
 */

