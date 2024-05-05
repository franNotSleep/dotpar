interface Configuration {
  enterprise: {
    PORT: number;
    database: {
      POSTGRES_DATABASE: string;
      POSTGRES_PORT: number;
    };
    auth: {
      JWT_ACCESS_SECRET: string;
      JWT_REFRESH_SECRET: string;
    };
  };
  ecommerce: {
    PORT: number;
    database: {
      ECOMMERCE_DATABASE: string;
      ECOMMERCE_PORT: number;
    };
    auth: {
      ECOMMERCE_ACCESS_SECRET: string;
      ECOMMERCE_REFRESH_SECRET: string;
    };
  };
}
