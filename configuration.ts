interface Configuration {
  enterprise: {
    port: number;
    database: {
      postgresDatabase: string;
      postgresPort: number;
    };
    auth: {
      jwtAccessSecret: string;
      jwtRefreshSecret: string;
    };
  };
  ecommerce: {
    port: number;
    database: {
      ecommerceDatabase: string;
      ecommercePort: number;
    };
    auth: {
      ecommerceAccessSecret: string;
      ecommerceRefreshSecret: string;
    };
  };
};
